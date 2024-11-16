%{
#include <stdlib.h>
#include <string.h>
#include "BST.c"

typedef struct{
    char atom[105];
    int code;
    int ts_code;
} FIP;

int fip_length = 0, current_line = 1;
FIP fip[1005];
BST* identifiers;
BST* constants;

void fip_add(char* atom, int code, int ts_code){
    strcpy(fip[fip_length].atom, atom);
    fip[fip_length].code = code;
    fip[fip_length].ts_code = ts_code;

    fip_length++;
}

void print_fip(){
    printf("FIP:\n");
    int i;
    for(i = 0; i < fip_length; i++){
        if(fip[i].ts_code == -1)
            printf("%s  |  %d  | - \n", fip[i].atom, fip[i].code);
        else
            printf("%s  |  %d  | %d \n", fip[i].atom, fip[i].code, fip[i].ts_code);
    }
} 
%}

%%
"int"       { fip_add(yytext, 8, -1); }
"float"     { fip_add(yytext, 9, -1); }
"string"    { fip_add(yytext, 10, -1); }
"write"     { fip_add(yytext, 24, -1); }
"read"      { fip_add(yytext, 25, -1); }
"if"        { fip_add(yytext, 26, -1); }
"else"      { fip_add(yytext, 27, -1); }
"while"     { fip_add(yytext, 28, -1); }
"const"     { fip_add(yytext, 29, -1); }
"nothing"   { fip_add(yytext, 30, -1); }
"main"      { fip_add(yytext, 33, -1); }
"struct"    { fip_add(yytext, 34, -1); }


"("         { fip_add(yytext, 2, -1); }
")"         { fip_add(yytext, 3, -1); }
"{"         { fip_add(yytext, 4, -1); }
"}"         { fip_add(yytext, 5, -1); }
","         { fip_add(yytext, 6, -1); }
";"         { fip_add(yytext, 7, -1); }
"+"         { fip_add(yytext, 11, -1); }
"-"         { fip_add(yytext, 12, -1); }
"*"         { fip_add(yytext, 13, -1); }
"/"         { fip_add(yytext, 14, -1); }
"%"         { fip_add(yytext, 15, -1); }
"<-"        { fip_add(yytext, 16, -1); }
"=="        { fip_add(yytext, 17, -1); }
"!="        { fip_add(yytext, 18, -1); }
"<="        { fip_add(yytext, 21, -1); }
">="        { fip_add(yytext, 22, -1); }
"<"         { fip_add(yytext, 19, -1); }
">"         { fip_add(yytext, 20, -1); }
":"         { fip_add(yytext, 23, -1); }


[+-]?[0-9]+(\.[0-9]+)? { 
    int index = bst_search(constants, yytext);
    if(index == -1){
        bst_insert(constants, yytext);
        index = bst_search(constants, yytext);
    }

    fip_add(yytext, 1, index);
}

\"[^\"]*\"            { 
    int index = bst_search(constants, yytext);
    if(index == -1){
        bst_insert(constants, yytext);
        index = bst_search(constants, yytext);
    }

    fip_add(yytext, 1, index);
}

[a-zA-Z][a-zA-Z_]*    { 
    if(yyleng > 10)
        printf("Error: Length of %s at line %d surpasses the limit\n", yytext, current_line);

    int index = bst_search(identifiers, yytext);
    if(index == -1){
        bst_insert(identifiers, yytext);
        index = bst_search(identifiers, yytext);
    }

    fip_add(yytext, 0, index);
}


[\n]                   { current_line++; }
[ \t\r]+ ;               
.   {
    printf("Error on line %d. Unrecognized character: %s\n", current_line, yytext);
}
%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv){
    if(argc > 1){
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            return 1;
        }

        identifiers = create_bst();
        constants = create_bst();

        yylex();
        fclose(yyin);

        printf("TS identifiers:\n");
        bst_output(identifiers->root);
        printf("\n\n");
        printf("TS constants:\n");
        bst_output(constants->root);
        printf("\n\n");

        print_fip();

        free_bst(identifiers);
        free_bst(constants);
        return 0;
    }
    return 1;
}