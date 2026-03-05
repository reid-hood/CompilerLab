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
        }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
