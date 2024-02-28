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
    ERROR
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
};

/**
 * @def parser
 * @brief This function will parse the input string and return a list of tokens
 * @param input The input string to be parsed
*/
bool parser(string input, Token tokens[], int * error_pointer, string * error_symbol){
    int i, j = 0;
    int state = START;
    string token = "";
    string tokenType = "";
    while (j < input.length()){
        switch (state){
            case START:
                if (input.substr(j, 1) == " "){
                    j++;
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
                *error_pointer = j;
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
                    state = ERROR;
                }
                break;
            case IN_ID_2:
                if (regex_match(input.substr(j, 1), regex("[a-z0-9]+"))){
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
                }
                break;
            case IN_SPECIAL:
                state = START;
                if (input[j] == ';'){
                    tokens[i++] = Token("SEMI", " ");
                }
                else if (input[j] == '>'){
                    tokens[i++] = Token("REL_OP", " ");
                }
                else if (input[j] == '='){
                    tokens[i++] = Token("ASSIGN", " ");
                }
                else{
                    state = ERROR;
                }
                token = "";
                tokenType = "";
                j++;
                break;
            case IN_SYNTAX:
                if (regex_match(input.substr(j,1), regex("[;]+")) || input.substr(j, 1) == " "){
                    state = START;
                    if(token == "if")
                        tokens[i++] = Token("IF", " ");
                    else if (token == "then"){
                        tokens[i++] = Token("THEN", " ");
                    }
                    else if (token == "else"){
                        tokens[i++] = Token("ELSE", " ");
                    }
                    else 
                        state = ERROR;
                    token = "";
                    tokenType = "";
                }
                else if (regex_match(input.substr(j,1), regex("[a-z]+"))){
                    token = token.append(1, input[j]);
                    j++;
                }
                else {
                    state = ERROR;
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

    string filename = argv[1];
    ifstream file(filename);
    string outputFilename = argv[2];
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << endl;
        return -1;
    }

    string line[100];
    int i = 0;
    while (getline(file, line[i++])) {
    }

    file.close();

    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cout << "Failed to create the output file: " << outputFilename << endl;
    }
    
    Token tokens[1000];
    int error_pointer = 0;
    string error_symbol = "";
    for (int j = 0; j < i; j++){
        if (!parser(line[j], tokens, &error_pointer, &error_symbol)){
            outputFile << "Error at line " << j << " at position " << error_pointer << ", can't resolve the symbol "<< error_symbol << endl;
            return -1;
        }
    }
    for (int j = 0; j < 1000; j++){
        if (tokens[j].getType() == ""){
            break;
        }
        outputFile << "<" << tokens[j].getType() << ", " << tokens[j].getValue()<< ">;" << endl;
    }     
}