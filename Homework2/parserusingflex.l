%{
#include<stdio.h>
int count = 0; 
int error = 0; 
%}

/* Flex option: */
%option yylineno

/* Definitions: */
letter [A-Za-z]
digit [0-9]
id {letter}{digit}({letter}|{digit})*
number {digit}+

/* rules of the language, following regex */
%%
[ ]         {count+=yyleng;}
"\n"        {count = 0;}
{id}        {fprintf(yyout, "<ID, %s>\n", yytext); count += yyleng;}
if          {fprintf(yyout, "<IF, >\n", yytext); count += yyleng;}
then        {fprintf(yyout, "<THEN, >\n", yytext); count += yyleng;}
else        {fprintf(yyout, "<ELSE, >\n", yytext); count += yyleng;}
";"         {fprintf(yyout, "<SEMICOLON, >\n", yytext); count += yyleng;}
">"         {fprintf(yyout, "<REL_OP, %s>\n", yytext); count += yyleng;}
">="         {fprintf(yyout, "<REL_OP, %s>\n", yytext); count += yyleng;}
"="         {fprintf(yyout, "<ASSIGN, >\n", yytext); count += yyleng;}
{number}    {fprintf(yyout, "<NUMBER, %s>\n", yytext); count += yyleng;}
.           {count+=yyleng; error = 1;return 0;}
%%

int yywrap(){
    return 1;
}
int main(int argc, char* args[]){
    if (argc < 3) {
        printf("./parser expected 2 arguments but receive %d\n", argc); 
        printf("Usage: ./parser INPUT OUTPUT \n \t - INPUT: PATH to the input file \n \t -OUTPUT: PATH to file the parser will save the result to.");
        return -1;
    }
    char * input = args[1];
    char * output = args[2];
    FILE * myfile = fopen(input, "r");
    if (!myfile){
        printf("Failed to open the file: %s", input);
        return -1;
    }
    yyin = myfile;
    yyout = fopen(output, "w");
    yylex();

    if (error == 1){
        fclose(yyout);
        remove(output);
        yyout = fopen(output, "w");
        fprintf(yyout, "Error at line %d at position %d, can't resolve the symbol: %s", yylineno, count, yytext); 
        printf("Error at line %d at position %d, can't resolve the symbol: %s", yylineno, count, yytext);
        return -1;
    }
    return 0;
}
