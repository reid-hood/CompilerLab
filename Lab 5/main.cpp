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
long long cSymbol::nextId = 0;

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

    // Create built-in type symbols before parsing so TYPE_ID tokenization
    // and symbol IDs match Lab 4 behavior.
    cSymbol *charSym = new cSymbol("char");
    charSym->SetDecl(new cBaseTypeNode("char", 1, false));
    g_symbolTable.Insert(charSym);

    cSymbol *intSym = new cSymbol("int");
    intSym->SetDecl(new cBaseTypeNode("int", 4, false));
    g_symbolTable.Insert(intSym);

    cSymbol *floatSym = new cSymbol("float");
    floatSym->SetDecl(new cBaseTypeNode("float", 4, true));
    g_symbolTable.Insert(floatSym);

    cSymbol *longSym = new cSymbol("long");
    longSym->SetDecl(new cBaseTypeNode("long", 8, false));
    g_symbolTable.Insert(longSym);

    cSymbol *doubleSym = new cSymbol("double");
    doubleSym->SetDecl(new cBaseTypeNode("double", 8, true));
    g_symbolTable.Insert(doubleSym);

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
            std::cout << yynerrs << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    return result;
}
