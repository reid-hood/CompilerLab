#pragma once

#include "cVisitor.h"
#include <stack>
#include <map>
#include <string>

class cAstNode;
class cProgramNode;
class cBlockNode;
class cDeclsNode;
class cVarDeclNode;
class cArrayDeclNode;
class cStmtNode;
class cStmtsNode;
class cExprNode;
class cIntExprNode;
class cBinaryExprNode;
class cVarExprNode;
class cIfNode;
class cWhileNode;
class cAssignNode;
class cReturnNode;
class cPrintNode;
class cPrintsNode;
class cFuncNode;
class cFuncDeclNode;
class cFuncCallNode;
class cSymbol;

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(const std::string& filename);
        virtual ~cCodeGen();

        // Main program entry point
        virtual void Visit(cProgramNode *node) override;
        
        // Blocks and declarations
        virtual void Visit(cBlockNode *node) override;
        virtual void Visit(cDeclsNode *node) override;
        virtual void Visit(cVarDeclNode *node) override;
        virtual void Visit(cArrayDeclNode *node) override;
        virtual void Visit(cFuncDeclNode *node) override;
        virtual void Visit(cFuncNode *node) override;

        // Statements
        virtual void Visit(cStmtsNode *node) override;
        virtual void Visit(cStmtNode *node) override;
        virtual void Visit(cIfNode *node) override;
        virtual void Visit(cWhileNode *node) override;
        virtual void Visit(cAssignNode *node) override;
        virtual void Visit(cReturnNode *node) override;
        virtual void Visit(cPrintNode *node) override;
        virtual void Visit(cPrintsNode *node) override;
        virtual void Visit(cFuncCallNode *node) override;

        // Expressions
        virtual void Visit(cExprNode *node) override;
        virtual void Visit(cIntExprNode *node) override;
        virtual void Visit(cBinaryExprNode *node) override;
        virtual void Visit(cVarExprNode *node) override;

    private:
        std::string m_filename;
        std::map<std::string, int> m_labelCounter;
        int m_fpSaveOffset = 0;  // offset of the saved-FP slot in the current frame

        // Helper methods
        void EmitCode(const std::string& code);
        std::string GenerateLabel();
};
