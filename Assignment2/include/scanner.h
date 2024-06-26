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
} ScannerError;

typedef enum {
    _id_,
    _number_,
    _assign_,
    _semicolon_,
    _brace_open_,
    _brace_close_,
    _parenthesis_open_,
    _parenthesis_close_,
    _add_,
    _mult_,
    _rel_op_,
    _print_,
    _if_,
    _else_,
    _while_,
    _do_,
    _then_,
    _end_,
    _type_,
    _begin_,
    _comment_,
} TokenType;


class Token{
    private:
        string type;
        string value;
        int line;
        int position;
        TokenType tokenType;
        // int idType;
    public:
        int idType;
        Token(){
            this->type = "";
            this->value = "";
            this->line = 0;
            this->position = 0;
            this->tokenType = _id_;
            this->idType = -1;
        }
        Token(string type, string value, int line, int position, TokenType tokenType){
            this->type = type;
            this->value = value;
            this->line = line;
            this->position = position;
            this->tokenType = tokenType;
        }
        
        void setType(int type){
            this->idType = type;
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

bool checkDefine(Token *token, Token tokens[], int length);
bool scanner(string input, Token tokens[], int * error_pointer, string * error_symbol, int * error_line, int * error_type);


