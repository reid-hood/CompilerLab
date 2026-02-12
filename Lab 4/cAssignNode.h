#pragma once
//**************************************
// cAssignNode.h
//
// Defines AST node for assignment statement (lval = expr)
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"
#include "cExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        // params are the lvalue (varref) and the expression being assigned
        cAssignNode(cExprNode *lval, cExprNode *rval) : cStmtNode()
        {
            AddChild(lval);
            AddChild(rval);
        }

        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
