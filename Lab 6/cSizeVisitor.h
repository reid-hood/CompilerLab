#pragma once

#include "cVisitor.h"
#include <stack>

class cDeclNode;
class cBlockNode;
class cDeclsNode;
class cStmtNode;
class cVarExprNode;
class cFuncNode;
class cFuncDeclNode;
class cArgsNode;

class cSizeVisitor : public cVisitor
{
    public:
        cSizeVisitor() : cVisitor(), m_currentOffset(0), m_maxOffsetReached(0) {}

        // Override Visit methods for size computation
        virtual void Visit(cProgramNode *node) override;
        virtual void Visit(cBlockNode *node) override;
        virtual void Visit(cDeclsNode *node) override;
        virtual void Visit(cVarDeclNode *node) override;
        virtual void Visit(cArrayDeclNode *node) override;
        virtual void Visit(cStructDeclNode *node) override;
        virtual void Visit(cVarExprNode *node) override;
        virtual void Visit(cFuncNode *node) override;
        virtual void Visit(cFuncDeclNode *node) override;
        virtual void Visit(cArgsNode *node) override;
        virtual void Visit(cParamsNode *node) override;

    protected:
        int m_currentOffset;
        int m_maxOffsetReached;  // Track high water mark across scope boundaries
        std::stack<int> m_scopeOffsetStack;

        // Helper methods
        int ComputeSize(cDeclNode *decl);
        int AlignOffset(int offset, int size);
};
