#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

class cIntExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }

        virtual cDeclNode *GetType()
        {
            // If value is in range of char (0..127), return char type
            if (m_value >= 0 && m_value <= 127)
            {
                cSymbol *charSym = g_symbolTable.Find("char");
                if (charSym != nullptr)
                {
                    return charSym->GetDecl();
                }
            }
            // Otherwise return int type
            cSymbol *intSym = g_symbolTable.Find("int");
            if (intSym != nullptr)
            {
                return intSym->GetDecl();
            }
            return nullptr;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual string NodeType() { return string("int"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        int m_value;        // value of integer constant (literal)
};
