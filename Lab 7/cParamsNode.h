// cParamsNode.h - wrapper for function call parameter lists
#pragma once

#include "cAstNode.h"
#include "cExprNode.h"
#include <cstdlib>

class cParamsNode : public cAstNode
{
    public:
        cParamsNode(cExprNode *first)
            : m_size(0)
        {
            AddChild(first);
        }

        void Insert(cExprNode *expr)
        {
            AddChild(expr);
        }

        int GetParamCount() { return NumChildren(); }
        cExprNode *GetParam(int index) { return static_cast<cExprNode*>(GetChild(index)); }

        int GetParamsSize() { return m_size; }
        void SetParamsSize(int size) { m_size = size; }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString()
        {
            const char *suppress = std::getenv("LAB6_SUPPRESS_PARAMS_SIZE");
            if (suppress != nullptr && suppress[0] == '1') return "";

            if (m_size == 0) return "";
            return " size=\"" + std::to_string(m_size) + "\"";
        }

    private:
        int m_size;
};
