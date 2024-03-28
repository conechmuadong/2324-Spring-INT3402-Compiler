%{
#include<stdio.h>
int count = 0;
int error = 0;
int line = 1;
%}

/* Start condition */
%x comment 

/* Definitions: */
letter [A-Za-z]
digit [0-9]
id {letter}({letter}|{digit})*
number {digit}+

/* Rules of the language, following regex */
%%
[ \t]+                  {count += yyleng;}
[\n]+                   {line++; count = 0;}
begin                   {printf("<BEGIN, %s>\n", yytext); count += yyleng;}
end                     {printf("<END, %s>\n", yytext); count += yyleng;}
int                     {printf("<INT, %s>\n", yytext); count += yyleng;}
bool                    {printf("<BOOLEAN, %s>\n", yytext); count += yyleng;}
if                      {printf("<IF, %s>\n", yytext); count += yyleng;}
then                    {printf("<THEN, %s>\n", yytext); count += yyleng;}
else                    {printf("<ELSE, %s>\n", yytext); count += yyleng;}
do                      {printf("<DO, %s>\n", yytext); count += yyleng;}
while                   {printf("<WHILE, %s>\n", yytext); count += yyleng;}
print                   {printf("<PRINT, %s>\n", yytext); count += yyleng;}
{id}                    {printf("<ID, %s>\n",yytext); count += yyleng;}
{number}                {printf("<NUMBER, %s>\n", yytext); count += yyleng;}
{number}{id}+           {count += yyleng; error = 1; return 0;}
"/*"                    {BEGIN(comment); printf("<COMMENT, >\n");} /* Scanner in start condition. Activate rule to recognize comments */
<comment>[^*\n]*        /* Remove anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* Remove '*'s not followed by '/'s */
<comment>\n             ++line; /* Maintain count of current input line */
<comment>"*"+"/"        BEGIN(INITIAL);
\/\/(.*)                {printf("<COMMENT, >\n");}
[(){}]                  {printf("<BRACKET, %s>\n", yytext); count += yyleng;}
"+"                     {printf("<ADD, %s>\n", yytext); count += yyleng;}
"*"                     {printf("<MULT, %s>\n", yytext); count += yyleng;}
">"                     {printf("<RELOP_GTR, %s>\n", yytext); count += yyleng;}
"="                     {printf("<ASSIGN, %s>\n", yytext); count += yyleng;}
">="                    {printf("<RELOP_GEQ, %s>\n", yytext); count += yyleng;}
"=="                    {printf("<RELOP_EQ, %s>\n", yytext); count += yyleng;}
";"                     {printf("<SEMICOLON, %s>\n", yytext); count += yyleng;}
.                       {count += yyleng; error = 1; return 0;}
%%

int yywrap(){
    return 1;
}
int main(int argc, char * args[]){
    if (argc < 3) {
        printf("./parser expected 2 arguments but receive %d\n", argc); 
        printf("Usage: ./parser INPUT OUTPUT \n \t - INPUT: PATH to the input file \n \t -OUTPUT: PATH to file the parser will save the result to.");
        return -1;
    }
    
    char * input = args[1];
    char * output = args[2];    
    /* Open a file handle to a particular file */
    FILE * myfile = fopen(input, "r");
    /* Make sure it's valid */
    if (!myfile){
        printf("Failed to open the file: %s", input);
        return -1;
    }
    
    /* Set flex to read from it instead of defaulting to stdin and write the result to another file */
    yyin = input;
    yyout = fopen(output, "w");

    /* Flex through the input, stop if encountered a pre-defined error */
    while(yylex());
    if (error == 1){
        fclose(yyout);
        remove(output);
        yyout = fopen(output, "w");
        fprintf(yyout, "Error at line %d at position %d, can't resolve the symbol: %s", line, count, yytext); 
        printf("Error at line %d at position %d, can't resolve the symbol: %s", yylineno, count, yytext);
        return -1;
    }
    return 0;
}
