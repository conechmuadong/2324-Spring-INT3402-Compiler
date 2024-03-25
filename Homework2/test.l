%{
#include <iostream>
using namespace std;
int line = 0;
int character = 0;
%}

%option noyywrap c++

id [a-z][0-9][a-z0-9]*
rel_op [<|>]={0,1}
if if
then then
else else
assign =
number [0-9]+
semi  ;

ws [\t]+

%%

{ws} /* skip blanks and tabs */

{if}  std::cout << "<IF, >" << endl;

{then} std::cout << "<THEN, >" << endl;

{else} std::cout << "<ELSE, >" << endl;

{id} std::cout << "<ID, " << YYText() << ">" << endl;

{rel_op} std::cout << "<REL_OP, " << YYText() << ">" << endl;

{semi} std::cout <<"<SEMI, >" << endl;

{number} std::cout << "<NUM, " << YYText() << ">" << endl;

{assign} std::cout << "<ASSIGN, >" << endl;

\n {line++; character=0; }

.^[\t\n] {character++;}

%%

// This include is required if main() is an another source file.
//#include <FlexLexer.h>

int main( int /* argc */, char** /* argv */ )
{
    FlexLexer* lexer = new yyFlexLexer;
    while(lexer->yylex() != 0)
        character++;
        ;
    return 0;
}

