#pragma once
//**************************************
// cWhileNode.h
//
// Defines AST node for a while statement
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
    public:
        // params are the condition and statement
        cWhileNode(cExprNode *condition, cStmtNode *stmt)
            : cStmtNode()
        {
            AddChild(condition);
            AddChild(stmt);
            m_condition = condition;
            m_stmt = stmt;
        }
        
        cExprNode *GetCondition() { return m_condition; }
        cStmtNode *GetBody() { return m_stmt; }

        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
    protected:
        cExprNode *m_condition;
        cStmtNode *m_stmt;
};

