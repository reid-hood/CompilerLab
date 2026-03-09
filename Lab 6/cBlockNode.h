#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode()
        {
            AddChild(decls);
            AddChild(statements);
        }

        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        cDeclsNode *GetDecls()
        {
            return static_cast<cDeclsNode*>(GetChild(0));
        }
        cStmtsNode *GetStmts()
        {
            return static_cast<cStmtsNode*>(GetChild(1));
        }

        // Size of the block scope
        int GetBlockSize() { return m_size; }
        void SetBlockSize(int size) { m_size = size; }

        virtual string AttributesToString()
        {
            int size = GetBlockSize();
            if (size == 0) return "";
            return " size=\"" + std::to_string(size) + "\"";
        }

    private:
        int m_size = 0;
    };
