// cFuncCallNode.h - AST node for function calls
#pragma once

#include "cAstNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"

class cFuncCallNode : public cAstNode
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

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
