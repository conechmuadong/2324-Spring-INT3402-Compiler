#include <iostream>
#include <string>
#include <vector>

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

class Token{
    private:
        string type;
        string value;
        int line;
        int position;
    public:
        Token(){
            this->type = "";
            this->value = "";
            int line = 1;
            int position = 1;
        }
        Token(string type, string value, int line, int position){
            this->type = type;
            this->value = value;
            this->line = line;
            this->position = position;
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
};

bool checkDefine(Token token, Token tokens[], int lenght);
bool scanner(string input, Token tokens[], int * error_pointer, string * error_symbol, int * error_line, int * error_type);


