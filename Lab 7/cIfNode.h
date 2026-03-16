#pragma once
//**************************************
// cIfNode.h
//
// AST node for if statements
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"
#include "cStmtsNode.h"

class cIfNode : public cStmtNode
{
    public:
        cIfNode(cExprNode *cond, cStmtsNode *ifstmts, cStmtsNode *elsestmts=nullptr) : cStmtNode()
        {
            AddChild(cond);
            AddChild(ifstmts);
            if (elsestmts) AddChild(elsestmts);
            m_cond = cond;
            m_ifstmts = ifstmts;
            m_elsestmts = elsestmts;
        }
        
        cExprNode *GetCondition() { return m_cond; }
        cStmtsNode *GetThenStmts() { return m_ifstmts; }
        cStmtsNode *GetElseStmts() { return m_elsestmts; }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
    protected:
        cExprNode *m_cond;
        cStmtsNode *m_ifstmts;
        cStmtsNode *m_elsestmts;
};
