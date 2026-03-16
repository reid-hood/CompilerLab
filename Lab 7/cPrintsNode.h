#pragma once
//**************************************
// cPrintsNode.h
//
// Defines AST node for prints (string printing) statement
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"
#include <string>

using std::string;

class cPrintsNode : public cStmtNode
{
    public:
        // param is the string literal to print
        cPrintsNode(string *str) : cStmtNode(), m_string(str)
        {
        }

        string GetString() { return m_string ? *m_string : ""; }

        virtual string NodeType() { return string("prints"); }
        virtual string AttributesToString() 
        { 
            if (m_string) {
                return string(" value=\"") + *m_string + "\"";
            }
            return string("");
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    private:
        string *m_string;
};
