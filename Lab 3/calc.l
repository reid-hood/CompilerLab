%{
//*********************
// calc.l
// Flex scanner for Lab 3
//
// Author: Phil Howard
// phil.howard@oit.edu
//

#include "lex.h"

%}

%option noyywrap
%option yylineno

digit           [0-9]
letter          [a-zA-Z_]

%%

"//".* {
    /* C++ style comments */
}

"end" {
    return 0; /* Special token for 'end' */
}

{letter}({letter}|{digit})* {
    /* identifier */
    return 256; /* identifier token */
}

{digit}+ {
    /* integer constant */
    return 257; /* num token */
}

"+" {
    return '+';
}

"-" {
    return '-';
}

"*" {
    return '*';
}

"/" {
    return '/';
}

"[" {
    return '[';
}

"]" {
    return ']';
}

"(" {
    return '(';
}

")" {
    return ')';
}

"{" {
    return '{';
}

"}" {
    return '}';
}

"=" {
    return '=';
}

";" {
    return ';';
}

[ \t\r]+ {
    /* whitespace */
}

\n {
    /* newline - yylineno is automatically incremented by %option yylineno */
}

. {
    /* unrecognized character - just return it */
    return yytext[0];
}

%%

