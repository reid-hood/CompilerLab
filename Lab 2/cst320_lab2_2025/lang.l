%{
#include "lex.h"
#include "tokens.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

void Process_ID(const char *name);

#define DO_RETURN(a) { return (a); }
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

letters          [a-zA-Z_]
digits           [0-9]

%%

program                 DO_RETURN(PROGRAM);
if                      DO_RETURN(IF);
else                    DO_RETURN(ELSE);
endif                   DO_RETURN(ENDIF);
while                   DO_RETURN(WHILE);
print                   DO_RETURN(PRINT);
prints                  DO_RETURN(PRINTS);
char                    DO_RETURN(CHAR);
int                     DO_RETURN(INT);
long                    DO_RETURN(LONG);
float                   DO_RETURN(FLOAT);
double                  DO_RETURN(DOUBLE);
struct                  DO_RETURN(STRUCT);
array                   DO_RETURN(ARRAY);
return                  DO_RETURN(RETURN);

==                      DO_RETURN(EQUALS);
!=                      DO_RETURN(NOT_EQUALS);
"&&"                    DO_RETURN(AND);
"||"                    DO_RETURN(OR);
">="                    DO_RETURN(GE);
"<="                    DO_RETURN(LE);

\(                      DO_RETURN(40);
\)                      DO_RETURN(41);
\{                      { g_symbolTable.IncreaseScope(); return 123; }
\}                      { g_symbolTable.DecreaseScope(); return 125; }
\[                      DO_RETURN(91);
\]                      DO_RETURN(93);
,                       DO_RETURN(44);
;                       DO_RETURN(59);
\.                      DO_RETURN(46);
=                       DO_RETURN(61);
">"                     DO_RETURN(62);
"<"                     DO_RETURN(60);
\+                      DO_RETURN(43);
-                       DO_RETURN(45);
\*                      DO_RETURN(42);
\/                      DO_RETURN(47);
%                       DO_RETURN(37);

{letters}({letters}|{digits})*  { Process_ID(yytext); DO_RETURN(IDENTIFIER); }
{digits}+\.{digits}+            DO_RETURN(FLOAT_VAL);
{digits}+                       DO_RETURN(INT_VAL);
\"[^\"]*\"                      DO_RETURN(STRING_LIT);

[ \t\n]+                        /* skip whitespace */
"//".+                          /* skip line comments */

.                              DO_RETURN(JUNK_TOKEN);

%%

void Process_ID(const char *name)
{
    cSymbol *sym = g_symbolTable.FindLocal(name);
    
    if (sym == nullptr)
    {
        sym = new cSymbol(name);
        g_symbolTable.Insert(sym);
    }
    
    yylval.symbol = sym;
}
