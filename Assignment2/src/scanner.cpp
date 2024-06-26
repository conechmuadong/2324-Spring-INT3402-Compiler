#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "scanner.h"

using namespace std;

bool checkDefine(Token *token, Token tokens[], int length){
    for (int i = 0; i < length; i++){
        if (tokens[i].isDefined(*token) && tokens[i].getType() == "ID"){
            token->idType = tokens[i].idType;
            return true;
        }
    }   
    return false;
}

void setDefineType(Token tokens[], int i){
    cout << tokens[i-1].getValue() << endl;
    cout << tokens[i-2].getValue() << endl;
    if (tokens[i-2].getType() == "TYPE"){
        if (tokens[i-2].getValue()=="int"){
            // cout << "int - scanner" << endl;
            tokens[i-1].idType = 1;
            // cout << tokens[i-1].getValue() << " " << tokens[i-1].idType << endl;
        }
        else if (tokens[i-2].getValue()=="bool")
            tokens[i-1].idType = 0;
        return;
    }
    if (!checkDefine(&tokens[i-1], tokens, i-1)){
        tokens[i-1].idType = -1;
    }
}

/**
 * @def parser
 * @brief This function will parse the input string and return a list of tokens
 * @param input The input string to be parsed
*/
bool scanner(string input, Token tokens[], int * error_pointer, string * error_symbol, int * error_line, int * error_type){
    int i = 0, j = 0, k = 0;
    int state = START;
    string token = "";
    string tokenType = "";
    while (j < input.length()){
        switch (state){
            case START:
                if (input.substr(j, 1) == " " || input.substr(j, 1) == "\t"){
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
                    tokens[i++] = Token("ID", token, *error_line, k, TokenType::_id_);
                    setDefineType(tokens, i);
                    // cout << tokens[i-1].getValue() << "k" << tokens[i-1].idType << endl;
                    token = "";
                    tokenType = "";
                }
                break;
            case IN_ID_2:
                if (regex_match(input.substr(j, 1), regex("[0-9]+"))){
                    token = token.append(1, input[j]);
                    j++;
                } 
                else if (!regex_match(input.substr(j, 1), regex("[a-zA-Z0-9]+"))){
                    state = START;
                    tokens[i++] = Token(tokenType, token, *error_line, k, TokenType::_id_);
                    setDefineType(tokens, i);
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
                    tokens[i++] = Token(tokenType, token, *error_line, k, TokenType::_number_);
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
                    tokens[i++] = Token("SEMI", " ", *error_line, k, TokenType::_semicolon_);
                }
                else if (token == ">"){
                    if (input[j+1] == '='){
                        tokens[i++] = Token("REL_OP", ">=", *error_line, k, TokenType::_rel_op_);
                        j++;
                    }
                    else{
                        tokens[i++] = Token("REL_OP", token, *error_line, k, TokenType::_rel_op_);
                    }
                    // tokens[i++] = Token("REL_OP", token);
                }
                else if (token == "="){
                    if (input[j+1] == '='){
                        tokens[i++] = Token("REL_OP", "==", *error_line, k, TokenType::_rel_op_);
                        j++;
                    }
                    else{
                        tokens[i++] = Token("ASSIGN", " ", *error_line, k, TokenType::_assign_);
                    }
                    // tokens[i++] = Token("ASSIGN", " ", *error_line, k);
                }
                else if (token == "{" || token=="}" || token == "(" || token == ")")
                {
                    if (token == "{")
                    {
                        tokens[i++] = Token("BRACE_OPEN", " ", *error_line, k, TokenType::_brace_open_);
                    }
                    else if (token == "}")
                    {
                        tokens[i++] = Token("BRACE_CLOSE", " ", *error_line, k, TokenType::_brace_close_);
                    }
                    else if (token == "(")
                    {
                        tokens[i++] = Token("PARENTHESIS_OPEN", " ", *error_line, k, TokenType::_parenthesis_open_);
                    }
                    else if (token == ")")
                    {
                        tokens[i++] = Token("PARENTHESIS_CLOSE", " ", *error_line, k, TokenType::_parenthesis_close_);
                    }
                }                                      
                else if (token == "+")
                {
                    tokens[i++] = Token("ADD_OP", " ", *error_line, k, TokenType::_add_);
                }
                else if (token == "*")
                {
                    tokens[i++] = Token("MUL_OP", " ", *error_line, k, TokenType::_mult_);
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
                        tokens[i++] = Token("IF", " ", *error_line, k, TokenType::_if_);
                    else if (token == "then"){
                        tokens[i++] = Token("THEN", " ", *error_line, k, TokenType::_then_);
                    }
                    else if (token == "else"){
                        tokens[i++] = Token("ELSE", " ", *error_line, k, TokenType::_else_);
                    }
                    else if (token == "do")
                    {
                        tokens[i++] = Token("DO", " ", *error_line, k, TokenType::_do_);
                    }
                    else if (token == "while")
                    {
                        tokens[i++] = Token("WHILE", " ", *error_line, k, TokenType::_while_);
                    }
                    else if (token == "begin"){
                        tokens[i++] = Token("BEGIN", " ", *error_line, k, TokenType::_begin_);
                    } 
                    else if (token == "end"){
                        tokens[i++] = Token("END", " ", *error_line, k, TokenType::_end_);
                    }
                    else if (token == "print")
                    {
                        tokens[i++] = Token("PRINT", " ", *error_line, k, TokenType::_print_);
                    }
                    else if (token == "bool" || token == "int"){
                        tokens[i++] = Token("TYPE", token, *error_line, k, TokenType::_type_);
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
                    j++;
                }else if ( j == input.length() - 1){
                    tokens[i++] = Token("COMMENT", " ", *error_line, k, TokenType::_comment_);
                    state = START;
                }
                else if (input.substr(j, 1) == "\n"){
                    *error_line = *error_line + 1;
                    j++;
                }                 
                else {
                    j++;
                }
                break;
            case IN_MULTILINE_COMMENT_END:
                if (input.substr(j, 1) == "/"){
                    state = START;
                    tokens[i++] = Token("COMMENT", " ", *error_line, k, TokenType::_comment_);
                    token = "";
                    tokenType = "";
                    j++;
                }
                else if ( j == input.length() - 1){
                    tokens[i++] = Token("COMMENT", " ", *error_line, k, TokenType::_comment_);
                    state = START;
                }
                else {
                    state = IN_MULTILINE_COMMENT;
                }
                break;
            case IN_INLINE_COMMENT:
                if (input.substr(j, 1) == "\n"){
                    state = START;
                    error_line++;
                    tokens[i++] = Token("COMMENT", " ", *error_line, k, TokenType::_comment_);
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