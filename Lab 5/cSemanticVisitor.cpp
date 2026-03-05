#include "cSemanticVisitor.h"

#include "astnodes.h"
#include "cFuncDeclNode.h"
#include "cSymbolTable.h"
#include "langparse.h"

static int TypeRank(cDeclNode *type)
{
    if (type == nullptr) return -1;
    if (type->IsChar()) return 0;
    if (type->IsInt()) return (type->GetSize() >= 8) ? 2 : 1;
    if (type->IsFloat()) return (type->GetSize() >= 8) ? 4 : 3;
    return -1;
}

static string TypeName(cDeclNode *type)
{
    if (type == nullptr) return "unknown";
    if (type->IsChar()) return "char";
    if (type->IsInt()) return (type->GetSize() >= 8) ? "long" : "int";
    if (type->IsFloat()) return (type->GetSize() >= 8) ? "double" : "float";
    if (type->IsFunc()) return "function";
    if (type->IsArray()) return "array";
    if (type->IsStruct()) return "struct";
    return "unknown";
}

cDeclNode *cSemanticVisitor::GetIntType()
{
    cSymbol *intSym = g_symbolTable.Find("int");
    if (intSym == nullptr) return nullptr;
    return intSym->GetDecl();
}

cDeclNode *cSemanticVisitor::Promote(cDeclNode *left, cDeclNode *right)
{
    if (left == nullptr) return right;
    if (right == nullptr) return left;

    return (TypeRank(left) >= TypeRank(right)) ? left : right;
}

cDeclNode *cSemanticVisitor::InferVarType(cVarExprNode *node)
{
    cSymbol *base = node->GetBaseSymbol();
    if (base == nullptr) return nullptr;

    cDeclNode *current = base->GetDecl();
    if (current == nullptr) return nullptr;

    for (int i = 1; i < node->GetPartCount(); i++)
    {
        cAstNode *part = node->GetPart(i);
        cExprNode *indexExpr = dynamic_cast<cExprNode*>(part);
        if (indexExpr != nullptr)
        {
            if (current->IsArray())
                current = current->GetType();
            continue;
        }

        cSymbol *fieldSym = dynamic_cast<cSymbol*>(part);
        if (fieldSym != nullptr && current->IsStruct())
        {
            cStructDeclNode *structDecl = dynamic_cast<cStructDeclNode*>(current);
            cDeclsNode *fields = (structDecl != nullptr) ? structDecl->GetDecls() : nullptr;
            cDeclNode *matched = nullptr;

            if (fields != nullptr)
            {
                for (int j = 0; j < fields->GetDeclCount(); j++)
                {
                    cDeclNode *field = fields->GetDecl(j);
                    if (field == nullptr) continue;

                    cVarDeclNode *varField = dynamic_cast<cVarDeclNode*>(field);
                    if (varField != nullptr && varField->GetName() == fieldSym->GetName())
                    {
                        matched = varField;
                        break;
                    }

                    cArrayDeclNode *arrField = dynamic_cast<cArrayDeclNode*>(field);
                    if (arrField != nullptr && arrField->GetName() == fieldSym->GetName())
                    {
                        matched = arrField;
                        break;
                    }
                }
            }

            current = matched;
        }
    }

    if (current == nullptr) return nullptr;
    if (current->IsFunc()) return current->GetType();
    return current->GetType();
}

cDeclNode *cSemanticVisitor::InferExprType(cAstNode *node)
{
    if (node == nullptr) return nullptr;

    if (dynamic_cast<cIntExprNode*>(node) != nullptr)
        return static_cast<cIntExprNode*>(node)->GetType();

    if (dynamic_cast<cFloatExprNode*>(node) != nullptr)
        return static_cast<cFloatExprNode*>(node)->GetType();

    if (dynamic_cast<cVarExprNode*>(node) != nullptr)
        return InferVarType(static_cast<cVarExprNode*>(node));

    if (dynamic_cast<cBinaryExprNode*>(node) != nullptr)
    {
        cBinaryExprNode *expr = static_cast<cBinaryExprNode*>(node);
        cDeclNode *leftType = InferExprType(expr->GetLeft());
        cDeclNode *rightType = InferExprType(expr->GetRight());
        cOpNode *opNode = expr->GetOp();
        int op = (opNode != nullptr) ? opNode->GetOp() : 0;

        if (op == '>' || op == '<' || op == GE || op == LE ||
            op == EQUALS || op == NOT_EQUALS || op == OR || op == AND)
        {
            return GetIntType();
        }

        return Promote(leftType, rightType);
    }

    if (dynamic_cast<cFuncCallNode*>(node) != nullptr)
    {
        cFuncCallNode *call = static_cast<cFuncCallNode*>(node);
        cSymbol *funcSym = call->GetFuncSymbol();
        if (funcSym == nullptr || funcSym->GetDecl() == nullptr) return nullptr;
        return funcSym->GetDecl()->GetType();
    }

    cExprNode *expr = dynamic_cast<cExprNode*>(node);
    if (expr != nullptr) return expr->GetType();

    return nullptr;
}

void cSemanticVisitor::Visit(cAssignNode *node)
{
    node->VisitAllChildren(this);

    cDeclNode *leftType = InferExprType(node->GetLval());
    cDeclNode *rightType = InferExprType(node->GetRval());
    if (leftType == nullptr || rightType == nullptr) return;

    if (!leftType->IsCompatibleWith(rightType))
    {
        node->SemanticError("Cannot assign " + TypeName(rightType) +
            " to " + TypeName(leftType));
    }
}

void cSemanticVisitor::Visit(cVarExprNode *node)
{
    node->VisitAllChildren(this);

    cSymbol *base = node->GetBaseSymbol();
    if (base == nullptr) return;

    cDeclNode *current = base->GetDecl();
    if (current == nullptr) return;

    if (current->IsFunc())
    {
        node->SemanticError("Symbol " + base->GetName() + " is a function, not a variable");
        return;
    }

    string arrayName = base->GetName();
    for (int i = 1; i < node->GetPartCount(); i++)
    {
        cAstNode *part = node->GetPart(i);
        cExprNode *indexExpr = dynamic_cast<cExprNode*>(part);
        if (indexExpr == nullptr) continue; // Ignore struct-style access in this lab.

        if (!current->IsArray())
        {
            node->SemanticError(arrayName + " is not an array");
            break;
        }

        cDeclNode *indexType = InferExprType(indexExpr);
        if (indexType == nullptr || !indexType->IsInt())
        {
            node->SemanticError("Index of " + arrayName + " is not an int");
        }

        current = current->GetType();
        if (current == nullptr) break;
    }
}

void cSemanticVisitor::Visit(cFuncCallNode *node)
{
    node->VisitAllChildren(this);

    cSymbol *funcSym = node->GetFuncSymbol();
    if (funcSym == nullptr) return;

    cDeclNode *decl = funcSym->GetDecl();
    if (decl == nullptr || !decl->IsFunc()) return;

    cFuncDeclNode *funcDecl = dynamic_cast<cFuncDeclNode*>(decl);
    if (funcDecl == nullptr) return;

    if (!funcDecl->IsDefined())
    {
        node->SemanticError("Function " + funcSym->GetName() + " not fully defined");
    }

    cParamsNode *params = node->GetParams();
    int actualCount = (params != nullptr) ? params->GetParamCount() : 0;
    int formalCount = (funcDecl->GetArgs() != nullptr) ? funcDecl->GetArgs()->GetArgCount() : 0;

    if (actualCount != formalCount)
    {
        node->SemanticError(funcSym->GetName() + " called with wrong number of arguments");
        return;
    }

    if (params == nullptr || funcDecl->GetArgs() == nullptr) return;

    for (int i = 0; i < formalCount; i++)
    {
        cDeclNode *formalDecl = funcDecl->GetArgs()->GetArgDecl(i);
        cDeclNode *formalType = (formalDecl != nullptr) ? formalDecl->GetType() : nullptr;
        cDeclNode *actualType = InferExprType(params->GetParam(i));

        if (formalType == nullptr || actualType == nullptr) continue;

        if (!formalType->IsCompatibleWith(actualType))
        {
            node->SemanticError("function " + funcSym->GetName() + " called with incompatible argument");
            return;
        }
    }
}
