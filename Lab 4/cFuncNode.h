// cFuncNode.h - AST node for function declarations
#pragma once

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtsNode.h"
#include "cSymbol.h"
#include "cArgsNode.h"

class cFuncNode : public cAstNode
{
    public:
        cFuncNode(cSymbol *rettype, cSymbol *name, cArgsNode *args,
                  cDeclsNode *decls, cStmtsNode *stmts)
        {
            AddChild(rettype);
            AddChild(name);
            if (args != nullptr) AddChild(args);
            if (decls != nullptr) AddChild(decls);
            if (stmts != nullptr) AddChild(stmts);
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
