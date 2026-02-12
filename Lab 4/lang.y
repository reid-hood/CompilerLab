%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"

%}

%locations
%debug

 /* union defines the type for lexical values */
%union{
    int             int_val;
    std::string*    str_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    double          float_val;
    cFloatExprNode* float_node;
    cBinaryExprNode* binary_node;
    cSymbol*        symbol;
    }

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;
    cSymbol *g_current_func_sym = nullptr;
    cSymbol *g_current_ret_sym = nullptr;
%}

%start  program

%token <str_val>   IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   EQUALS
%token <int_val>   NOT_EQUALS
%token <str_val>   STRING_LIT

%token  PROGRAM
%token  PRINT PRINTS
%token  WHILE IF ELSE ENDIF
%token  STRUCT ARRAY
%token  RETURN
%token  JUNK_TOKEN

%type <program_node> program
%type <block_node> block
%type <ast_node> open
%type <ast_node> close
%type <ast_node> decls
%type <ast_node> decl
%type <ast_node> var_decl
%type <ast_node> struct_decl
%type <ast_node> array_decl
%type <ast_node> func_decl
%type <ast_node> func_prefix
%type <ast_node> func_call
%type <ast_node> paramsspec
%type <ast_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <expr_node> lval
%type <ast_node> params
%type <ast_node> param
%type <expr_node> expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <expr_node> varref
%type <symbol> varpart

%%

program: PROGRAM block
                                { $$ = new cProgramNode($2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close
                                { $$ = new cBlockNode(static_cast<cDeclsNode*>($2), $3); }
    |   open stmts close
                                { $$ = new cBlockNode(nullptr, $2); }

open:   '{'
                                { g_symbolTable.IncreaseScope(); }

close:  '}'
                                { g_symbolTable.DecreaseScope(); }

decls:      decls decl
                                { static_cast<cDeclsNode*>($1)->Insert(static_cast<cDeclNode*>($2)); $$ = $1; }
        |   decl
                                { $$ = new cDeclsNode(static_cast<cDeclNode*>($1)); }
decl:       var_decl ';'
                                { $$ = $1; }
        |   array_decl ';'
                            {  }
        |   struct_decl ';'
                            {  }
        |   func_decl
                            {  }
        |   error ';'
                            {  }

var_decl:   TYPE_ID IDENTIFIER
                                    { 
                                        // Declaration with a TYPE_ID token (built-in or previously
                                        // created type symbol). $1 is a cSymbol* for the type.
                                        cSymbol *typesym = $1;
                                        cSymbol *declsym = new cSymbol(*$2);
                                        g_symbolTable.Insert(declsym);
                                        $$ = new cVarDeclNode(typesym, declsym);
                                        delete $2;
                                    }
        |   IDENTIFIER IDENTIFIER
                                    {
                                        // Declaration where the type is given by an identifier
                                        // (e.g., a previously declared struct type).
                                        cSymbol *typesym = g_symbolTable.Find(*$1);
                                        if (typesym == nullptr) typesym = new cSymbol(*$1);
                                        cSymbol *declsym = new cSymbol(*$2);
                                        g_symbolTable.Insert(declsym);
                                        $$ = new cVarDeclNode(typesym, declsym);
                                        delete $1;
                                        delete $2;
                                    }
struct_decl:  STRUCT open decls close IDENTIFIER
                                { 
                                    // Create a symbol for the struct type name and insert
                                    cSymbol *typesym = new cSymbol(*$5);
                                    g_symbolTable.Insert(typesym);
                                    $$ = new cStructDeclNode(static_cast<cDeclsNode*>($3), typesym);
                                    delete $5;
                                }
array_decl:   ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                { 
                                    cSymbol *typesym = $2;
                                    cSymbol *namesym = new cSymbol(*$6);
                                    $$ = new cArrayDeclNode($4, typesym, namesym);
                                    g_symbolTable.Insert(namesym);
                                    delete $6;
                                }
        |   ARRAY IDENTIFIER '[' INT_VAL ']' IDENTIFIER
                                {
                                    // array of a user-defined type
                                    cSymbol *typesym = g_symbolTable.Find(*$2);
                                    if (typesym == nullptr) typesym = new cSymbol(*$2);
                                    cSymbol *namesym = new cSymbol(*$6);
                                    $$ = new cArrayDeclNode($4, typesym, namesym);
                                    g_symbolTable.Insert(namesym);
                                    delete $2;
                                    delete $6;
                                }

// Function prototypes and definitions
func_decl:  func_prefix ')' ';'
                                {
                                    // prototype with no args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    $$ = new cFuncNode(rettype, funcsym, nullptr, nullptr, nullptr);
                                    g_current_func_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix paramsspec ')' ';'
                                {
                                    // prototype with args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>($2));
                                    $$ = new cFuncNode(rettype, funcsym, args, nullptr, nullptr);
                                    g_current_func_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix ')' open decls stmts close
                                {
                                    // definition with no args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    $$ = new cFuncNode(rettype, funcsym, nullptr, static_cast<cDeclsNode*>($4), $5);
                                    g_current_func_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix paramsspec ')' open decls stmts close
                                {
                                    // definition with args and decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>($2));
                                    $$ = new cFuncNode(rettype, funcsym, args, static_cast<cDeclsNode*>($5), $6);
                                    g_current_func_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix paramsspec ')' open stmts close
                                {
                                    // definition with args and no decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>($2));
                                    $$ = new cFuncNode(rettype, funcsym, args, nullptr, static_cast<cStmtsNode*>($5));
                                    g_current_func_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_symbolTable.DecreaseScope();
                                }
func_prefix: TYPE_ID IDENTIFIER '('
                                {
                                    // Create a function symbol in the current (enclosing)
                                    // scope, then open a new scope for parameters.
                                    g_current_ret_sym = $1;
                                    g_current_func_sym = new cSymbol(*$2);
                                    g_symbolTable.Insert(g_current_func_sym);
                                    g_symbolTable.IncreaseScope();
                                    $$ = nullptr;
                                    delete $2;
                                }
paramsspec:  paramsspec ',' paramspec
                                { static_cast<cDeclsNode*>($1)->Insert(static_cast<cDeclNode*>($3)); $$ = $1; }
        |   paramspec
                            { $$ = new cDeclsNode(static_cast<cDeclNode*>($1)); }

paramspec:  var_decl
                                    { $$ = $1; }

stmts:      stmts stmt
                                { $1->Insert($2); $$ = $1; }
        |   stmt
                            { $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, $7); }
        |   WHILE '(' expr ')' stmt
                                { $$ = new cWhileNode($3, static_cast<cStmtNode*>($5)); }
        |   PRINT '(' expr ')' ';'
                                { $$ = new cPrintNode($3); }
        |   PRINTS '(' STRING_LIT ')' ';'
                                { $$ = new cPrintsNode($3); }
        |   lval '=' expr ';'
                                { $$ = new cAssignNode(static_cast<cExprNode*>($1), $3); }
        |   func_call ';'
                            {  }
        |   block
                            {  }
         |   RETURN expr ';'
                                     { $$ = new cReturnNode($2); }
        |   error ';'
                            {}

func_call:  IDENTIFIER '(' params ')'
                                    { 
                                        cSymbol *fs = g_symbolTable.Find(*$1);
                                        cParamsNode *p = static_cast<cParamsNode*>($3);
                                        $$ = new cFuncCallNode(fs, p);
                                        delete $1;
                                    }
        |   IDENTIFIER '(' ')'
                            { 
                                        cSymbol *fs = g_symbolTable.Find(*$1);
                                        $$ = new cFuncCallNode(fs, nullptr);
                                        delete $1;
                            }

varref:   varref '.' varpart
                                { static_cast<cVarExprNode*>($1)->AddPart($3); $$ = $1; }
        | varref '[' expr ']'
                            { static_cast<cVarExprNode*>($1)->AddPart(static_cast<cExprNode*>($3)); $$ = $1; }
        | varpart
                            { $$ = new cVarExprNode($1); }

varpart:  IDENTIFIER
                                { 
                                    // Resolve identifier to symbol in current scopes
                                    cSymbol *found = g_symbolTable.Find(*$1);
                                    if (found != nullptr) {
                                        $$ = found;
                                    }
                                    else {
                                        $$ = new cSymbol(*$1);
                                    }
                                    delete $1;
                                }

lval:     varref
                                { $$ = $1; }

params:   params ',' param
                                { static_cast<cParamsNode*>($1)->Insert(static_cast<cExprNode*>($3)); $$ = $1; }
        |   param
                            { $$ = new cParamsNode(static_cast<cExprNode*>($1)); }

param:      expr
                                { $$ = $1; }

expr:       expr EQUALS addit
                                { $$ = new cBinaryExprNode($1, new cOpNode(EQUALS), $3); }
        |   addit
                            { $$ = $1; }

addit:      addit '+' term
                                { $$ = new cBinaryExprNode($1, new cOpNode('+'), $3); }
        |   addit '-' term
                            { $$ = new cBinaryExprNode($1, new cOpNode('-'), $3); }
        |   term
                            { $$ = $1; }

term:       term '*' fact
                                { $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); }
        |   term '/' fact
                            { $$ = new cBinaryExprNode($1, new cOpNode('/'), $3); }
        |   term '%' fact
                            { $$ = new cBinaryExprNode($1, new cOpNode('%'), $3); }
        |   fact
                            { $$ = $1; }

fact:       '(' expr ')'
                                { $$ = $2; }
        |   INT_VAL
                            { $$ = new cIntExprNode($1); }
        |   FLOAT_VAL
                            { $$ = new cFloatExprNode($1); }
        |   varref
                            { $$ = $1; }
        |   func_call
                            {  }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
