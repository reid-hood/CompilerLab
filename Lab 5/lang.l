%{
#include "lex.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "cDeclNode.h"
#include "langparse.h"
#ifndef _WIN32
extern "C" int fileno(FILE *);
#endif
#include <string>
using std::string;

extern cSymbolTable g_symbolTable;

#define DO_RETURN(a) { return (a); }

int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

%%

program                     { DO_RETURN(PROGRAM); }
print                       { DO_RETURN(PRINT); }
prints                      { DO_RETURN(PRINTS); }

if                          { DO_RETURN(IF); }
else                        { DO_RETURN(ELSE); }
endif                       { DO_RETURN(ENDIF); }
while                       { DO_RETURN(WHILE); }
struct                      { DO_RETURN(STRUCT); }
array                       { DO_RETURN(ARRAY); }
return                      { DO_RETURN(RETURN); }
[0-9]+\.[0-9]+             { yylval.float_val = atof(yytext); DO_RETURN(FLOAT_VAL); }
[0-9]+                      { yylval.int_val = atoi(yytext); DO_RETURN(INT_VAL); }

[A-Za-z_][A-Za-z0-9_]*      {
    std::string s(yytext);
    
    // Look up in symbol table (searches all scopes). 
    cSymbol *sym = g_symbolTable.Find(s);
    if (sym != nullptr && sym->GetDecl() != nullptr && sym->GetDecl()->IsType()) {
        // It's a type identifier
        yylval.symbol = sym;
        DO_RETURN(TYPE_ID);
    }
    
    // Not a type, return as regular identifier
    yylval.str_val = new std::string(s);
    DO_RETURN(IDENTIFIER);
}

"//".*                   { /* skip line comment */ }
\"[^\"]*\"              { yylval.str_val = new std::string(yytext + 1, yyleng - 2); DO_RETURN(STRING_LIT); }
"=="                     { yylval.int_val = '='; DO_RETURN(EQUALS); }
"!="                     { yylval.int_val = '!'; DO_RETURN(NOT_EQUALS); }
"<="                     { yylval.int_val = '<'; DO_RETURN(LE); }
">="                     { yylval.int_val = '>'; DO_RETURN(GE); }
"&&"                     { yylval.int_val = '&'; DO_RETURN(AND); }
"||"                     { yylval.int_val = '|'; DO_RETURN(OR); }
[ \t\r\n]+               { /* skip whitespace */ }

[{};()\[\].,+\-*/%=<>]   { DO_RETURN(yytext[0]); }

%%

int Return(int val)
{
    return val;
}
