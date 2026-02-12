%{
#include "lex.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "langparse.h"
#ifndef _WIN32
extern "C" int fileno(FILE *);
#endif
#include <string>
using std::string;

extern cSymbol *g_char_sym;
extern cSymbol *g_int_sym;
extern cSymbol *g_float_sym;
extern cSymbol *g_long_sym;
extern cSymbol *g_double_sym;
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
    // Check if it's a built-in type
    if (g_char_sym && s == g_char_sym->GetName()) { yylval.symbol = g_char_sym; DO_RETURN(TYPE_ID); }
    if (g_int_sym && s == g_int_sym->GetName()) { yylval.symbol = g_int_sym; DO_RETURN(TYPE_ID); }
    if (g_float_sym && s == g_float_sym->GetName()) { yylval.symbol = g_float_sym; DO_RETURN(TYPE_ID); }
    if (g_long_sym && s == g_long_sym->GetName()) { yylval.symbol = g_long_sym; DO_RETURN(TYPE_ID); }
    if (g_double_sym && s == g_double_sym->GetName()) { yylval.symbol = g_double_sym; DO_RETURN(TYPE_ID); }
    
    // Look up in symbol table (searches all scopes). If not found,
    // Return the identifier name to the parser as a string. The parser
    // will perform symbol table lookups and create symbols as needed.
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

[{};()\[\].,+\-*/%=]     { DO_RETURN(yytext[0]); }

%%

int Return(int val)
{
    return val;
}
