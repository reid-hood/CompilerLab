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
        }

        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};

