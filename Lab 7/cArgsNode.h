// cArgsNode.h - wrapper for function argument lists
#pragma once

#include "cAstNode.h"
#include "cDeclNode.h"

class cArgsNode : public cAstNode
{
    public:
        cArgsNode(cDeclsNode *decls) : m_size(0)
        {
            AddAllChildren(decls);
            delete decls;
        }

        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        int GetArgCount()
        {
            return NumChildren();
        }

        cDeclNode *GetArgDecl(int index)
        {
            return static_cast<cDeclNode*>(GetChild(index));
        }

        int GetArgsSize() { return m_size; }
        void SetArgsSize(int size) { m_size = size; }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
        virtual string AttributesToString()
        {
            if (m_size != 0)
            {
                return string(" size=\"") + std::to_string(m_size) + "\"";
            }
            return string("");
        }

    private:
        int m_size;
};
