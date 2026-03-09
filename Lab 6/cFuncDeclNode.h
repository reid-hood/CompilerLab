// cFuncDeclNode.h - AST node for function declarations
#pragma once

#include "cDeclNode.h"
#include "cArgsNode.h"
#include "cSymbol.h"

class cStmtsNode;

class cFuncDeclNode : public cDeclNode
{
    public:
        cFuncDeclNode(cSymbol *rettype, cSymbol *name, cArgsNode *args, bool isDefinition=false) 
            : cDeclNode(), m_rettype(rettype), m_name(name), m_args(args),
              m_isDefined(isDefinition), m_prevDecl(nullptr), m_body(nullptr), m_displaySym(name),
              m_func_size(0), m_func_offset(0)
        {
            AddChild(rettype);
            AddChild(name);
            if (args != nullptr) AddChild(args);

            if (name != nullptr)
            {
                cDeclNode *prevDecl = name->GetDecl();
                if (prevDecl != nullptr && prevDecl->IsFunc())
                {
                    m_prevDecl = prevDecl;
                    cFuncDeclNode *prevFunc = dynamic_cast<cFuncDeclNode*>(prevDecl);

                    // Return type must match exactly.
                    cDeclNode *myRetType = (m_rettype != nullptr) ? m_rettype->GetDecl() : nullptr;
                    cDeclNode *prevRetType = (prevFunc != nullptr && prevFunc->GetRetType() != nullptr)
                        ? prevFunc->GetRetType()->GetDecl() : nullptr;
                    if (myRetType != prevRetType)
                    {
                        SemanticParseError(name->GetName() + " previously defined with different return type");
                    }

                    // Parameter count must match.
                    int myCount = (m_args != nullptr) ? m_args->GetArgCount() : 0;
                    int prevCount = (prevFunc != nullptr && prevFunc->GetArgs() != nullptr)
                        ? prevFunc->GetArgs()->GetArgCount() : 0;
                    if (myCount != prevCount)
                    {
                        SemanticParseError(name->GetName() + " redeclared with a different number of parameters");
                    }
                    else if (prevFunc != nullptr && myCount > 0)
                    {
                        bool paramsMatch = true;
                        for (int i = 0; i < myCount; i++)
                        {
                            cDeclNode *myArg = (m_args != nullptr) ? m_args->GetArgDecl(i) : nullptr;
                            cDeclNode *prevArg = (prevFunc->GetArgs() != nullptr) ? prevFunc->GetArgs()->GetArgDecl(i) : nullptr;
                            cDeclNode *myArgType = (myArg != nullptr) ? myArg->GetType() : nullptr;
                            cDeclNode *prevArgType = (prevArg != nullptr) ? prevArg->GetType() : nullptr;
                            if (myArgType != prevArgType)
                            {
                                paramsMatch = false;
                                break;
                            }
                        }

                        if (!paramsMatch)
                        {
                            SemanticParseError(name->GetName() + " previously defined with different parameters");
                        }
                    }

                    // Keep previous declaration as canonical and mark it defined.
                    if (m_isDefined && prevFunc != nullptr && !prevFunc->IsDefined())
                    {
                        prevFunc->SetDefined();
                    }
                }
                else if (prevDecl == nullptr)
                {
                    // First declaration of this function.
                    name->SetDecl(this);
                }
                else
                {
                    // Existing non-function declaration in this scope is an error handled by parser.
                    // Do not overwrite that declaration.
                }
            }
        }

        virtual bool IsFunc() { return true; }

        virtual cDeclNode *GetType() 
        { 
            if (m_rettype == nullptr) return nullptr;
            cDeclNode *decl = m_rettype->GetDecl();
            return decl ? decl->GetType() : nullptr;
        }

        virtual string GetName() { return (m_name != nullptr) ? m_name->GetName() : string(""); }
        cSymbol *GetNameSym() { return m_name; }
        
        cSymbol *GetRetType() { return m_rettype; }
        cArgsNode *GetArgs() { return m_args; }
        
        void SetDefined() { m_isDefined = true; }
        bool IsDefined() { return m_isDefined; }

        void SetBody(cStmtsNode *body) { m_body = body; }
        cStmtsNode *GetBody() { return m_body; }

        void SetDisplaySym(cSymbol *sym) { if (sym != nullptr) m_displaySym = sym; }
        cSymbol *GetDisplaySym() { return m_displaySym; }
        
        cDeclNode *GetPrevDecl() { return m_prevDecl; }

        int GetFuncSize() { return m_func_size; }
        void SetFuncSize(int size) { m_func_size = size; }
        int GetFuncOffset() { return m_func_offset; }
        void SetFuncOffset(int offset) { m_func_offset = offset; }

        virtual string NodeType() { return string("func_decl"); }
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
        bool m_isDefined;
        cDeclNode *m_prevDecl;  // Previous declaration if function is redeclared
        cStmtsNode *m_body;
        cSymbol *m_displaySym;
        int m_func_size;
        int m_func_offset;
};
