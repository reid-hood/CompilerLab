#pragma once
//**************************************
// cBinaryExprNode.h
//
// AST node for binary expressions: left op right
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode *left, cOpNode *op, cExprNode *right) : cExprNode()
        {
            AddChild(left);
            AddChild(op);
            AddChild(right);
            m_left = left;
            m_right = right;
        }

        virtual cDeclNode *GetType()
        {
            // Binary expressions return the type of the left operand
            if (m_left != nullptr)
            {
                return m_left->GetType();
            }
            return nullptr;
        }

        cExprNode *GetLeft() { return m_left; }
        cExprNode *GetRight() { return m_right; }
        cOpNode *GetOp() { return static_cast<cOpNode*>(GetChild(1)); }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        cExprNode *m_left;
        cExprNode *m_right;
};
