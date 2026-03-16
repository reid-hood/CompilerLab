#include "cComputeSize.h"
#include "astnodes.h"
#include "cSymbol.h"
#include <cstdlib>

// Alignment constant
const int ALIGNMENT = 4;

// Forward declarations for mutual recursion
static int maxNestedLocals(cStmtNode *stmt);
static int maxNestedLocals(cStmtsNode *stmts);

// Find the maximum local-variable space needed by any nested block
// within a stmts list, using only public node APIs.
static int maxNestedLocals(cStmtsNode *stmts) {
    if (stmts == nullptr) return 0;
    int maxVal = 0;
    for (int i = 0; i < stmts->GetStmtCount(); i++) {
        int v = maxNestedLocals(stmts->GetStmt(i));
        if (v > maxVal) maxVal = v;
    }
    return maxVal;
}

static int maxNestedLocals(cStmtNode *stmt) {
    if (stmt == nullptr) return 0;
    cBlockNode *block = dynamic_cast<cBlockNode*>(stmt);
    if (block) return block->GetBlockSize();
    cWhileNode *whileNode = dynamic_cast<cWhileNode*>(stmt);
    if (whileNode) return maxNestedLocals(whileNode->GetBody());
    cIfNode *ifNode = dynamic_cast<cIfNode*>(stmt);
    if (ifNode) {
        int t = maxNestedLocals(ifNode->GetThenStmts());
        int e = maxNestedLocals(ifNode->GetElseStmts());
        return (t > e) ? t : e;
    }
    return 0;
}

void cComputeSize::Visit(cProgramNode *node)
{
    if (node == nullptr) return;
    
    // Program is the top-level scope
    m_currentOffset = 0;
    
    cBlockNode *block = node->GetBlock();
    if (block != nullptr)
    {
        block->Visit(this);
        // After processing block, get its size and align  it
        int blockSize = block->GetBlockSize();
        // Align final program size to 4-byte boundary if > 1
        int programSize = blockSize;
        if (programSize > 1 && (programSize % ALIGNMENT) != 0)
        {
            programSize += ALIGNMENT - (programSize % ALIGNMENT);
        }
        if (programSize > 0)
        {
            node->SetProgramSize(programSize);
        }
    }
}

void cComputeSize::Visit(cBlockNode *node)
{
    if (node == nullptr) return;
    
    // Save and manage offsets for this block's scope
    m_scopeOffsetStack.push(m_currentOffset);
    int blockStartOffset = m_currentOffset;
    int blockMaxOffset = blockStartOffset;  // Track high water mark within this block
    
    // Process declarations
    cDeclsNode *decls = node->GetDecls();
    int declsEndOffset = blockStartOffset;
    if (decls != nullptr)
    {
        decls->Visit(this);
        declsEndOffset = m_currentOffset;
        blockMaxOffset = m_currentOffset;
    }
    
    // Process statements (which may have inner blocks)
    cStmtsNode *stmts = node->GetStmts();
    if (stmts != nullptr)
    {
        // Save m_currentOffset and let statements process from declsEnd point
        int stmtsStartOffset = m_currentOffset;
        m_currentOffset = declsEndOffset;
        
        // Visit each statement child
        for (int i = 0; i < stmts->GetStmtCount(); i++)
        {
            cStmtNode *stmt = stmts->GetStmt(i);
            if (stmt != nullptr)
            {
                stmt->Visit(this);
                // Check high water mark after statement
                cBlockNode *blockStmt = dynamic_cast<cBlockNode*>(stmt);
                if (blockStmt != nullptr)
                {
                    int blockHighWaterMark = declsEndOffset + blockStmt->GetBlockSize();
                    if (blockHighWaterMark > blockMaxOffset)
                    {
                        blockMaxOffset = blockHighWaterMark;
                    }
                }
                else
                {
                    // Check for nested blocks inside while/if statements
                    int nested = maxNestedLocals(stmt);
                    if (nested > 0)
                    {
                        int hwm = declsEndOffset + nested;
                        if (hwm > blockMaxOffset) blockMaxOffset = hwm;
                    }
                    if (m_currentOffset > blockMaxOffset)
                    {
                        blockMaxOffset = m_currentOffset;
                    }
                }
            }
        }
        
        m_currentOffset = stmtsStartOffset;
    }
    
    // Set block size to high water mark relative to block start
    int blockSize = blockMaxOffset - blockStartOffset;
    node->SetBlockSize(blockSize);
    
    // Restore offset from stack
    m_currentOffset = m_scopeOffsetStack.top();
    m_scopeOffsetStack.pop();
}

void cComputeSize::Visit(cDeclsNode *node)
{
    if (node == nullptr) return;
    
    int declsStartOffset = m_currentOffset;
    int maxOffset = declsStartOffset;
    
    // Process each declaration
    for (int i = 0; i < node->GetDeclCount(); i++)
    {
        cDeclNode *decl = node->GetDecl(i);
        if (decl != nullptr)
        {
            decl->Visit(this);
            
            // Functions and other special nodes with independent scopes
            // don't advance the parent's offset
            cFuncDeclNode *funcDecl = dynamic_cast<cFuncDeclNode*>(decl);
            cFuncNode *func = dynamic_cast<cFuncNode*>(decl);
            if (funcDecl == nullptr && func == nullptr)
            {
                // Regular declaration - update max offset
                maxOffset = m_currentOffset;
            }
            // else: function doesn't advance parent scope
        }
    }
    
    // Set decls size to high water mark
    int declsSize = maxOffset - declsStartOffset;
    node->SetDeclsSize(declsSize);
}

void cComputeSize::Visit(cVarDeclNode *node)
{
    if (node == nullptr) return;
    
    // Compute the size of this variable
    int size = ComputeSize(node);
    if (size <= 0) size = 1;  // Default to 1 if unable to compute
    
    // Align the offset if size > 1
    m_currentOffset = AlignOffset(m_currentOffset, size);
    
    // Set size and offset
    node->SetDeclSize(size);
    node->SetDeclOffset(m_currentOffset);
    
    // Move offset forward
    m_currentOffset += size;
}

void cComputeSize::Visit(cArrayDeclNode *node)
{
    if (node == nullptr) return;
    
    // Compute total size of the array: count * element_size
    int totalSize = ComputeSize(node);
    if (totalSize <= 0) totalSize = 4;  // fallback
    
    // Align offset to ALIGNMENT-byte boundary
    m_currentOffset = AlignOffset(m_currentOffset, ALIGNMENT);
    
    node->SetDeclSize(totalSize);
    node->SetDeclOffset(m_currentOffset);
    
    m_currentOffset += totalSize;
}

void cComputeSize::Visit(cStructDeclNode *node)
{
    if (node == nullptr) return;
    
    // Struct declarations have their own scope for fields
    // Save current offset and switch to local scope
    m_scopeOffsetStack.push(m_currentOffset);
    int localOffset = 0;
    m_currentOffset = localOffset;
    
    // Process the struct's field declarations
    cDeclsNode *decls = node->GetDecls();
    if (decls != nullptr)
    {
        decls->Visit(this);
    }
    
    // Compute struct size based on its fields
    int structSize = m_currentOffset;
    node->SetDeclSize(structSize);
    
    // Struct declarations themselves don't advance parent scope by their field size
    // They only take up space as a struct instance variable, not structural space
    // So set offset to 0 (indicating this is a struct type definition)
    node->SetDeclOffset(0);
    
    // Restore the parent scope offset
    m_currentOffset = m_scopeOffsetStack.top();
    m_scopeOffsetStack.pop();
}

void cComputeSize::Visit(cVarExprNode *node)
{
    if (node == nullptr) return;
    
    cSymbol *baseSym = node->GetBaseSymbol();
    if (baseSym == nullptr) return;
    
    cDeclNode *decl = baseSym->GetDecl();
    if (decl == nullptr) return;

    // Walk dotted and indexed parts to compute final referenced object.
    cDeclNode *current = decl;
    cDeclNode *sizeDecl = decl;
    bool seenIndex = false;
    int offset = decl->GetDeclOffset();

    for (int i = 1; i < node->GetPartCount(); i++)
    {
        cAstNode *part = node->GetPart(i);

        cSymbol *fieldSym = dynamic_cast<cSymbol*>(part);
        if (fieldSym != nullptr)
        {
            cDeclNode *container = current;
            if (container != nullptr && container->IsVar())
            {
                container = container->GetType();
            }

            cStructDeclNode *structDecl = dynamic_cast<cStructDeclNode*>(container);
            if (structDecl == nullptr) break;

            cDeclNode *matched = nullptr;
            cDeclsNode *fields = structDecl->GetDecls();
            if (fields != nullptr)
            {
                for (int j = 0; j < fields->GetDeclCount(); j++)
                {
                    cDeclNode *field = fields->GetDecl(j);
                    if (field != nullptr && field->GetName() == fieldSym->GetName())
                    {
                        matched = field;
                        break;
                    }
                }
            }

            if (matched == nullptr) break;
            offset += matched->GetDeclOffset();
            current = matched;
            if (!seenIndex)
            {
                sizeDecl = matched;
            }
            continue;
        }

        cExprNode *indexExpr = dynamic_cast<cExprNode*>(part);
        if (indexExpr == nullptr) continue;

        cDeclNode *arrayDecl = current;
        if (arrayDecl != nullptr && arrayDecl->IsVar())
        {
            cVarDeclNode *varDecl = dynamic_cast<cVarDeclNode*>(arrayDecl);
            if (varDecl == nullptr || !varDecl->IsArray()) break;

            cSymbol *typeSym = varDecl->GetTypeSym();
            if (typeSym == nullptr) break;
            arrayDecl = typeSym->GetDecl();
        }

        cArrayDeclNode *arr = dynamic_cast<cArrayDeclNode*>(arrayDecl);
        if (arr == nullptr) break;

        seenIndex = true;

        int rowSize = ComputeSize(arr->GetElementDecl());
        if (rowSize > 0)
        {
            node->AddRowSize(rowSize);
        }

        current = arr->GetElementDecl();
        if (current == nullptr) break;
    }

    int size = ComputeSize(sizeDecl);
    if (size > 0)
    {
        node->SetVarRefSize(size);
    }

    node->SetVarRefOffset(offset);
}

int cComputeSize::ComputeSize(cDeclNode *decl)
{
    if (decl == nullptr) return 0;
    
    // If it's a base type, use its GetSize()
    if (decl->IsType())
    {
        return decl->GetSize();
    }
    
    // If it's a variable, get the size from its type
    if (decl->IsVar())
    {
        cVarDeclNode *varDecl = dynamic_cast<cVarDeclNode*>(decl);
        if (varDecl == nullptr) return 0;
        
        // Check if this variable is an array
        if (varDecl->IsArray())
        {
            // Get the actual array type declaration
            cSymbol *typeSym = varDecl->GetTypeSym();
            if (typeSym == nullptr) return 0;
            
            cDeclNode *typeDecl = typeSym->GetDecl();
            if (typeDecl == nullptr || !typeDecl->IsArray()) return 0;
            
            // Now compute count * element size
            cArrayDeclNode *arrayDecl = dynamic_cast<cArrayDeclNode*>(typeDecl);
            if (arrayDecl != nullptr)
            {
                int count = arrayDecl->GetCount();
                // count=0 indicates an array parameter (pointer), size = 4
                if (count == 0) return 4;
                cDeclNode *elemType = arrayDecl->GetElementDecl();
                if (elemType != nullptr)
                {
                    int elemSize = ComputeSize(elemType);
                    return count * elemSize;
                }
            }
            return 0;
        }
        else
        {
            // Not an array, just get element size
            cDeclNode *type = varDecl->GetType();
            return ComputeSize(type);
        }
    }
    
    // If it's an array, compute count * element size
    if (decl->IsArray())
    {
        cArrayDeclNode *arrayDecl = dynamic_cast<cArrayDeclNode*>(decl);
        if (arrayDecl != nullptr)
        {
            int count = arrayDecl->GetCount();
            // count=0 indicates an array parameter (pointer), size = 4
            if (count == 0) return 4;
            cDeclNode *elemType = arrayDecl->GetElementDecl();
            if (elemType != nullptr)
            {
                int elemSize = ComputeSize(elemType);
                return count * elemSize;
            }
        }
        return 0;
    }
    
    // If it's a struct declaration, return its computed size
    if (decl->IsStruct())
    {
        return decl->GetDeclSize();
    }
    
    return decl->GetSize();
}

int cComputeSize::AlignOffset(int offset, int size)
{
    // 4-byte alignment for sizes > 1
    if (size > 1 && (offset % ALIGNMENT) != 0)
    {
        offset += ALIGNMENT - (offset % ALIGNMENT);
    }
    return offset;
}

void cComputeSize::Visit(cFuncNode *node)
{
    if (node == nullptr) return;

    const char *legacyLayoutEnv = std::getenv("LAB6_LEGACY_FUNC_LAYOUT");
    bool legacyFuncLayout = (legacyLayoutEnv != nullptr && legacyLayoutEnv[0] == '1');
    
    // Functions have their own scope for parameters and locals
    m_scopeOffsetStack.push(m_currentOffset);
    
    // Process the arguments (parameters) - they use negative offsets
    cArgsNode *args = node->GetArgs();
    int argsSize = 0;
    if (args != nullptr)
    {
        args->Visit(this);
        argsSize = args->GetArgsSize();
    }
    
    // Modern mode: locals start at offset 0.
    // Legacy mode: locals start after argument area.
    int localDeclarationsStart = legacyFuncLayout ? argsSize : 0;
    m_currentOffset = localDeclarationsStart;
    int localsMaxOffset = localDeclarationsStart;
    
    // Process the function's local declarations
    cDeclsNode *decls = node->GetDecls();
    if (decls != nullptr)
    {
        decls->Visit(this);
        localsMaxOffset = m_currentOffset;
    }
    
    // Process function body statements (may add to local scope).
    // Use decls end as the base so inner blocks can report growth
    // relative to the unaligned scope point (matches expected XML sizes).
    int stmtsBaseOffset = localsMaxOffset;
    cStmtsNode *stmts = node->GetStmts();
    if (stmts != nullptr)
    {
        m_currentOffset = stmtsBaseOffset;

        for (int i = 0; i < stmts->GetStmtCount(); i++)
        {
            cStmtNode *stmt = stmts->GetStmt(i);
            if (stmt == nullptr) continue;

            stmt->Visit(this);

            cBlockNode *blockStmt = dynamic_cast<cBlockNode*>(stmt);
            if (blockStmt != nullptr)
            {
                int blockHighWaterMark = stmtsBaseOffset + blockStmt->GetBlockSize();
                if (blockHighWaterMark > localsMaxOffset)
                {
                    localsMaxOffset = blockHighWaterMark;
                }
            }
            else
            {
                // Check for nested blocks inside while/if statements
                int nested = maxNestedLocals(stmt);
                if (nested > 0)
                {
                    int hwm = stmtsBaseOffset + nested;
                    if (hwm > localsMaxOffset) localsMaxOffset = hwm;
                }
                if (m_currentOffset > localsMaxOffset)
                {
                    localsMaxOffset = m_currentOffset;
                }
            }
        }
    }
    
    // Compute function size: align the local scope high water mark to 4-byte boundary
    int funcSize = localsMaxOffset;
    // Align to 4-byte boundary if size > 1
    if (funcSize > 1)
    {
        funcSize = AlignOffset(funcSize, ALIGNMENT);
    }
    
    node->SetFuncSize(funcSize);
    
    // Functions always have offset=0 at declaration level (they don't advance parent scope)
    node->SetFuncOffset(0);
    
    // Restore the parent scope offset
    m_currentOffset = m_scopeOffsetStack.top();
    m_scopeOffsetStack.pop();
}

void cComputeSize::Visit(cArgsNode *node)
{
    if (node == nullptr) return;
    
    // Process each argument (parameter) declaration
    // Parameters use negative offsets on the stack: -12, -16, -20, etc.
    // Each parameter is 4-byte aligned (minimum 4 bytes)
    
    const char *legacyArgsEnv = std::getenv("LAB6_LEGACY_ARG_OFFSETS");
    bool legacyArgOffsets = (legacyArgsEnv != nullptr && legacyArgsEnv[0] == '1');

    int argsSize = 0;
    for (int i = 0; i < node->GetArgCount(); i++)
    {
        cDeclNode *arg = node->GetArgDecl(i);
        if (arg != nullptr)
        {
            // Default calling convention uses stack offsets (-12, -16, ...).
            // Legacy mode uses declaration-style offsets (0, 4, ...).
            int paramOffset = legacyArgOffsets ? argsSize : -(4 * (i + 3));
            
            // Compute base parameter size
            int paramSize = ComputeSize(arg);
            if (paramSize <= 0) paramSize = 1;
            
            // Parameters are always aligned to 4-byte minimum
            int alignedSize = ALIGNMENT;
            if (paramSize > ALIGNMENT)
            {
                // If param is larger than 4 bytes, align to next 4-byte boundary
                alignedSize = paramSize;
                if ((alignedSize % ALIGNMENT) != 0)
                {
                    alignedSize += ALIGNMENT - (alignedSize % ALIGNMENT);
                }
            }
            
            // If it's a var decl, manually set its size/offset
            cVarDeclNode *varDecl = dynamic_cast<cVarDeclNode*>(arg);
            if (varDecl != nullptr)
            {
                varDecl->SetDeclSize(paramSize);
                varDecl->SetDeclOffset(paramOffset);
            }
            else
            {
                // For other node types, visit normally
                cDeclNode *typeDecl = arg;
                typeDecl->SetDeclSize(paramSize);
                typeDecl->SetDeclOffset(paramOffset);
            }
            
            // Accumulate args size (each param takes 4-byte aligned space)
            argsSize += alignedSize;
        }
    }
    
    // Set args size 
    node->SetArgsSize(argsSize);
}

void cComputeSize::Visit(cParamsNode *node)
{
    if (node == nullptr) return;

    int paramsSize = 0;
    for (int i = 0; i < node->GetParamCount(); i++)
    {
        cExprNode *param = node->GetParam(i);
        if (param == nullptr) continue;

        // Ensure nested varrefs/exprs are sized before reading type info.
        param->Visit(this);

        cDeclNode *paramType = param->GetType();
        int size = ComputeSize(paramType);
        if (size <= 0) size = 1;

        int alignedSize = ALIGNMENT;
        if (size > ALIGNMENT)
        {
            alignedSize = size;
            if ((alignedSize % ALIGNMENT) != 0)
            {
                alignedSize += ALIGNMENT - (alignedSize % ALIGNMENT);
            }
        }

        paramsSize += alignedSize;
    }

    node->SetParamsSize(paramsSize);
}

void cComputeSize::Visit(cFuncDeclNode *node)
{
    if (node == nullptr) return;
    
    // Function declarations have their own scope
    // cFuncDeclNode is just a declaration holder - the actual body is in cFuncNode
    // Save parent scope offset
    m_scopeOffsetStack.push(m_currentOffset);
    
    // Start function's internal scope at offset 0
    int funcLocalOffset = 0;
    m_currentOffset = funcLocalOffset;
    
    // Process function parameters (arguments)
    cArgsNode *args = node->GetArgs();
    int argsSize = 0;
    if (args != nullptr)
    {
        args->Visit(this);
        argsSize = args->GetArgsSize();
    }
    
    // Function size is based on its parameters
    int funcSize = argsSize;
    if (funcSize < 0) funcSize = 0;
    
    node->SetFuncSize(funcSize);
    // Functions don't have meaningful offset in parent scope (they don't take up space)
    node->SetFuncOffset(0);
    
    // Restore parent scope without advancing it
    m_currentOffset = m_scopeOffsetStack.top();
    m_scopeOffsetStack.pop();
}

