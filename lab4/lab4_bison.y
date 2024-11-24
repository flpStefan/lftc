%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex();
    extern int yyparse();
    extern FILE* yyin;
    extern int current_line;
    void yyerror(const char *s);
%}


%token DATADECLARATION
%token WRITE
%token READ
%token IF
%token ELSE
%token WHILE
%token MAIN
%token OPERATOR
%token ASSIGN
%token EQUAL
%token CONSTANT
%token IDENTIFIER

%%

program_start : MAIN '(' ')' instruction_block {}
              ;

instruction_block : '{' instructions '}' {}
                  ;

instructions : declaration ';' instructions
             | simple_calculation ';' instructions
             | if_instruction instructions
             | while_instruction instructions
             | read_instruction ';' instructions
             | write_instruction ';' instructions
             | |
             ;

declaration : DATADECLARATION IDENTIFIER 
            | DATADECLARATION IDENTIFIER ASSIGN CONSTANT 
            | DATADECLARATION IDENTIFIER ASSIGN simple_calculation
            ;

simple_calculation : term OPERATOR simple_calculation
                   | term
                   ;

if_instruction : IF '(' simple_calculation ')' instruction_block ELSE instruction_block {}
         | IF '(' simple_calculation ')' instruction_block {}
         ;

while_instruction : WHILE '(' simple_calculation ')' instruction_block {}
            ;

read_instruction : READ '(' IDENTIFIER ')'
                 ;

write_instruction : WRITE '(' IDENTIFIER ')'
                  ;

term : IDENTIFIER | CONSTANT

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", current_line, s);
    exit(1);
}

int yywrap(void){
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) 
        yyin = fopen(argv[1], "r"); 
    else 
        yyin = stdin; 

    while(!feof(yyin)){
        yyparse();
    }

    printf("\nDone\n");
    return 0;
}