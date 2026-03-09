#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"

// Forward declaration to avoid circular dependency
class cSymbol;

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        // Virtual getter functions for type checking
        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int  GetSize()  { return 0; }

        // Return the declaration name as a string.
        virtual string GetName() = 0;

        virtual bool IsCompatibleWith(cDeclNode *type)
        {
            if (type == nullptr) return false;

            // Struct assignment/argument compatibility: only same struct type.
            if (this->IsStruct() || type->IsStruct())
            {
                return this->IsStruct() && type->IsStruct() && this == type;
            }

            auto TypeRank = [](cDeclNode *decl) -> int
            {
                if (decl == nullptr) return -1;
                if (decl->IsChar()) return 0;
                if (decl->IsInt()) return (decl->GetSize() >= 8) ? 2 : 1;
                if (decl->IsFloat()) return (decl->GetSize() >= 8) ? 4 : 3;
                return -1;
            };

            int myRank = TypeRank(this);
            int otherRank = TypeRank(type);
            if (myRank < 0 || otherRank < 0) return false;

            return otherRank <= myRank;
        }
        
        // Pure virtual - must be overridden by all decl subclasses
        virtual cDeclNode *GetType() = 0;

        // Size and offset for storage allocation
        int GetDeclSize() { return m_size; }
        void SetDeclSize(int size) { m_size = size; }

        int GetDeclOffset() { return m_offset; }
        void SetDeclOffset(int offset) { m_offset = offset; }

    private:
        int m_size = 0;
        int m_offset = 0;
};
