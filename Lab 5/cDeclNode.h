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
        
        // Pure virtual - must be overridden by all decl subclasses
        virtual cDeclNode *GetType() = 0;
};
