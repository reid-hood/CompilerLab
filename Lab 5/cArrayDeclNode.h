// cArrayDeclNode.h
#pragma once

#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(int count, cSymbol *type, cSymbol *name) : cDeclNode(), m_count(count)
        {
            AddChild(type);
            AddChild(name);
            m_type = type;
            m_name = name;

            // Set the declaration for this symbol
            if (name != nullptr)
            {
                name->SetDecl(this);
            }
        }

        virtual bool IsArray() { return true; }

        virtual cDeclNode *GetType() 
        { 
            if (m_type == nullptr) return nullptr;
            cDeclNode *decl = m_type->GetDecl();
            return decl ? decl->GetType() : nullptr;
        }

        virtual string GetName() { return (m_name != nullptr) ? m_name->GetName() : string(""); }
        cSymbol *GetNameSym() { return m_name; }

        virtual string NodeType() { return string("array_decl"); }
        virtual string AttributesToString() { return string(" count=\"") + std::to_string(m_count) + "\""; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_count;
        cSymbol *m_type;
        cSymbol *m_name;
};
