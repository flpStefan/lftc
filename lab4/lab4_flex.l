%{
#include "lab4_bison.tab.h"
int current_line = 1;
%}

%option noyywrap

%%
"int"       { return DATADECLARATION; }
"float"     { return DATADECLARATION; }
"string"    { return DATADECLARATION; }
"write"     { return WRITE; }
"read"      { return READ; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"main"      { return MAIN; }

"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }
","         { return ','; }
";"         { return ';'; }
"+"         { return OPERATOR; }
"-"         { return OPERATOR; }
"*"         { return OPERATOR; }
"/"         { return OPERATOR; }
"%"         { return OPERATOR; }
"<-"        { return ASSIGN; }
"=="        { return EQUAL; }
"!="        { return OPERATOR; }
"<="        { return OPERATOR; }
">="        { return OPERATOR; }
"<"         { return OPERATOR; }
">"         { return OPERATOR; }

[+-]?[0-9]+(\.[0-9]+)? { return CONSTANT; }

\"[^\"]*\" { return CONSTANT; }

[a-zA-Z][a-zA-Z_]*    { return IDENTIFIER; }


[\n]                   { current_line++; }
[ \t\r]+ ;

.   {
    printf("Error on line %d. Unrecognized character: %s\n", current_line, yytext);
}
%%