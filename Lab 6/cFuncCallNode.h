// cFuncCallNode.h - AST node for function calls
#pragma once

#include "cExprNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"

class cFuncCallNode : public cExprNode
{
    public:
        cFuncCallNode(cSymbol *funcsym, cParamsNode *params)
        {
            AddChild(funcsym);
            if (params != nullptr) AddChild(params);

            // Semantic check: is the function defined?
            if (funcsym != nullptr && funcsym->GetDecl() == nullptr)
            {
                SemanticParseError("Symbol " + funcsym->GetName() + " not defined");
            }
        }

        cSymbol *GetFuncSymbol() { return static_cast<cSymbol*>(GetChild(0)); }
        cParamsNode *GetParams()
        {
            if (NumChildren() > 1) return static_cast<cParamsNode*>(GetChild(1));
            return nullptr;
        }

        virtual cDeclNode *GetType()
        {
            cSymbol *funcSym = GetFuncSymbol();
            if (funcSym == nullptr) return nullptr;

            cDeclNode *decl = funcSym->GetDecl();
            if (decl == nullptr) return nullptr;

            return decl->GetType();
        }

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
