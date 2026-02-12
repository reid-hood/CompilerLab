#pragma once
//**************************************
// cVarExprNode.h
//
// AST node for variable references
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol *sym) : cExprNode()
        {
            AddChild(sym);
        }

        // Add another part (for field access)
        void AddPart(cSymbol *sym)
        {
            AddChild(sym);
        }

        // Add an index or other expression part
        void AddPart(cExprNode *expr)
        {
            AddChild(expr);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
