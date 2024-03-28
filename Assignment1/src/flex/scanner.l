%{
#include<stdio.h>
int count = 0;
int error = 0;
int line = 1;
%}

/* Start condition */
%x comment 

/* Definitions: */
letter      [A-Za-z]
digit       [0-9]
id          {letter}({letter}|{digit})*
number      {digit}+
other       [^0-9+=>*{}(); \t\n]+

/* Rules of the language, following regex */
%%
[ \t]+                          {count += yyleng;}
[\n]+                           {line++; count = 0;}

begin                           {fprintf(yyout, "<BEGIN, %s>\n", yytext); count += yyleng;}
end                             {fprintf(yyout, "<END, %s>\n", yytext); count += yyleng;}
int                             {fprintf(yyout, "<TYPE, %s>\n", yytext); count += yyleng;}
bool                            {fprintf(yyout, "<TYPE, %s>\n", yytext); count += yyleng;}
if                              {fprintf(yyout, "<IF, %s>\n", yytext); count += yyleng;}
then                            {fprintf(yyout, "<THEN, %s>\n", yytext); count += yyleng;}
else                            {fprintf(yyout, "<ELSE, %s>\n", yytext); count += yyleng;}
do                              {fprintf(yyout, "<DO, %s>\n", yytext); count += yyleng;}
while                           {fprintf(yyout, "<WHILE, %s>\n", yytext); count += yyleng;}
print                           {fprintf(yyout, "<PRINT, %s>\n", yytext); count += yyleng;}

{id}                            {fprintf(yyout, "<ID, %s>\n", yytext); count += yyleng;}
{number}                        {fprintf(yyout, "<NUMBER, %s>\n", yytext); count += yyleng;}

"/*"                            {BEGIN(comment); fprintf(yyout, "<COMMENT, >\n"); count += yyleng;} /* Scanner in start condition. Activate rule to identify comments containing '/' '*' */
<comment>[^*\n]*                /* Remove anything that's not a '*' */
<comment>"*"+[^*/\n]*           /* Remove '*'s not followed by '/'s */
<comment>\n                     ++line; /* Maintain count of current input line */
<comment>"*"+"/"                BEGIN(INITIAL);

\/\/(.*)                        {fprintf(yyout, "<COMMENT, >\n"); count += yyleng;} /* Identify single-line comments */
[(){}]                          {fprintf(yyout, "<BRACKET, %s>\n", yytext); count += yyleng;}
"+"                             {fprintf(yyout, "<ADD, %s>\n", yytext); count += yyleng;}
"*"                             {fprintf(yyout, "<MULTIPLY, %s>\n", yytext); count += yyleng;}
">"                             {fprintf(yyout, "<RELOP, %s>\n", yytext); count += yyleng;}
"="                             {fprintf(yyout, "<ASSIGN, %s>\n", yytext); count += yyleng;}
">="                            {fprintf(yyout, "<RELOP, %s>\n", yytext); count += yyleng;}
"=="                            {fprintf(yyout, "<RELOP, %s>\n", yytext); count += yyleng;}
";"                             {fprintf(yyout, "<SEMICOLON, %s>\n", yytext); count += yyleng;}

{id}{other}+                    {count += yyleng; error = 1; return 0;} /* Error rules below */
{number}({letter}|{other})+     {count += yyleng; error = 1; return 0;}
.                               {count += yyleng; error = 1; return 0;}
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
    /* Open a file handle to a particular file */
    FILE * myfile = fopen(input, "r");
    /* Make sure it's valid */
    if (!myfile){
        printf("Failed to open the file: %s\n", input);
        return -1;
    }
    
    /* Set flex to read from it instead of defaulting to stdin and write the result to another file */
    yyin = myfile;
    yyout = fopen(output, "w");

    /* Flex through the input, stop if encountered a pre-defined error */
    while(yylex());
    if (error == 1){
        fclose(yyout);
        remove(output);
        yyout = fopen(output, "w");
        fprintf(yyout, "Error at line %d at position %d, can't resolve the symbol: %s\n", line, count, yytext); 
        printf("Error at line %d at position %d, can't resolve the symbol: %s", line, count, yytext);
        return -1;
    }
    return 0;
}
