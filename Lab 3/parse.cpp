//**************************************
// parse.cpp
//
// Recursive descent parser for Lab 3
// Following the grammar specification exactly
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
#include "lex.h"
#include "parse.h"
#include "utils.h"

//*******************************************
// PROG  -> STMTS end
bool FindPROG()
{
    if (!FindSTMTS()) 
    {
        return false;
    }

    int token = PeekToken();
    if (token != 0)  // 0 is the token for "end"
    {
        Error("end");
        return false;
    }
    AdvanceToken();  // past 'end'

    return true;
}

//*******************************************
// STMTS -> STMT STMTS
//       -> λ
bool FindSTMTS()
{
    // Try to find STMT, if it fails, this is the lambda production
    if (!FindSTMT())
    {
        // This is the lambda production - it's always valid
        return true;
    }

    // We found a statement, now look for more statements
    return FindSTMTS();
}

//*******************************************
// STMT  -> EXPR ;
//       -> identifier = EXPR ;
bool FindSTMT()
{
    int token = PeekToken();

    // Check if this is an identifier assignment
    if (token == 256)  // 256 is identifier token
    {
        AdvanceToken();  // past identifier
        
        token = PeekToken();
        if (token != '=')
        {
            // We had an identifier but no assignment, this isn't a valid STMT
            Error("=");
            // Synchronize by skipping to semicolon
            while ((token = PeekToken()) != ';' && token != 0)
            {
                AdvanceToken();
            }
            if (token == ';')
            {
                AdvanceToken();  // past ';'
                return true;
            }
            return false;
        }
        AdvanceToken();  // past '='

        if (!FindEXPR())
        {
            // Error in EXPR, synchronize
            token = PeekToken();
            while (token != ';' && token != 0)
            {
                AdvanceToken();
                token = PeekToken();
            }
            if (token == ';')
            {
                AdvanceToken();  // past ';'
                return true;
            }
            return false;
        }

        token = PeekToken();
        if (token != ';')
        {
            Error(";");
            // Synchronize
            while ((token = PeekToken()) != ';' && token != 0)
            {
                AdvanceToken();
            }
            if (token == ';')
            {
                AdvanceToken();  // past ';'
                return true;
            }
            return false;
        }
        AdvanceToken();  // past ';'
        std::cout << "Found a statement\n";
        return true;
    }

    // Try EXPR ; production
    if (!FindEXPR())
    {
        // Error in EXPR, but try to sync and recover
        int token = PeekToken();
        while (token != ';' && token != 0)
        {
            AdvanceToken();
            token = PeekToken();
        }
        if (token == ';')
        {
            AdvanceToken();  // past ';'
            return true;
        }
        return false;
    }

    token = PeekToken();
    if (token != ';')
    {
        Error(";");
        // Synchronize by finding semicolon
        while ((token = PeekToken()) != ';' && token != 0)
        {
            AdvanceToken();
        }
        if (token == ';')
        {
            AdvanceToken();  // past ';'
            return true;
        }
        return false;
    }
    AdvanceToken();  // past ';'
    std::cout << "Found a statement\n";
    return true;
}

//*******************************************
// EXPR  -> (EXPR) EXPR'
//       -> TERM
bool FindEXPR()
{
    int token = PeekToken();

    if (token == '(')
    {
        AdvanceToken();  // past '('

        if (!FindEXPR())
        {
            return false;
        }

        token = PeekToken();
        if (token != ')')
        {
            Error(")");
            return false;
        }
        AdvanceToken();  // past ')'

        return FindEXPR_P();
    }

    // Try TERM production
    return FindTERM();
}

//*******************************************
// EXPR' -> PLUSOP (EXPR) EXPR'
//       -> λ
bool FindEXPR_P()
{
    int token = PeekToken();

    // Check for PLUSOP
    if (token == '+' || token == '-')
    {
        AdvanceToken();  // past the operator

        token = PeekToken();
        if (token != '(')
        {
            Error("(");
            return false;
        }
        AdvanceToken();  // past '('

        if (!FindEXPR())
        {
            return false;
        }

        token = PeekToken();
        if (token != ')')
        {
            Error(")");
            return false;
        }
        AdvanceToken();  // past ')'

        return FindEXPR_P();
    }

    // Lambda production - always succeeds
    return true;
}

//*******************************************
// PLUSOP -> +
//        -> -
bool FindPLUSOP()
{
    int token = PeekToken();
    if (token != '+' && token != '-')
    {
        return false;
    }
    AdvanceToken();
    return true;
}

//*******************************************
// TERM  -> [EXPR] TERM'
//       -> num
//       -> { identifier }
bool FindTERM()
{
    int token = PeekToken();

    if (token == '[')
    {
        AdvanceToken();  // past '['

        if (!FindEXPR())
        {
            return false;
        }

        token = PeekToken();
        if (token != ']')
        {
            Error("]");
            return false;
        }
        AdvanceToken();  // past ']'

        return FindTERM_P();
    }

    if (token == 257)  // 257 is num token
    {
        AdvanceToken();  // past number
        return true;
    }

    if (token == '{')
    {
        AdvanceToken();  // past '{'

        token = PeekToken();
        if (token != 256)  // 256 is identifier
        {
            Error("identifier");
            return false;
        }
        AdvanceToken();  // past identifier

        token = PeekToken();
        if (token != '}')
        {
            Error("}");
            return false;
        }
        AdvanceToken();  // past '}'

        return true;
    }

    return false;
}

//*******************************************
// TERM' -> TIMESOP [EXPR] TERM'
//       -> λ
bool FindTERM_P()
{
    int token = PeekToken();

    // Check for TIMESOP
    if (token == '*' || token == '/')
    {
        AdvanceToken();  // past the operator

        token = PeekToken();
        if (token != '[')
        {
            Error("[");
            return false;
        }
        AdvanceToken();  // past '['

        if (!FindEXPR())
        {
            return false;
        }

        token = PeekToken();
        if (token != ']')
        {
            Error("]");
            return false;
        }
        AdvanceToken();  // past ']'

        return FindTERM_P();
    }

    // Lambda production - always succeeds
    return true;
}

//*******************************************
// TIMESOP -> *
//         -> /
bool FindTIMESOP()
{
    int token = PeekToken();
    if (token != '*' && token != '/')
    {
        return false;
    }
    AdvanceToken();
    return true;
}