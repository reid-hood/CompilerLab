//**************************************
// cStructDeclNode.h
//
// AST node for struct declarations
//

#pragma once

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cStructDeclNode : public cDeclNode
{
    public:
        cStructDeclNode(cDeclsNode *decls, cSymbol *name) : cDeclNode()
        {
            AddChild(decls);
            AddChild(name);
            m_name = name;

            // Set the declaration for this symbol
            if (name != nullptr)
            {
                name->SetDecl(this);
            }
        }

        virtual bool IsStruct() { return true; }
        
        virtual cDeclNode *GetType() { return this; }

        virtual cSymbol *GetName() { return m_name; }

        virtual string NodeType() { return string("struct_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        cSymbol *m_name;
};
