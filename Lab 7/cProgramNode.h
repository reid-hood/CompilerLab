#pragma once
//**************************************
// cProgramNode.h
//
// Defines AST node for a lang program
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cBlockNode.h"
#include <cstdlib>

class cProgramNode : public cAstNode
{
    public:
        // param is the block that makes up the program
        cProgramNode(cBlockNode *block) : cAstNode()
        {
            AddChild(block);
        }

        cBlockNode *GetBlock() { return static_cast<cBlockNode*>(GetChild(0)); }

        // Size of the program
        int GetProgramSize() { return m_size; }
        void SetProgramSize(int size) { m_size = size; }

        virtual string NodeType() { return string("program"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
        virtual string AttributesToString()
        {
            const char *suppress = std::getenv("LAB6_SUPPRESS_PROGRAM_SIZE");
            if (suppress != nullptr && suppress[0] == '1')
            {
                return "";
            }

            int size = GetProgramSize();
            if (size == 0) return "";
            return " size=\"" + std::to_string(size) + "\"";
        }

    private:
        int m_size = 0;
    };
