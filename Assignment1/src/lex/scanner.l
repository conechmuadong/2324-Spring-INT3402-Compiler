%{
#include<stdio.h>
#include<string.h>
int line = 1;
int count = 0;
int error = 0;
int num;
int found;

void initialize();
void create(char* name);
int readtokentab(char* name);
int checkDefine(char* name);
%}

/* Start condition */
%x comment 

/* Definitions: */
letter   [A-Za-z]
digit    [0-9]
id       {letter}+({digit})*
number   {digit}+
other    [^0-9+=>*{}(); \t\n]+

/* Rules of the language, following regex */
%%
[ \t]+                      {count+=yyleng;};
[\n]                        {line++; count = 0;}

begin                       {fprintf(yyout,"<BEGIN, >\n"); count+=yyleng;}
end                         {fprintf(yyout,"<END, >\n"); count+=yyleng;}
int                         {fprintf(yyout,"<TYPE, %s>\n", yytext); create(yytext); count+=yyleng;}
bool                        {fprintf(yyout,"<TYPE, %s>\n", yytext); create(yytext); count+=yyleng;}
if                          {fprintf(yyout,"<IF, >\n"); count+=yyleng;}
then                        {fprintf(yyout,"<THEN, >\n"); count+=yyleng;}
else                        {fprintf(yyout,"<ELSE, >\n"); count+=yyleng;}
do                          {fprintf(yyout,"<DO, >\n"); count+=yyleng;}
while                       {fprintf(yyout,"<WHILE, >\n"); count+=yyleng;}
print                       {fprintf(yyout,"<PRINT, >\n"); count+=yyleng;}

{id}                        {int found = readtokentab(yytext);
                            if (found == -1) {count+=yyleng; error = 2; return 0;}
                            else if (found == -2) {count+=yyleng; error = 1; return 0;}
                            else if (found == 0) {fprintf(yyout,"<ID, %s>\n",yytext); count+=yyleng; create(yytext);}
                            }
{number}                    {fprintf(yyout,"<NUM, %s>\n", yytext); count+=yyleng;}

"/*"                        {BEGIN(comment); fprintf(yyout,"<COMMENT, >\n"); count+=yyleng;} /* Scanner in start condition. Activate rule to recognize comments */
<comment>[^*\n]*            /* Remove anything that's not a '*' */
<comment>"*"+[^*/\n]*       /* Remove up '*'s not followed by '/'s */
<comment>\n                 ++line; /* Maintain count of current input line */
<comment>"*"+"/"            BEGIN(INITIAL);

\/\/(.*)                    {fprintf(yyout,"<COMMENT, >\n"); count+=yyleng;}
[(){}]                      {fprintf(yyout,"<BRACKET, %s>\n", yytext); count+=yyleng;}
"+"                         {fprintf(yyout,"<ADD_OP, >\n"); count+=yyleng;}
"*"                         {fprintf(yyout,"<MUL_OP, >\n"); count+=yyleng;}
">"                         {fprintf(yyout,"<RELOP, %s>\n", yytext); count+=yyleng;}
"="                         {fprintf(yyout,"<ASSIGN, >\n"); count+=yyleng;}
">="                        {fprintf(yyout,"<RELOP, %s>\n", yytext); count+=yyleng;}
"=="                        {fprintf(yyout,"<RELOP, %s>\n", yytext); count+=yyleng;}
";"                         {fprintf(yyout,"<SEMI, >\n"); count+=yyleng;}

{id}{other}+                {count+=yyleng; error = 1; return 0;} /* Error rules below */
{number}({id}|{other})+     {count+=yyleng; error = 1; return 0;}
.                           {count+=yyleng; error = 1; return 0;}
%%

struct tokentab 
{
    char Name[18];
};
struct tokentab arr_tokentab[200]; /* Data structure in which the token table entries are stored */

void initialize() /* Function to enter initial value into the token table */
{
    num = 0;
    char name[18] = "BEGIN";
    create(name);
}

void create(char* name) /* Function to create a new entry into the token table */
{
    num += 1;
    strcpy(arr_tokentab[num].Name, name);
}

int checkDefine(char* name) /* Function to check if the entry is already in the token table or not */
{
    for (int i = 0; i < num+1; i++){
        if (strcmp(arr_tokentab[i].Name, name)==0){
            return 1;
        }
    }   
    return 0;
}

int readtokentab(char* name) /* Function to check if the variable is reidentified/unidentified */
{ 
    if ((strcmp(arr_tokentab[num].Name, "bool") == 0 || strcmp(arr_tokentab[num].Name, "int") == 0) && checkDefine(name) == 1){
        return -1; /* Case of reidentified */
    }
    else if ((strcmp(arr_tokentab[num].Name, "bool") != 0 && strcmp(arr_tokentab[num].Name, "int") != 0) && checkDefine(name) == 0){
        return -2; /* Case of unidentified */
    }
    return 0;
}

int yywrap()
{
    return 1;
}

int main(int argc, char* args[])
{
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
    yyin = myfile;
    yyout = fopen(output, "w");

    /* Flex through the input, stop if encountered a pre-defined error */
    initialize();
    while(yylex());
    if (error == 1){
        fclose(yyout);
        remove(output);
        yyout = fopen(output, "w");
        fprintf(yyout, "Error at line %d at position %d, can't resolve the symbol: %s", line, count, yytext); 
        printf("Error at line %d at position %d, can't resolve the symbol: %s\n", line, count, yytext);
        return -1;
    }
    else if (error == 2){
        fclose(yyout);
        remove(output);
        yyout = fopen(output, "w");
        fprintf(yyout, "Error at line %d at position %d, %s is redefined", line, count, yytext); 
        printf("Error at line %d at position %d, %s is redefined\n", line, count, yytext);
        return -1;
    }
    return 0;
}
