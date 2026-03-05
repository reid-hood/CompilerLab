#pragma once
//**************************************
// cVarDeclNode.h
//
// AST node for variable declarations
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol *type, cSymbol *name) : cDeclNode()
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

        virtual bool IsVar() { return true; }

        virtual bool IsArray()
        {
            if (m_type == nullptr || m_type->GetDecl() == nullptr) return false;
            return m_type->GetDecl()->IsArray();
        }
        
        virtual cDeclNode *GetType() 
        { 
            if (m_type == nullptr) return nullptr;
            cDeclNode *decl = m_type->GetDecl();
            return decl ? decl->GetType() : nullptr;
        }

        virtual string GetName() { return (m_name != nullptr) ? m_name->GetName() : string(""); }
        cSymbol *GetNameSym() { return m_name; }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        cSymbol *m_type;
        cSymbol *m_name;
};
