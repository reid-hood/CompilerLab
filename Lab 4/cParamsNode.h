// cParamsNode.h - wrapper for function call parameter lists
#pragma once

#include "cAstNode.h"
#include "cExprNode.h"

class cParamsNode : public cAstNode
{
    public:
        cParamsNode(cExprNode *first)
        {
            AddChild(first);
        }

        void Insert(cExprNode *expr)
        {
            AddChild(expr);
        }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
