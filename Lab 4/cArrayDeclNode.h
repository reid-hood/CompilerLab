// cArrayDeclNode.h
#pragma once

#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(int count, cSymbol *type, cSymbol *name) : cDeclNode(), m_count(count)
        {
            AddChild(type);
            AddChild(name);
        }

        virtual string NodeType() { return string("array_decl"); }
        virtual string AttributesToString() { return string(" count=\"") + std::to_string(m_count) + "\""; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_count;
};
