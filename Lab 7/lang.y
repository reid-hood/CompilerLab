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

// Global flag to track semantic errors
static bool g_semanticErrorHappened = false;

// Macros for error handling
#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }

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
    cSymbol *g_current_func_display_sym = nullptr;
    cSymbol *g_current_ret_sym = nullptr;
    int g_current_func_line = 0;
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
%type <expr_node> logic_and
%type <expr_node> equality
%type <expr_node> relational
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> unary
%type <expr_node> fact
%type <expr_node> varref
%type <symbol> varpart

%%

program: PROGRAM block
                                { 
                                    $$ = new cProgramNode($2);
                                    yyast_root = $$;
                                    YYACCEPT;
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
                            { $$ = $1; }
        |   struct_decl ';'
                            { $$ = $1; }
        |   func_decl
                            { $$ = $1; }
        |   error ';'
                            {  }

var_decl:   TYPE_ID IDENTIFIER
                                    { 
                                        // Declaration with a TYPE_ID token (built-in or previously
                                        // created type symbol). $1 is a cSymbol* for the type.
                                        cSymbol *typesym = $1;
                                        
                                        // Check for duplicate declaration
                                        if (g_symbolTable.FindLocal(*$2) != nullptr)
                                        {
                                            SemanticParseError("Symbol " + *$2 + " already defined in current scope");
                                        }
                                        
                                        cSymbol *declsym = new cSymbol(*$2);
                                        g_symbolTable.Insert(declsym);
                                        $$ = new cVarDeclNode(typesym, declsym);
                                        delete $2;
                                        CHECK_ERROR();
                                    }
        |   IDENTIFIER IDENTIFIER
                                    {
                                        // Declaration where the type is given by an identifier
                                        // (e.g., a previously declared struct type).
                                        cSymbol *typesym = g_symbolTable.Find(*$1);
                                        if (typesym == nullptr) typesym = new cSymbol(*$1);
                                        
                                        // Check for duplicate declaration
                                        if (g_symbolTable.FindLocal(*$2) != nullptr)
                                        {
                                            SemanticParseError("Symbol " + *$2 + " already defined in current scope");
                                        }
                                        
                                        cSymbol *declsym = new cSymbol(*$2);
                                        g_symbolTable.Insert(declsym);
                                        $$ = new cVarDeclNode(typesym, declsym);
                                        delete $1;
                                        delete $2;
                                        CHECK_ERROR();
                                    }
struct_decl:  STRUCT open decls close IDENTIFIER
                                { 
                                    // Check for duplicate declaration
                                    if (g_symbolTable.FindLocal(*$5) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + *$5 + " already defined in current scope");
                                    }
                                    
                                    // Create a symbol for the struct type name and insert
                                    cSymbol *typesym = new cSymbol(*$5);
                                    g_symbolTable.Insert(typesym);
                                    $$ = new cStructDeclNode(static_cast<cDeclsNode*>($3), typesym);
                                    delete $5;
                                    CHECK_ERROR();
                                }
array_decl:   ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                { 
                                    cSymbol *typesym = $2;
                                    
                                    // Check for duplicate declaration
                                    if (g_symbolTable.FindLocal(*$6) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + *$6 + " already defined in current scope");
                                    }
                                    
                                    cSymbol *namesym = new cSymbol(*$6);
                                    $$ = new cArrayDeclNode($4, typesym, namesym);
                                    g_symbolTable.Insert(namesym);
                                    delete $6;
                                    CHECK_ERROR();
                                }
        |   ARRAY IDENTIFIER '[' INT_VAL ']' IDENTIFIER
                                {
                                    // array of a user-defined type
                                    cSymbol *typesym = g_symbolTable.Find(*$2);
                                    if (typesym == nullptr) typesym = new cSymbol(*$2);
                                    
                                    // Check for duplicate declaration
                                    if (g_symbolTable.FindLocal(*$6) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + *$6 + " already defined in current scope");
                                    }
                                    
                                    cSymbol *namesym = new cSymbol(*$6);
                                    $$ = new cArrayDeclNode($4, typesym, namesym);
                                    g_symbolTable.Insert(namesym);
                                    delete $2;
                                    delete $6;
                                    CHECK_ERROR();
                                }

// Function prototypes and definitions
func_decl:  func_prefix ')' ';'
                                {
                                    // prototype with no args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cStmtsNode *knownBody = nullptr;
                                    cFuncDeclNode *knownDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (knownDecl != nullptr && knownDecl->IsDefined()) knownBody = knownDecl->GetBody();
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    new cFuncDeclNode(rettype, funcsym, nullptr);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    $$ = new cFuncNode(rettype, displaySym, nullptr, nullptr, knownBody);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix paramsspec ')' ';'
                                {
                                    // prototype with args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>($2));
                                    cStmtsNode *knownBody = nullptr;
                                    cFuncDeclNode *knownDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (knownDecl != nullptr && knownDecl->IsDefined()) knownBody = knownDecl->GetBody();
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    new cFuncDeclNode(rettype, funcsym, args);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    $$ = new cFuncNode(rettype, displaySym, args, nullptr, knownBody);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix ')' open stmts close
                                {
                                    // definition with no args and no decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, nullptr, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>($4);
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    $$ = new cFuncNode(rettype, displaySym, nullptr, nullptr, body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix ')' open decls stmts close
                                {
                                    // definition with no args
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, nullptr, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>($5);
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    $$ = new cFuncNode(rettype, displaySym, nullptr, static_cast<cDeclsNode*>($4), body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix paramsspec ')' open decls stmts close
                                {
                                    // definition with args and decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>($2));
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, args, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>($6);
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    $$ = new cFuncNode(rettype, displaySym, args, static_cast<cDeclsNode*>($5), body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
        |   func_prefix paramsspec ')' open stmts close
                                {
                                    // definition with args and no decls
                                    cSymbol *funcsym = g_current_func_sym;
                                    cSymbol *displaySym = g_current_func_display_sym;
                                    cSymbol *rettype = g_current_ret_sym;
                                    cArgsNode *args = new cArgsNode(static_cast<cDeclsNode*>($2));
                                    cFuncDeclNode *prevDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    bool alreadyDefined = (prevDecl != nullptr && prevDecl->IsDefined());
                                    int savedLine = yylineno;
                                    yylineno = g_current_func_line;
                                    cFuncDeclNode *funcDecl = new cFuncDeclNode(rettype, funcsym, args, true);
                                    yylineno = savedLine;
                                    cFuncDeclNode *canonDecl = dynamic_cast<cFuncDeclNode*>(funcsym != nullptr ? funcsym->GetDecl() : nullptr);
                                    if (canonDecl != nullptr) canonDecl->SetDisplaySym(displaySym);
                                    cStmtsNode *body = static_cast<cStmtsNode*>($5);
                                    if (prevDecl != nullptr) prevDecl->SetBody(body);
                                    else funcDecl->SetBody(body);
                                    if (alreadyDefined)
                                    {
                                        SemanticParseError(funcsym->GetName() + " already has a definition");
                                    }
                                    $$ = new cFuncNode(rettype, displaySym, args, nullptr, body);
                                    g_current_func_sym = nullptr;
                                    g_current_func_display_sym = nullptr;
                                    g_current_ret_sym = nullptr;
                                    g_current_func_line = 0;
                                    g_symbolTable.DecreaseScope();
                                }
func_prefix: TYPE_ID IDENTIFIER '('
                                {
                                    // Create a function symbol in the current (enclosing)
                                    // scope, then open a new scope for parameters.
                                    std::string funcName(*$2);
                                    cSymbol *outer = g_symbolTable.Find(funcName);
                                    
                                    // Check if function name already exists in current scope
                                    cSymbol *existing = g_symbolTable.FindLocal(funcName);
                                    
                                    if (existing != nullptr && existing->GetDecl() != nullptr)
                                    {
                                        // Symbol exists - check if it's a function
                                        if (existing->GetDecl()->IsFunc())
                                        {
                                            // It's already a function, reuse it
                                            g_current_func_sym = existing;
                                            cFuncDeclNode *decl = dynamic_cast<cFuncDeclNode*>(existing->GetDecl());
                                            g_current_func_display_sym = (decl != nullptr) ? decl->GetDisplaySym() : existing;
                                        }
                                        else
                                        {
                                            // It's not a function, error
                                            SemanticParseError(funcName + " previously defined as other than a function");
                                            g_current_func_sym = existing;  // Continue parsing
                                            g_current_func_display_sym = existing;
                                        }
                                    }
                                    else if (existing != nullptr)
                                    {
                                        // Symbol exists but no decl yet, treat as new
                                        g_current_func_sym = existing;
                                        g_current_func_display_sym = existing;
                                    }
                                    else
                                    {
                                        // New function, create symbol
                                        g_current_func_sym = new cSymbol(funcName);
                                        g_symbolTable.Insert(g_current_func_sym);

                                        // Match historical XML behavior for nested shadowing where
                                        // an outer non-function symbol has the same name.
                                        if (outer != nullptr && outer->GetDecl() != nullptr && !outer->GetDecl()->IsFunc())
                                            g_current_func_display_sym = outer;
                                        else
                                            g_current_func_display_sym = g_current_func_sym;
                                    }
                                    
                                    g_current_ret_sym = $1;
                                    g_current_func_line = yylineno;
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
        |   ARRAY TYPE_ID IDENTIFIER
                                    {
                                        // Array parameter without dimension (pass by reference)
                                        cSymbol *typesym = $2;
                                        
                                        // Check for duplicate declaration
                                        if (g_symbolTable.FindLocal(*$3) != nullptr)
                                        {
                                            SemanticParseError("Symbol " + *$3 + " already defined in current scope");
                                        }
                                        
                                        cSymbol *namesym = new cSymbol(*$3);
                                        // count=0 indicates an array reference/pointer parameter
                                        $$ = new cArrayDeclNode(0, typesym, namesym);
                                        g_symbolTable.Insert(namesym);
                                        delete $3;
                                        CHECK_ERROR();
                                    }
        |   ARRAY IDENTIFIER IDENTIFIER
                                    {
                                        // Array parameter of user-defined element type
                                        cSymbol *typesym = g_symbolTable.Find(*$2);
                                        if (typesym == nullptr) typesym = new cSymbol(*$2);
                                        
                                        // Check for duplicate declaration
                                        if (g_symbolTable.FindLocal(*$3) != nullptr)
                                        {
                                            SemanticParseError("Symbol " + *$3 + " already defined in current scope");
                                        }
                                        
                                        cSymbol *namesym = new cSymbol(*$3);
                                        // count=0 indicates an array reference/pointer parameter
                                        $$ = new cArrayDeclNode(0, typesym, namesym);
                                        g_symbolTable.Insert(namesym);
                                        delete $2;
                                        delete $3;
                                        CHECK_ERROR();
                                    }

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
                            { $$ = static_cast<cStmtNode*>($1); }
        |   block
                            { $$ = static_cast<cStmtNode*>($1); }
         |   RETURN expr ';'
                                     { $$ = new cReturnNode($2); }
        |   error ';'
                            {}

func_call:  IDENTIFIER '(' params ')'
                                    { 
                                        cSymbol *fs = g_symbolTable.Find(*$1);
                                        if (fs == nullptr) fs = new cSymbol(*$1);
                                        cParamsNode *p = static_cast<cParamsNode*>($3);
                                        $$ = new cFuncCallNode(fs, p);
                                        delete $1;
                                    }
        |   IDENTIFIER '(' ')'
                            { 
                                        cSymbol *fs = g_symbolTable.Find(*$1);
                                        if (fs == nullptr) fs = new cSymbol(*$1);
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

expr:       expr OR logic_and
                                { $$ = new cBinaryExprNode($1, new cOpNode(OR), $3); }
        |   logic_and
                            { $$ = $1; }

logic_and:  logic_and AND equality
                                { $$ = new cBinaryExprNode($1, new cOpNode(AND), $3); }
        |   equality
                            { $$ = $1; }

equality:   equality EQUALS relational
                                { $$ = new cBinaryExprNode($1, new cOpNode(EQUALS), $3); }
        |   equality NOT_EQUALS relational
                                { $$ = new cBinaryExprNode($1, new cOpNode(NOT_EQUALS), $3); }
        |   relational
                            { $$ = $1; }

relational: relational '>' addit
                                { $$ = new cBinaryExprNode($1, new cOpNode('>'), $3); }
        |   relational '<' addit
                                { $$ = new cBinaryExprNode($1, new cOpNode('<'), $3); }
        |   relational GE addit
                                { $$ = new cBinaryExprNode($1, new cOpNode(GE), $3); }
        |   relational LE addit
                                { $$ = new cBinaryExprNode($1, new cOpNode(LE), $3); }
        |   addit
                            { $$ = $1; }

addit:      addit '+' term
                                { $$ = new cBinaryExprNode($1, new cOpNode('+'), $3); }
        |   addit '-' term
                            { $$ = new cBinaryExprNode($1, new cOpNode('-'), $3); }
        |   term
                            { $$ = $1; }

term:       term '*' unary
                { $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); }
    |   term '/' unary
                { $$ = new cBinaryExprNode($1, new cOpNode('/'), $3); }
    |   term '%' unary
                { $$ = new cBinaryExprNode($1, new cOpNode('%'), $3); }
    |   unary
                            { $$ = $1; }

unary:      '-' unary
                { $$ = new cBinaryExprNode(new cIntExprNode(0), new cOpNode('-'), $2); }
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
                            { $$ = static_cast<cExprNode*>($1); }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
// Function that gets called when a semantic error happens
void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " near line " 
              << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}