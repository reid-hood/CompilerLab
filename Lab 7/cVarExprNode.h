#pragma once
//**************************************
// cVarExprNode.h
//
// AST node for variable references
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cArrayDeclNode.h"
#include "cStructDeclNode.h"
#include "cVarDeclNode.h"
#include <vector>

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
            if (m_baseSym == nullptr) return nullptr;

            cDeclNode *current = m_baseSym->GetDecl();
            if (current == nullptr) return nullptr;

            // Walk dotted and indexed parts to compute final type.
            for (int i = 1; i < GetPartCount(); i++)
            {
                cAstNode *part = GetPart(i);

                // Field access: var.field
                cSymbol *fieldSym = dynamic_cast<cSymbol*>(part);
                if (fieldSym != nullptr)
                {
                    cDeclNode *container = current;
                    if (container->IsVar())
                    {
                        container = container->GetType();
                    }

                    cStructDeclNode *structDecl = dynamic_cast<cStructDeclNode*>(container);
                    if (structDecl == nullptr) return nullptr;

                    cDeclsNode *fields = structDecl->GetDecls();
                    if (fields == nullptr) return nullptr;

                    cDeclNode *matchedField = nullptr;
                    for (int j = 0; j < fields->GetDeclCount(); j++)
                    {
                        cDeclNode *field = fields->GetDecl(j);
                        if (field != nullptr && field->GetName() == fieldSym->GetName())
                        {
                            matchedField = field;
                            break;
                        }
                    }

                    if (matchedField == nullptr) return nullptr;
                    current = matchedField;
                    continue;
                }

                // Indexing: var[idx]
                cExprNode *indexExpr = dynamic_cast<cExprNode*>(part);
                if (indexExpr != nullptr)
                {
                    cDeclNode *arrayDecl = current;

                    if (arrayDecl->IsVar())
                    {
                        cVarDeclNode *varDecl = dynamic_cast<cVarDeclNode*>(arrayDecl);
                        if (varDecl == nullptr || !varDecl->IsArray()) return nullptr;

                        cSymbol *typeSym = varDecl->GetTypeSym();
                        if (typeSym == nullptr) return nullptr;

                        arrayDecl = typeSym->GetDecl();
                    }

                    cArrayDeclNode *arr = dynamic_cast<cArrayDeclNode*>(arrayDecl);
                    if (arr == nullptr) return nullptr;

                    current = arr->GetElementDecl();
                    if (current == nullptr) return nullptr;
                    continue;
                }

                return nullptr;
            }

            return current->GetType();
        }

        cSymbol *GetBaseSymbol() { return m_baseSym; }
        int GetPartCount() { return NumChildren(); }
        cAstNode *GetPart(int index) { return GetChild(index); }
        void SetPart(int index, cAstNode *part) { SetChild(index, part); }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
        virtual string AttributesToString()
        {
            int size = GetVarRefSize();
            int offset = GetVarRefOffset();
            int rowCount = GetRowSizeCount();
            
            if (size == 0 && offset == 0 && rowCount == 0) return "";
            
            string result;
            if (size != 0)
            {
                result += " size=\"" + std::to_string(size) + "\"";
                // For varrefs, emit offset whenever size is present, including offset 0.
                result += " offset=\"" + std::to_string(offset) + "\"";
            }
            else if (offset != 0)
            {
                result += " offset=\"" + std::to_string(offset) + "\"";
            }
            
            if (rowCount > 0)
            {
                result += " rowsizes=\"";
                for (int i = 0; i < rowCount; i++)
                {
                    if (i > 0) result += " ";
                    result += std::to_string(GetRowSize(i));
                }
                result += "\"";
            }
            
            return result;
        }

        // Size and offset for variable references (especially for arrays)
        int GetVarRefSize() { return m_size; }
        void SetVarRefSize(int size) { m_size = size; }

        int GetVarRefOffset() { return m_offset; }
        void SetVarRefOffset(int offset) { m_offset = offset; }

        // Row sizes for multi-dimensional arrays
        void AddRowSize(int size) { m_rowsizes.push_back(size); }
        int GetRowSizeCount() { return (int)m_rowsizes.size(); }
        int GetRowSize(int index) { return (index >= 0 && index < (int)m_rowsizes.size()) ? m_rowsizes[index] : 0; }

    protected:
        cSymbol *m_baseSym;

    private:
        int m_size = 0;
        int m_offset = 0;
        std::vector<int> m_rowsizes;
};
