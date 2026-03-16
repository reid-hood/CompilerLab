#pragma once
//**************************************
// lex.h
//
// Defines global symbols used the the flex generated lexer 
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2015
//

//#include "cSymbolTable.h"
#include "cAstNode.h"

int yyparse();                          // parsing function
int yylex();                            // scanning function
extern char *yytext;                    // text of current token
extern int yylineno;                    // line number of input file
extern int yynerrs;                     // Num of compile errors
extern FILE *yyin;                      // FILE pointer for input
extern cAstNode *yyast_root;            // root of AST
// Built-in type symbols (created by main)
extern class cSymbol *g_char_sym;
extern class cSymbol *g_int_sym;
extern class cSymbol *g_float_sym;
extern class cSymbol *g_long_sym;
extern class cSymbol *g_double_sym;
