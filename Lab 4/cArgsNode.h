// cArgsNode.h - wrapper for function argument lists
#pragma once

#include "cAstNode.h"
#include "cDeclNode.h"

class cArgsNode : public cAstNode
{
    public:
        cArgsNode(cDeclsNode *decls)
        {
            AddAllChildren(decls);
            delete decls;
        }

        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
