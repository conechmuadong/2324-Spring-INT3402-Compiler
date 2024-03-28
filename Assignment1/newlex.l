%{
#include<stdio.h>
#include<string.h>
int line = 1;
int num;
int found;
void initialize();
void create(char* name);
int readtokentab(char* name);
%}

/* Start condition */
%x cmt 

/* Definitions: */
letter [A-Za-z]
digit [0-9]
id {letter}({letter}|{digit})*
number {digit}+
other [^0-9+=>*{}(); \t\n]+
operator [>+*>=;]

/* Rules of the language, following regex */
%%
[ \t]+          {};
[\n]            {line++;}
begin           {printf("<BEGIN, %s>\n", yytext);}
end             {printf("<END, %s>\n", yytext);}
int             {printf("<INT, %s>\n", yytext);
                create(yytext);
                };
bool            {printf("<BOOLEAN, %s>\n", yytext);
                create(yytext);
                };
if              {printf("<IF, %s>\n", yytext);}
then            {printf("<THEN, %s>\n", yytext);}
else            {printf("<ELSE, %s>\n", yytext);}
do              {printf("<DO, %s>\n", yytext);}
while           {printf("<WHILE, %s>\n", yytext);}
print           {printf("<PRINT, %s>\n", yytext);}
{id}            {int found = readtokentab(yytext);
                printf("%d\n", found);
                if (found == 1) {fprintf(stderr, "ERROR: Re-generate variable \"%s\"\n", yytext); return 0;}
                else if (found == -1) {fprintf(stderr, "ERROR: Undeclared variable \"%s\"\n", yytext); return 0;}
                else if (found == 2) {printf("<ID, %s>\n",yytext); create(yytext);}
                };
{number}        {printf("<NUMBER, %s>\n", yytext);}
"/*"            {BEGIN(cmt); printf("<COMMENT, >\n");} /* Scanner in start condition. Activate rule to recognize comments containing '/' '*' */
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
{id}{other}+        {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;} /* Error rules below */
{number}({letter}|{other})+    {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;}
{operator}+     {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;}
.               {fprintf(stderr, "ERROR: Invalid input in line %d: \"%s\"\n", line, yytext); return 0;}
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

int readtokentab(char* name)
{
    for (int i = num; i >= 0; i--){
        int comp = strcmp(arr_tokentab[i].Name, name);
        if ((comp == 0 && strcmp(arr_tokentab[i-1].Name, "bool") == 0) || (comp == 0 && strcmp(arr_tokentab[i].Name, "int") == 0)){
            return 1;
        }
        else if ((comp != 0 && strcmp(arr_tokentab[i-1].Name, "bool") != 0) || (comp != 0 && strcmp(arr_tokentab[i].Name, "int") != 0)){
            return 2;
        }
    }
    return -1;
}

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

    initialize();
    /* flex through the input */
    while(yylex());
    fclose(input);
    return 0;
}
