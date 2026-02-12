//**************************************
// main.cpp
//
// Main function for lang compiler
//
// Author: Phil Howard 
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <iostream>
#include <fstream>
//#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"
#include "langparse.h"

#ifndef _WIN32
extern "C" int fileno(FILE *);
#endif

// define global variables
//cSymbolTable g_SymbolTable;
long long cSymbol::nextId;

// built-in type symbols (defined here so lexer can reference them)
cSymbol *g_char_sym = nullptr;
cSymbol *g_int_sym = nullptr;
cSymbol *g_float_sym = nullptr;
cSymbol *g_long_sym = nullptr;
cSymbol *g_double_sym = nullptr;

// takes two string args: input_file, and output_file
int main(int argc, char **argv)
{
    std::cout << "Philip Howard" << std::endl;

    const char *outfile_name;
    int result = 0;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    // Setup the output. If empty, use stdout (which may be redirected)
    if (argc > 2)
    {
        outfile_name = argv[2];

        FILE *output = fopen(outfile_name, "w");
        if (output == nullptr)
        {
            std::cerr << "Unable to open output file " << outfile_name << "\n";
            exit(-1);
        }

        // redirect stdout to the output file
        int output_fd;
    #ifdef _WIN32
        output_fd = _fileno(output);
    #else
        output_fd = fileno(output);
    #endif
        if (dup2(output_fd, 1) != 1)
        {
            std::cerr << "Unable configure output stream\n";
            exit(-1);
        }
    }

    // Insert standard type symbols in the outermost scope so their IDs
    // match the expected test outputs (char, int, float, long, double)
    g_char_sym = new cSymbol("char");
    g_int_sym = new cSymbol("int");
    g_float_sym = new cSymbol("float");
    g_long_sym = new cSymbol("long");
    g_double_sym = new cSymbol("double");

    // Insert built-in type symbols into the global symbol table (outermost scope)
    g_symbolTable.Insert(g_char_sym);
    g_symbolTable.Insert(g_int_sym);
    g_symbolTable.Insert(g_float_sym);
    g_symbolTable.Insert(g_long_sym);
    g_symbolTable.Insert(g_double_sym);

    std::cerr << "MAIN: About to call yyparse()\n";
    result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            // Print XML declaration then AST
            std::cout << "<?xml version=\"1.0\"?>\n";
            std::cout << yyast_root->ToString();
        } else {
            std::cout << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    return result;
}
