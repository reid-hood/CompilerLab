#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"

// Forward declaration
class cDeclNode;

class cExprNode : public cStmtNode
{
    public:
        cExprNode() : cStmtNode() {}

        // Pure virtual - all expressions must return their type
        virtual cDeclNode *GetType() = 0;
};
