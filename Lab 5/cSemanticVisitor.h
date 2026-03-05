#pragma once

#include "cVisitor.h"

class cAstNode;
class cDeclNode;
class cExprNode;
class cVarExprNode;
class cAssignNode;
class cBinaryExprNode;
class cFuncCallNode;

class cSemanticVisitor : public cVisitor
{
    public:
        cSemanticVisitor() : cVisitor() {}

        virtual void Visit(cAssignNode *node) override;
        virtual void Visit(cVarExprNode *node) override;
        virtual void Visit(cFuncCallNode *node) override;

    protected:
        cDeclNode *InferExprType(cAstNode *node);
        cDeclNode *InferVarType(cVarExprNode *node);
        cDeclNode *Promote(cDeclNode *left, cDeclNode *right);
        cDeclNode *GetIntType();
};
