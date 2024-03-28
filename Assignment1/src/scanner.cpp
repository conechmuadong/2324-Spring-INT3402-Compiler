#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

enum State{
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
};

enum ERROR{
    UNDEFINE_SYMBOL,
    REDEFINE_SYMBOL,
    UNDEFINE_TYPE,
};

class Token{
    private:
        string type;
        string value;
    public:
        Token(){
            this->type = "";
            this->value = "";
        }
        Token(string type, string value){
            this->type = type;
            this->value = value;
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

bool checkDefine(Token token, Token tokens[], int lenght){
    for (int i = 0; i < lenght; i++){
        if (tokens[i].isDefined(token) && tokens[i].getType() == "ID"){
            return true;
        }
    }   
    return false;
}
/**
 * @def parser
 * @brief This function will parse the input string and return a list of tokens
 * @param input The input string to be parsed
*/
bool parser(string input, Token tokens[], int * error_pointer, string * error_symbol, int * error_line, int * error_type){
    int i = 0, j = 0, k =0;
    int state = START;
    string token = "";
    string tokenType = "";
    while (j < input.length()){
        switch (state){
            case START:
                if (input.substr(j, 1) == " "){
                    j++;
                    k++;
                    continue;
                }
                if (input.substr(j, 1) == "\n"){
                    j++;
                    k = 0;
                    *error_line+=1;
                    continue;
                }
                token = token.append(1, input[j]);
                if (regex_match(input.substr(j, 1), regex("[a-z]+"))){
                    state = IN_ID_1;
                    tokenType = "ID";
                    j++;
                }
                else if (regex_match(input.substr(j, 1), regex("[0-9]+"))){
                    state = IN_NUM;
                    tokenType = "NUM";
                    j++;
                }
                else {
                    state = IN_SPECIAL;
                }
                break;
            case ERROR:
                *error_pointer = k;
                *error_symbol = token;
                return false;
                break;
            case IN_ID_1:
                if (regex_match(input.substr(j, 1), regex("[0-9]+"))){
                    state = IN_ID_2;
                    token = token.append(1, input[j]);
                    j++;
                } 
                else if (regex_match(input.substr(j, 1), regex("[a-z]+"))){
                    token = token.append(1, input[j]);
                    j++;
                    state = IN_SYNTAX;
                } 
                else {
                    state = START;
                    tokens[i++] = Token("ID", token);
                    if (tokens[i-2].getType() == "TYPE" && checkDefine(tokens[i-1], tokens, i-1)){
                        *error_type = REDEFINE_SYMBOL;
                        state = ERROR;
                    }
                    else if (tokens[i-2].getType()!="TYPE" && !checkDefine(tokens[i-1], tokens, i-1)){
                        *error_type = UNDEFINE_SYMBOL;
                        state = ERROR;
                    }
                    else{
                        token = "";
                        tokenType = "";
                    }
                }
                break;
            case IN_ID_2:
                if (regex_match(input.substr(j, 1), regex("[0-9]+"))){
                    token = token.append(1, input[j]);
                    j++;
                } 
                else if (!regex_match(input.substr(j, 1), regex("[a-zA-Z0-9]+"))){
                    state = START;
                    tokens[i++] = Token(tokenType, token);
                    if (tokens[i-2].getType() == "TYPE" && checkDefine(tokens[i-1], tokens, i-1)){
                        *error_type = REDEFINE_SYMBOL;
                        state = ERROR;
                    }
                    else if (tokens[i-2].getType()!="TYPE" && !checkDefine(tokens[i-1], tokens, i-1)){
                        *error_type = UNDEFINE_SYMBOL;
                        state = ERROR;
                    }
                    else{
                        token = "";
                        tokenType = "";
                    }
                }
                else {
                    state = ERROR;
                    *error_type = UNDEFINE_TYPE;
                }
                break;
            case IN_NUM:
                if (regex_match(input.substr(j, 1), regex("[0-9]+"))){
                    token = token.append(1, input[j]);
                    j++;
                } 
                else if (!regex_match(input.substr(j, 1), regex("[a-zA-Z0-9]+"))){
                    state = START;
                    tokens[i++] = Token(tokenType, token);
                    token = "";
                    tokenType = "";
                }
                else {
                    state = ERROR;
                    *error_type = UNDEFINE_TYPE;
                }
                break;
            case IN_SPECIAL:
                state = START;
                if (token == ";"){
                    tokens[i++] = Token("SEMI", " ");
                }
                else if (token == ">"){
                    if (input[j+1] == '='){
                        tokens[i++] = Token("REL_OP", ">=");
                        j++;
                    }
                    else{
                        tokens[i++] = Token("REL_OP", token);
                    }
                    // tokens[i++] = Token("REL_OP", token);
                }
                else if (token == "="){
                    if (input[j+1] == '='){
                        tokens[i++] = Token("REL_OP", "==");
                        j++;
                    }
                    else{
                        tokens[i++] = Token("ASSIGN", " ");
                    }
                    // tokens[i++] = Token("ASSIGN", " ");
                }
                else if (token == "{" || token=="}" || token == "(" || token == ")")
                {
                    tokens[i++] = Token("BRACKET", token);
                }                                      
                else if (token == "+")
                {
                    tokens[i++] = Token("ADD_OP", " ");
                }
                else if (token == "*")
                {
                    tokens[i++] = Token("MUL_OP", " ");
                }
                else if (token == "/*")
                {
                    state = IN_MULTILINE_COMMENT;
                }
                else if (token == "//")
                {
                    state = IN_INLINE_COMMENT;
                }
                else {
                    state = ERROR;
                }
                if (state == ERROR){
                    if (!regex_match(input.substr(j+1, 1), regex("[a-zA-Z0-9]+"))){
                        state = IN_SPECIAL;
                        token = token.append(1, input[j+1]);
                        if (input[j+1] == '\n'|| input[j+1] == ' '){
                            // error_line++;
                            state = ERROR;
                        }
                        j++;
                    }
                }
                else{
                    token = "";
                    tokenType = "";
                    j++;
                }
                break;
            case IN_SYNTAX:
                if (regex_match(input.substr(j,1), regex("[;>={}()]")) || input.substr(j, 1) == " "||input[j] == '\n'){
                    state = START;
                    if(token == "if")
                        tokens[i++] = Token("IF", " ");
                    else if (token == "then"){
                        tokens[i++] = Token("THEN", " ");
                    }
                    else if (token == "else"){
                        tokens[i++] = Token("ELSE", " ");
                    }
                    else if (token == "do")
                    {
                        tokens[i++] = Token("DO", " ");
                    }
                    else if (token == "while")
                    {
                        tokens[i++] = Token("WHILE", " ");
                    }
                    else if (token == "begin"){
                        tokens[i++] = Token("BEGIN", " ");
                    } 
                    else if (token == "end"){
                        tokens[i++] = Token("END", " ");
                    }
                    else if (token == "print")
                    {
                        tokens[i++] = Token("PRINT", " ");
                    }
                    else if (token == "bool" || token == "int"){
                        tokens[i++] = Token("TYPE", token);
                    }
                    else{                   
                        tokens[i++] = Token("ID", token);
                        if (tokens[i-2].getType() == "TYPE" && checkDefine(tokens[i-1], tokens, i-1)){
                        *error_type = REDEFINE_SYMBOL;
                        state = ERROR;
                        }
                        else if (tokens[i-2].getType()!="TYPE" && !checkDefine(tokens[i-1], tokens, i-1)){
                            *error_type = UNDEFINE_SYMBOL;
                            state = ERROR;
                        }
                    }
                    if (state != ERROR){
                        token = "";
                        tokenType = "";
                    }
                }
                else if (regex_match(input.substr(j,1), regex("[a-z]+"))){
                    token = token.append(1, input[j]);
                    j++;
                }
                else if (regex_match(input.substr(j,1), regex("[0-9]+"))){
                    token = token.append(1, input[j]);
                    j++;
                    state = IN_ID_2;
                }
                else{
                    state = ERROR;
                    *error_type = UNDEFINE_TYPE;
                }
                break;
            case IN_MULTILINE_COMMENT:
                if (input.substr(j, 1) == "*"){
                    state = IN_MULTILINE_COMMENT_END;
                }else if ( j == input.length() - 1){
                    tokens[i++] = Token("COMMENT", " ");
                    state = START;
                } 
                else {
                    j++;
                }
                break;
            case IN_MULTILINE_COMMENT_END:
                if (input.substr(j, 1) == "/"){
                    state = START;
                    tokens[i++] = Token("COMMENT", " ");
                    token = "";
                    tokenType = "";
                    j++;
                }
                else if ( j == input.length() - 1){
                    tokens[i++] = Token("COMMENT", " ");
                    state = START;
                }
                else {
                    state = IN_MULTILINE_COMMENT;
                }
                break;
            case IN_INLINE_COMMENT:
                if (input.substr(j, 1) == "\n"){
                    state = START;
                    tokens[i++] = Token("COMMENT", " ");
                    token = "";
                    tokenType = "";
                } else {
                    j++;
                }
                break;
        }
    }
    return true;   
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout <<"./parser expected 2 arguments but receive "<< argc << "\n" << "Usage: ./parser INPUT OUTPUT \n \t - INPUT: PATH to the input file \n \t -OUTPUT: PATH to file the parser will save the result to." << endl;
        return -1;
    }

    string filename = argv[1]; // "input.in";
    ifstream file(filename);
    string outputFilename = argv[2]; // "output.out";
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << endl;
        return -1;
    }

    string input;
    int i = 0;
    while (!file.eof()) {
        input.append(1, file.get());
    }

    file.close();

    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cout << "Failed to create the output file: " << outputFilename << endl;
    }
    
    Token tokens[1000];
    int error_pointer = 0;
    int error_line = 1;
    int error_type = 0;
    string error_symbol = "";
    if (!parser(input, tokens, &error_pointer, &error_symbol, &error_line, &error_type)){
        if (error_type != REDEFINE_SYMBOL){
            outputFile << "Error at line " << error_line << " at position " << error_pointer << ", can't resolve the symbol "<< error_symbol << endl;
            cout << "Error at line " << error_line << " at position " << error_pointer << ", can't resolve the symbol "<< error_symbol << endl;
        }
        else {
            outputFile << "Error at line " << error_line << " at position " << error_pointer << ", " << error_symbol << "is redefined";
        }
        return -1;
    }
    for (int j = 0; j < 1000; j++){
        if (tokens[j].getType() == ""){
            break;
        }
        outputFile << "<" << tokens[j].getType() << ", " << tokens[j].getValue()<< ">;" << endl;
    }     
}