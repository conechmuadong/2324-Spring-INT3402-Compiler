%{
#include<stdio.h>
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
%x cmt 

/* Flex option: */
%option yylineno

/* Definitions: */
letter [A-Za-z]
digit [0-9]
id {letter}+({digit})*
number {digit}+
other [^0-9+=>*{}(); \t\n]+

/* rules of the language, following regex */
%%
[ \t]+          {count+=yyleng;};
[\n]            {line++; count = 0;}
begin           {fprintf(yyout,"<BEGIN, %s>\n", yytext); count+=yyleng;}
end             {fprintf(yyout,"<END, %s>\n", yytext); count+=yyleng;}
int             {fprintf(yyout,"<TYPE, %s>\n", yytext); create(yytext); count+=yyleng;}
bool            {fprintf(yyout,"<TYPE, %s>\n", yytext); create(yytext); count+=yyleng;}
if              {fprintf(yyout,"<IF, %s>\n", yytext); count+=yyleng;}
then            {fprintf(yyout,"<THEN, %s>\n", yytext); count+=yyleng;}
else            {fprintf(yyout,"<ELSE, %s>\n", yytext); count+=yyleng;}
do              {fprintf(yyout,"<DO, %s>\n", yytext); count+=yyleng;}
while           {fprintf(yyout,"<WHILE, %s>\n", yytext); count+=yyleng;}
print           {fprintf(yyout,"<PRINT, %s>\n", yytext); count+=yyleng;}
{id}            {int found = readtokentab(yytext);
                if (found == -1) {error = 2; return 0;}
                else if (found == -2) {error = 1; return 0;}
                else if (found == 0) {fprintf(yyout,"<ID, %s>\n",yytext); count+=yyleng; create(yytext);}
                }
{number}        {fprintf(yyout,"<NUM, %s>\n", yytext); count+=yyleng;}
"/*"            {BEGIN(cmt); fprintf(yyout,"<COMMENT, >\n"); count+=yyleng;} /* Scanner in start condition. Activate rule to recognize comments */
<cmt>[^*\n]*        /* eat anything that's not a '*' */
<cmt>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<cmt>\n             ++line;
<cmt>"*"+"/"        BEGIN(INITIAL);
\/\/(.*)        {fprintf(yyout,"<COMMENT, >\n"); count+=yyleng;}
[(){}]          {fprintf(yyout,"<BRACKET, %s>\n", yytext); count+=yyleng;}
"+"             {fprintf(yyout,"<ADD_OP, %s>\n", yytext); count+=yyleng;}
"*"             {fprintf(yyout,"<MUL_OP, %s>\n", yytext); count+=yyleng;}
">"             {fprintf(yyout,"<RELOP, %s>\n", yytext); count+=yyleng;}
"="             {fprintf(yyout,"<ASSIGN, %s>\n", yytext); count+=yyleng;}
">="            {fprintf(yyout,"<RELOP, %s>\n", yytext); count+=yyleng;}
"=="            {fprintf(yyout,"<RELOP, %s>\n", yytext); count+=yyleng;}
";"             {fprintf(yyout,"<SEMI, %s>\n", yytext); count+=yyleng;}

{id}{other}+        {error = 1; return 0;} /* Error rules below*/
{number}({id}|{other})+    {error = 1; return 0;}
.               {count+=yyleng; error = 1; return 0;}
%%

struct tokentab 
{
    char Name[18];
};
struct tokentab arr_tokentab[200];

void initialize()
{
    num = 0;
    char name[18] = "BEGIN";
    create(name);
}

void create(char* name)
{
    num += 1;
    strcpy(arr_tokentab[num].Name, name);
}

int checkDefine(char* name){
    for (int i = 0; i < num+1; i++){
        if (strcmp(arr_tokentab[i].Name, name)==0){
            return 1;
        }
    }   
    return 0;
}

int readtokentab(char* name)
{ 
    if ((strcmp(arr_tokentab[num].Name, "bool") == 0 || strcmp(arr_tokentab[num].Name, "int") == 0) && checkDefine(name) == 1){
        return -1;
    }
    else if ((strcmp(arr_tokentab[num].Name, "bool") != 0 && strcmp(arr_tokentab[num].Name, "int") != 0) && checkDefine(name) == 0){
        return -2;
    }
    return 0;
}

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
    }
    return 0;
}
