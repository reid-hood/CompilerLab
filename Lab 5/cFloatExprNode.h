#pragma once
//**************************************
// cFloatExprNode.h
//
// AST node for float constants
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"
#include <sstream>
#include <iomanip>

class cFloatExprNode : public cExprNode
{
    public:
        cFloatExprNode(double value) : cExprNode()
        {
            m_value = value;
        }

        virtual cDeclNode *GetType()
        {
            // Float constants are doubles
            cSymbol *doubleSym = g_symbolTable.Find("double");
            if (doubleSym != nullptr)
            {
                return doubleSym->GetDecl();
            }
            return nullptr;
        }

        virtual string AttributesToString()
        {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(6) << m_value;
            return string(" value=\"") + ss.str() + "\"";
        }

        virtual string NodeType() { return string("float"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        double m_value;
};
