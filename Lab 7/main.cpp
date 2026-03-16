//**************************************
// main.cpp
//
// Main function for lang compiler
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"
#include "cComputeSize.h"
#include "cCodeGen.h"
#include "cBaseTypeNode.h"

// define global variables
long long cSymbol::nextId;

// takes two string args: input_file, and output_file
int main(int argc, char **argv)
{
    std::cout << "Reid Hood" << std::endl;

    std::string outfile_name;
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

    if (argc > 2)
    {
        outfile_name = argv[2];
    } else {
        outfile_name = "langout";
    }

    cSymbol *charSym = new cSymbol("char");
    charSym->SetDecl(new cBaseTypeNode("char", 1, false));
    g_symbolTable.Insert(charSym);

    cSymbol *intSym = new cSymbol("int");
    intSym->SetDecl(new cBaseTypeNode("int", 4, false));
    g_symbolTable.Insert(intSym);

    cSymbol *floatSym = new cSymbol("float");
    floatSym->SetDecl(new cBaseTypeNode("float", 4, true));
    g_symbolTable.Insert(floatSym);

    result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            // NOTE: we should run the semantic error checker here,
            // but not everyone got it to work, so we'll skip it.
            // If yours works, feel free to include it
            //
            // cSemantic semantics;
            // semantics.VisitAllNodes(yyast_root);
            // result += semantics.NumErrors();
            //
            if (result == 0)
            {
                cComputeSize sizer;
                sizer.VisitAllNodes(yyast_root);

                // need to make the coder go out of scope before assembling
                {
                    cCodeGen coder(outfile_name + ".sl");
                    coder.VisitAllNodes(yyast_root);
                }

                string cmd = "slasm " + outfile_name + ".sl io320.sl";
                system(cmd.c_str());
            }
        } 

        if (result != 0)
        {
            std::cerr << yynerrs << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cerr << "Junk at end of program\n";
    }

    return result;
}
