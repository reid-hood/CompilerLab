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
            : m_rettype(rettype), m_name(name), m_args(args), m_decls(decls), m_stmts(stmts),
              m_func_size(0), m_func_offset(0)
        {
            AddChild(rettype);
            AddChild(name);
            if (args != nullptr) AddChild(args);
            if (decls != nullptr) AddChild(decls);
            if (stmts != nullptr) AddChild(stmts);
        }

        cSymbol *GetNameSymbol() { return m_name; }
        cSymbol *GetReturnTypeSymbol() { return m_rettype; }
        cArgsNode *GetArgs() { return m_args; }
        cDeclsNode *GetDecls() { return m_decls; }
        cStmtsNode *GetStmts() { return m_stmts; }

        int GetFuncSize() { return m_func_size; }
        void SetFuncSize(int size) { m_func_size = size; }
        int GetFuncOffset() { return m_func_offset; }
        void SetFuncOffset(int offset) { m_func_offset = offset; }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
        virtual string AttributesToString()
        {
            string result;
            if (m_func_size != 0)
            {
                result += " size=\"" + std::to_string(m_func_size) + "\"";
                result += " offset=\"" + std::to_string(m_func_offset) + "\"";
            }
            return result;
        }

    protected:
        cSymbol *m_rettype;
        cSymbol *m_name;
        cArgsNode *m_args;
        cDeclsNode *m_decls;
        cStmtsNode *m_stmts;
        int m_func_size;
        int m_func_offset;
};
