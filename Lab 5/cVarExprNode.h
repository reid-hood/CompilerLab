#pragma once
//**************************************
// cVarExprNode.h
//
// AST node for variable references
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"

class cExprNode;

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol *sym) : cExprNode(), m_baseSym(sym)
        {
            AddChild(sym);

            // Semantic check: is the variable defined?
            if (sym != nullptr && sym->GetDecl() == nullptr)
            {
                SemanticParseError("Symbol " + sym->GetName() + " not defined");
            }
        }

        // Add another part (for field access)
        void AddPart(cSymbol *sym)
        {
            AddChild(sym);
        }

        // Add an index or other expression part
        void AddPart(cExprNode *expr)
        {
            AddChild(expr);
        }

        virtual cDeclNode *GetType()
        {
            // Get type from the base symbol
            if (m_baseSym != nullptr)
            {
                cDeclNode *decl = m_baseSym->GetDecl();
                if (decl != nullptr)
                {
                    return decl->GetType();
                }
            }
            return nullptr;
        }

        cSymbol *GetBaseSymbol() { return m_baseSym; }
        int GetPartCount() { return NumChildren(); }
        cAstNode *GetPart(int index) { return GetChild(index); }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        cSymbol *m_baseSym;
};
