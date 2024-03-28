%{
#include<stdio.h>
int line = 1;
%}

/* Start condition */
%x cmt 

/* Flex option: */
%option yylineno

/* Definitions: */
letter [A-Za-z]
digit [0-9]
id {letter}({letter}|{digit})*
number {digit}+
other [^0-9+=>*{}(); \t\n]+
operator [>+*>=;]

/* rules of the language, following regex */
%%
[ \t]+          {};
[\n]            {line++;}
begin           {printf("<BEGIN, %s>\n", yytext);}
end             {printf("<END, %s>\n", yytext);}
int             {printf("<INT, %s>\n", yytext);}
bool            {printf("<BOOLEAN, %s>\n", yytext);}
if              {printf("<IF, %s>\n", yytext);}
then            {printf("<THEN, %s>\n", yytext);}
else            {printf("<ELSE, %s>\n", yytext);}
do              {printf("<DO, %s>\n", yytext);}
while           {printf("<WHILE, %s>\n", yytext);}
print           {printf("<PRINT, %s>\n", yytext);}
{id}            {printf("<ID, %s>\n",yytext);}
"/*"            {BEGIN(cmt); printf("<COMMENT, >\n");} /* Scanner in start condition. Activate rule to recognize comments */
<cmt>[^*\n]*        /* eat anything that's not a '*' */
<cmt>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<cmt>\n             ++line;
<cmt>"*"+"/"        BEGIN(INITIAL);
\/\/(.*)        {printf("<COMMENT, >\n");}
[(){}]          {printf("<BRACKET, %s>\n", yytext);}
"+"             {printf("<ADD, %s>\n", yytext);}
"*"             {printf("<MULT, %s>\n", yytext);}
">"             {printf("<RELOP_GTR, %s>\n", yytext);}
"="             {printf("<ASSIGN, %s>\n", yytext);}
">="            {printf("<RELOP_GEQ, %s>\n", yytext);}
"=="            {printf("<RELOP_EQ, %s>\n", yytext);}
";"             {printf("<SEMICOLON, %s>\n", yytext);}
{number}        {printf("<NUMBER, %s>\n", yytext);}
{id}{other}+        {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;} /* Error rules below*/
{number}({id}|{other})+    {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;}
{operator}+     {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;}
.               {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;}
%%

int yywrap(){
    return 1;
}
int main(){
    /* open a file handle to a particular file */
    FILE *input = fopen("file.txt", "r");
    /* make sure it's valid */
    if (!input){
        printf("File cannot be opened.");
        return 1;
    }
    /* set flex to read from it instead of defaulting to stdin */
    yyin = input;

    /* flex through the input */
    while(yylex());
    fclose(input);
    return 0;
}
