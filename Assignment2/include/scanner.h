#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef enum {
    START,
    IN_ID_1,
    IN_ID_2,
    IN_SYNTAX,
    IN_NUM,
    IN_SPECIAL,
    IN_INLINE_COMMENT,
    IN_MULTILINE_COMMENT,
    IN_MULTILINE_COMMENT_END,
    ERROR
} State;

typedef enum {
    UNDEFINE_SYMBOL,
    REDEFINE_SYMBOL,
    UNDEFINE_TYPE,
} Error;

typedef enum {
    id,
    number,
    assign,
    semicolon,
    brace_open,
    brace_close,
    parenthesis_open,
    parenthesis_close,
    add,
    mult,
    rel_op,
    _print,
    _if,
    _else,
    _while,
    _do,
    _then,
    _end,
    type,
    _begin,
    _comment,
} TokenType;


class Token{
    private:
        string type;
        string value;
        int line;
        int position;
        TokenType tokenType;
    public:
        Token(string type, string value, int line, int position, TokenType tokenType){
            this->type = type;
            this->value = value;
            this->line = line;
            this->position = position;
            this->tokenType = tokenType;
        }
        string getType(){
            return this->type;
        }
        string getValue(){
            return this->value;
        }
        bool isDefined(Token other){
            return this->value == other.getValue();
        }
        int getLine(){
            return this->line;
        }
        int getPosition(){
            return this->position;
        }
        TokenType getTokenType(){
            return this->tokenType;
        }
};

bool checkDefine(Token token, Token tokens[], int lenght);
bool scanner(string input, Token tokens[], int * error_pointer, string * error_symbol, int * error_line, int * error_type);


