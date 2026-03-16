#include "cCodeGen.h"
#include "astnodes.h"
#include "emit.h"
#include <iostream>

cCodeGen::cCodeGen(const std::string& filename) : m_filename(filename)
{
    if (!InitOutput(filename))
    {
        std::cerr << "ERROR: Failed to initialize output file\n";
    }
}

cCodeGen::~cCodeGen()
{
    FinalizeOutput();
}

void cCodeGen::EmitCode(const std::string& code)
{
    EmitString(code);
}

std::string cCodeGen::GenerateLabel()
{
    return ::GenerateLabel();
}

void cCodeGen::Visit(cProgramNode *node)
{
    if (node == nullptr) return;

    cBlockNode *block = node->GetBlock();

    // Emit all function definitions BEFORE emitting main,
    // so forward references work in the assembler.
    if (block && block->GetDecls())
    {
        block->GetDecls()->Visit(this);
    }

    int progSize = node->GetProgramSize();
    EmitCode(".function main\n");
    EmitCode("main:\n");
    // Allocate one extra word at the end of the frame to save FP before any
    // POPVAR/POPVARIND operations mutate what PUSHFP returns.
    // Word-align the FP save offset to avoid misaligned access.
    m_fpSaveOffset = (progSize + 3) & ~3;  // round up to next 4-byte boundary
    if (progSize > 0 || m_fpSaveOffset > 0)
    {
        EmitCode("ADJSP " + std::to_string(m_fpSaveOffset + 4) + "\n");
    }
    EmitCode("PUSHFP\n");
    EmitCode("POPVAR " + std::to_string(m_fpSaveOffset) + "\n");

    if (block && block->GetStmts())
    {
        block->GetStmts()->Visit(this);
    }

    EmitCode("PUSH 0\n");
    EmitCode("RETURNV\n");
}

void cCodeGen::Visit(cBlockNode *node)
{
    if (node == nullptr) return;

    // Only visit statements — declarations don't generate code
    cStmtsNode *stmts = node->GetStmts();
    if (stmts)
    {
        stmts->Visit(this);
    }
}

void cCodeGen::Visit(cDeclsNode *node)
{
    if (node)
    {
        node->VisitAllChildren(this);
    }
}

void cCodeGen::Visit(cVarDeclNode *node)
{
    // Variable declarations don't generate code
}

void cCodeGen::Visit(cArrayDeclNode *node)
{
    // Array declarations don't generate code
}

void cCodeGen::Visit(cFuncDeclNode *node)
{
    // Function prototypes don't generate code
}

void cCodeGen::Visit(cFuncNode *node)
{
    if (node == nullptr) return;

    // Prototypes have no body — skip them
    cStmtsNode *stmts = node->GetStmts();
    if (stmts == nullptr) return;

    cSymbol *funcName = node->GetNameSymbol();
    if (funcName == nullptr) return;

    EmitCode(".function " + funcName->GetName() + "\n");
    EmitCode(funcName->GetName() + ":\n");

    int funcSize = node->GetFuncSize();
    // Allocate one extra word to save FP before any POPVAR/POPVARIND mutations.
    // Word-align the FP save offset to avoid misaligned access.
    m_fpSaveOffset = (funcSize + 3) & ~3;  // round up to next 4-byte boundary
    if (funcSize > 0 || m_fpSaveOffset > 0)
    {
        EmitCode("ADJSP " + std::to_string(m_fpSaveOffset + 4) + "\n");
    }
    EmitCode("PUSHFP\n");
    EmitCode("POPVAR " + std::to_string(m_fpSaveOffset) + "\n");

    stmts->Visit(this);

    EmitCode("PUSH 0\n");
    EmitCode("RETURNV\n");
    EmitCode("\n");
}

void cCodeGen::Visit(cStmtsNode *node)
{
    if (node)
    {
        node->VisitAllChildren(this);
    }
}

void cCodeGen::Visit(cStmtNode *node)
{
    if (node)
    {
        node->VisitAllChildren(this);
    }
}

void cCodeGen::Visit(cIfNode *node)
{
    if (node == nullptr) return;

    std::string elseLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    cExprNode *condition = node->GetCondition();
    if (condition)
    {
        condition->Visit(this);
    }

    EmitCode("JUMPE @" + elseLabel + "\n");

    cStmtsNode *thenStmts = node->GetThenStmts();
    if (thenStmts)
    {
        thenStmts->Visit(this);
    }

    EmitCode("JUMP @" + endLabel + "\n");
    EmitCode(elseLabel + ":\n");

    cStmtsNode *elseStmts = node->GetElseStmts();
    if (elseStmts)
    {
        elseStmts->Visit(this);
    }

    EmitCode(endLabel + ":\n");
}

void cCodeGen::Visit(cWhileNode *node)
{
    if (node == nullptr) return;

    std::string loopLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    EmitCode(loopLabel + ":\n");

    cExprNode *condition = node->GetCondition();
    if (condition)
    {
        condition->Visit(this);
    }

    EmitCode("JUMPE @" + endLabel + "\n");

    cStmtNode *body = node->GetBody();
    if (body)
    {
        body->Visit(this);
    }

    EmitCode("JUMP @" + loopLabel + "\n");
    EmitCode(endLabel + ":\n");
}

void cCodeGen::Visit(cAssignNode *node)
{
    if (node == nullptr) return;

    cExprNode *rval = node->GetRval();
    if (rval) rval->Visit(this);   // push value

    cExprNode *lvalExpr = node->GetLval();
    cVarExprNode *lval = dynamic_cast<cVarExprNode*>(lvalExpr);
    if (lval && lval->GetBaseSymbol())
    {
        cSymbol *sym = lval->GetBaseSymbol();
        cDeclNode *baseDecl = sym->GetDecl();
        int baseOffset = (baseDecl != nullptr) ? baseDecl->GetDeclOffset() : 0;

        if (lval->GetPartCount() == 1)
        {
            // Simple variable assignment - use POPCVAR for 1-byte types (char)
            int varSize = (baseDecl != nullptr) ? baseDecl->GetDeclSize() : 4;
            if (varSize == 1)
                EmitCode("POPCVAR " + std::to_string(baseOffset) + "\n");
            else
                EmitCode("POPVAR " + std::to_string(baseOffset) + "\n");
            return;
        }

        // Complex access: struct fields and/or array indices
        // Stack currently has: [value]
        int staticOffset = baseOffset;
        bool needsIndirect = false;
        int finalElemSize = 4;
        cDeclNode *currentDecl = baseDecl;

        for (int i = 1; i < lval->GetPartCount(); i++)
        {
            cAstNode *part = lval->GetPart(i);

            // Check for struct field access (cSymbol)
            cSymbol *fieldSym = dynamic_cast<cSymbol*>(part);
            if (fieldSym != nullptr)
            {
                // Navigate to struct type
                cDeclNode *container = currentDecl;
                if (container != nullptr && container->IsVar())
                {
                    container = container->GetType();
                }
                cStructDeclNode *structDecl = dynamic_cast<cStructDeclNode*>(container);
                if (structDecl != nullptr)
                {
                    cDeclsNode *fields = structDecl->GetDecls();
                    if (fields != nullptr)
                    {
                        for (int j = 0; j < fields->GetDeclCount(); j++)
                        {
                            cDeclNode *field = fields->GetDecl(j);
                            if (field != nullptr && field->GetName() == fieldSym->GetName())
                            {
                                staticOffset += field->GetDeclOffset();
                                currentDecl = field;
                                finalElemSize = field->GetDeclSize();
                                break;
                            }
                        }
                    }
                }
                continue;
            }

            // Check for array index (cExprNode)
            cExprNode *indexExpr = dynamic_cast<cExprNode*>(part);
            if (indexExpr != nullptr)
            {
                // Get element size from array declaration
                int elemSize = 4;
                cArrayDeclNode *arrDecl = dynamic_cast<cArrayDeclNode*>(currentDecl);
                bool isArrayParam = (arrDecl != nullptr && arrDecl->GetCount() == 0);
                if (arrDecl != nullptr && arrDecl->GetElementDecl())
                {
                    elemSize = arrDecl->GetElementDecl()->GetDeclSize();
                    if (elemSize == 0) elemSize = arrDecl->GetElementDecl()->GetSize();
                    currentDecl = arrDecl->GetElementDecl();
                }
                finalElemSize = elemSize;

                // For array index, we need indirect access
                // Stack has: [value]
                indexExpr->Visit(this);                              // push index -> [value, index]
                EmitCode("PUSH " + std::to_string(elemSize) + "\n"); // -> [value, index, elemSize]
                EmitCode("TIMES\n");                                  // -> [value, index*elemSize]
                
                if (isArrayParam)
                {
                    // Array parameter: staticOffset points to the pointer, load it first
                    EmitCode("PUSHVAR " + std::to_string(staticOffset) + "\n"); // load array pointer
                    EmitCode("PLUS\n");                               // pointer + index*elemSize
                }
                else
                {
                    // Local array: compute address from FP + offset
                    EmitCode("PUSH " + std::to_string(staticOffset) + "\n");   // -> [value, offset, staticOffset]
                    EmitCode("PUSHVAR " + std::to_string(m_fpSaveOffset) + "\n"); // -> [value, offset, FP]
                    EmitCode("PLUS\n");                                   // -> [value, FP+offset]
                    EmitCode("PLUS\n");                                   // -> [value, address]
                }

                // Reset static offset since it's now on the stack
                staticOffset = 0;
                needsIndirect = true;
                continue;
            }
        }

        if (needsIndirect)
        {
            // Stack has: [value, address]
            // Add any remaining static offset
            if (staticOffset > 0)
            {
                EmitCode("PUSH " + std::to_string(staticOffset) + "\n");
                EmitCode("PLUS\n");
            }
            // Use POPCVARIND for 1-byte elements (char)
            if (finalElemSize == 1)
                EmitCode("POPCVARIND\n");
            else
                EmitCode("POPVARIND\n");
        }
        else
        {
            // Simple offset access (struct fields only, no array indices)
            if (finalElemSize == 1)
                EmitCode("POPCVAR " + std::to_string(staticOffset) + "\n");
            else
                EmitCode("POPVAR " + std::to_string(staticOffset) + "\n");
        }
    }
}

void cCodeGen::Visit(cReturnNode *node)
{
    if (node == nullptr) return;

    node->VisitAllChildren(this);

    EmitCode("RETURNV\n");
}

void cCodeGen::Visit(cPrintNode *node)
{
    if (node == nullptr) return;

    node->VisitAllChildren(this);

    EmitCode("CALL @print\n");
    EmitCode("POP\n");
}

void cCodeGen::Visit(cPrintsNode *node)
{
    if (node == nullptr) return;

    std::string label = GenerateLabel();
    std::string str = node->GetString();

    EmitCode("PUSH @" + label + "\n");
    EmitCode(".dataseg\n");
    EmitCode(label + ":\n");
    EmitCode(".string \"" + str + "\\n\"\n");
    EmitCode(".codeseg\n");
    EmitCode("OUTS\n");
}

void cCodeGen::Visit(cFuncCallNode *node)
{
    if (node == nullptr) return;

    // Get the function declaration to access formal parameter types
    cSymbol *funcSym = node->GetFuncSymbol();
    cFuncDeclNode *funcDecl = nullptr;
    cArgsNode *formalArgs = nullptr;
    if (funcSym && funcSym->GetDecl())
    {
        funcDecl = dynamic_cast<cFuncDeclNode*>(funcSym->GetDecl());
        if (funcDecl) formalArgs = funcDecl->GetArgs();
    }

    // Push arguments in reverse order so arg[0] lands at FP-12 after CALL
    cParamsNode *params = node->GetParams();
    if (params)
    {
        for (int i = params->GetParamCount() - 1; i >= 0; i--)
        {
            cExprNode *param = params->GetParam(i);
            if (!param) continue;

            // Check if formal parameter is an array reference (count=0)
            bool formalIsArrayRef = false;
            if (formalArgs && i < formalArgs->GetArgCount())
            {
                cDeclNode *formalArg = formalArgs->GetArgDecl(i);
                cArrayDeclNode *arrDecl = dynamic_cast<cArrayDeclNode*>(formalArg);
                if (arrDecl && arrDecl->GetCount() == 0)
                    formalIsArrayRef = true;
            }

            // Check if actual parameter is an array variable without index
            cVarExprNode *varExpr = dynamic_cast<cVarExprNode*>(param);
            bool actualIsWholeArray = false;
            int arrayOffset = 0;
            if (varExpr && varExpr->GetPartCount() == 1)  // Just base name, no index
            {
                cSymbol *baseSym = varExpr->GetBaseSymbol();
                if (baseSym)
                {
                    cDeclNode *baseDecl = baseSym->GetDecl();
                    // Check if it's an array (IsArray) with count > 0 (actual array, not reference)
                    cArrayDeclNode *srcArr = dynamic_cast<cArrayDeclNode*>(baseDecl);
                    if (srcArr && srcArr->GetCount() > 0)
                    {
                        actualIsWholeArray = true;
                        arrayOffset = baseDecl->GetDeclOffset();
                    }
                    // Also check VarDeclNode pointing to array type
                    cVarDeclNode *varDecl = dynamic_cast<cVarDeclNode*>(baseDecl);
                    if (varDecl && varDecl->IsArray())
                    {
                        cSymbol *typeSym = varDecl->GetTypeSym();
                        if (typeSym)
                        {
                            cArrayDeclNode *typeArr = dynamic_cast<cArrayDeclNode*>(typeSym->GetDecl());
                            if (typeArr && typeArr->GetCount() > 0)
                            {
                                actualIsWholeArray = true;
                                arrayOffset = baseDecl->GetDeclOffset();
                            }
                        }
                    }
                }
            }

            // If passing array to array parameter, push address instead of value
            if (formalIsArrayRef && actualIsWholeArray)
            {
                EmitCode("PUSHVAR " + std::to_string(m_fpSaveOffset) + "\n");  // saved FP
                EmitCode("PUSH " + std::to_string(arrayOffset) + "\n");
                EmitCode("PLUS\n");  // address of array
            }
            else
            {
                param->Visit(this);
            }
        }
    }

    // Emit the function call
    if (funcSym) EmitCode("CALL @" + funcSym->GetName() + "\n");

    // Clean up stale args from below the return value on the stack.
    // Each CALL leaves its pushed args beneath the return value;
    // SWAP+POP removes one arg slot, preserving the return value on top.
    if (params)
    {
        for (int i = 0; i < params->GetParamCount(); i++)
        {
            EmitCode("SWAP\nPOP\n");
        }
    }
}

void cCodeGen::Visit(cExprNode *node)
{
    if (node)
    {
        node->VisitAllChildren(this);
    }
}

void cCodeGen::Visit(cIntExprNode *node)
{
    if (node == nullptr) return;
    
    int value = node->GetValue();
    EmitCode("PUSH " + std::to_string(value) + "\n");
}

void cCodeGen::Visit(cBinaryExprNode *node)
{
    if (node == nullptr) return;

    cExprNode *left = node->GetLeft();
    if (left)
    {
        left->Visit(this);
    }

    cExprNode *right = node->GetRight();
    if (right)
    {
        right->Visit(this);
    }

    cOpNode *op = node->GetOp();
    if (op)
    {
        int opCode = op->GetOp();
        
        switch(opCode)
        {
            case '+': EmitCode("PLUS\n"); break;
            case '-': EmitCode("MINUS\n"); break;
            case '*': EmitCode("TIMES\n"); break;
            case '/': EmitCode("DIVIDE\n"); break;
            case '%': EmitCode("MOD\n"); break;
            case '<': EmitCode("LT\n"); break;
            case '>': EmitCode("GT\n"); break;
            case LE: EmitCode("LE\n"); break;
            case GE: EmitCode("GE\n"); break;
            case EQUALS: EmitCode("EQ\n"); break;
            case NOT_EQUALS: EmitCode("NE\n"); break;
            case AND: EmitCode("AND\n"); break;
            case OR: EmitCode("OR\n"); break;
            default: break;
        }
    }
}

void cCodeGen::Visit(cVarExprNode *node)
{
    if (node == nullptr) return;

    cSymbol *baseSym = node->GetBaseSymbol();
    if (baseSym == nullptr) return;

    cDeclNode *baseDecl = baseSym->GetDecl();
    int baseOffset = (baseDecl != nullptr) ? baseDecl->GetDeclOffset() : 0;

    if (node->GetPartCount() == 1)
    {
        // Simple variable - use PUSHCVAR for 1-byte types (char)
        int varSize = (baseDecl != nullptr) ? baseDecl->GetDeclSize() : 4;
        if (varSize == 1)
            EmitCode("PUSHCVAR " + std::to_string(baseOffset) + "\n");
        else
            EmitCode("PUSHVAR " + std::to_string(baseOffset) + "\n");
        return;
    }

    // Complex access: struct fields and/or array indices
    // Compute the final offset and whether we need indirect access
    int staticOffset = baseOffset;
    bool needsIndirect = false;
    int finalElemSize = 4;
    cDeclNode *currentDecl = baseDecl;

    for (int i = 1; i < node->GetPartCount(); i++)
    {
        cAstNode *part = node->GetPart(i);

        // Check for struct field access (cSymbol)
        cSymbol *fieldSym = dynamic_cast<cSymbol*>(part);
        if (fieldSym != nullptr)
        {
            // Navigate to struct type
            cDeclNode *container = currentDecl;
            if (container != nullptr && container->IsVar())
            {
                container = container->GetType();
            }
            cStructDeclNode *structDecl = dynamic_cast<cStructDeclNode*>(container);
            if (structDecl != nullptr)
            {
                cDeclsNode *fields = structDecl->GetDecls();
                if (fields != nullptr)
                {
                    for (int j = 0; j < fields->GetDeclCount(); j++)
                    {
                        cDeclNode *field = fields->GetDecl(j);
                        if (field != nullptr && field->GetName() == fieldSym->GetName())
                        {
                            staticOffset += field->GetDeclOffset();
                            currentDecl = field;
                            finalElemSize = field->GetDeclSize();
                            break;
                        }
                    }
                }
            }
            continue;
        }

        // Check for array index (cExprNode)
        cExprNode *indexExpr = dynamic_cast<cExprNode*>(part);
        if (indexExpr != nullptr)
        {
            // Get element size from array declaration
            int elemSize = 4;
            cArrayDeclNode *arrDecl = dynamic_cast<cArrayDeclNode*>(currentDecl);
            bool isArrayParam = (arrDecl != nullptr && arrDecl->GetCount() == 0);
            if (arrDecl != nullptr && arrDecl->GetElementDecl())
            {
                elemSize = arrDecl->GetElementDecl()->GetDeclSize();
                if (elemSize == 0) elemSize = arrDecl->GetElementDecl()->GetSize();
                currentDecl = arrDecl->GetElementDecl();
            }
            finalElemSize = elemSize;

            // For array index, we need indirect access
            indexExpr->Visit(this);                              // push index
            EmitCode("PUSH " + std::to_string(elemSize) + "\n"); // element size
            EmitCode("TIMES\n");                                  // index * elemSize
            
            if (isArrayParam)
            {
                // Array parameter: staticOffset points to the pointer, load it first
                EmitCode("PUSHVAR " + std::to_string(staticOffset) + "\n"); // load the array pointer
                EmitCode("PLUS\n");                               // pointer + index*elemSize
            }
            else
            {
                // Local array: compute address from FP + offset
                EmitCode("PUSH " + std::to_string(staticOffset) + "\n");   // base offset so far
                EmitCode("PUSHVAR " + std::to_string(m_fpSaveOffset) + "\n"); // saved frame pointer
                EmitCode("PLUS\n");                                   // FP + offset
                EmitCode("PLUS\n");                                   // FP + offset + index*elemSize
            }

            // Reset static offset since it's now on the stack
            staticOffset = 0;
            needsIndirect = true;
            continue;
        }
    }

    if (needsIndirect)
    {
        // Address is on the stack, add any remaining static offset
        if (staticOffset > 0)
        {
            EmitCode("PUSH " + std::to_string(staticOffset) + "\n");
            EmitCode("PLUS\n");
        }
        // Use PUSHCVARIND for 1-byte elements (char)
        if (finalElemSize == 1)
            EmitCode("PUSHCVARIND\n");
        else
            EmitCode("PUSHVARIND\n");
    }
    else
    {
        // Simple offset access (struct fields only, no array indices)
        if (finalElemSize == 1)
            EmitCode("PUSHCVAR " + std::to_string(staticOffset) + "\n");
        else
            EmitCode("PUSHVAR " + std::to_string(staticOffset) + "\n");
    }
}
