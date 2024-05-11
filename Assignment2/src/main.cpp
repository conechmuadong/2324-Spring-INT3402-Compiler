#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << argv[0] <<" expected 2 arguments but receive "<< argc << "\n" << "Usage: ./parser INPUT SYMBOL_FILE <PARSER_TREE> \n \t - INPUT: PATH to the input file \n \t -SYMBOL_FILE: PATH to the token file. \n \t -PARSER_TREE: <optional> PATH to the file which saves generated parser tree" << endl;
        return -1;
    }
    if (argc > 3) {
        cout << argv[0] <<" expected 2 arguments but receive "<< argc << "\n" << "Usage: ./parser INPUT OUTPUT \n \t - INPUT: PATH to the input file \n \t -OUTPUT: PATH to file the parser will save the result to." << endl;
        return -1;
    }
    string filename = argv[1]; //"../example/input2.in";
    ifstream file(filename);
    string outputFilename = argv[2]; // "output.out";
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << endl;
        return -1;
    }

    string input;
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
    if (scanner(input, tokens, &error_pointer, &error_symbol, &error_type, &error_line)){
       for (int i = 0; i < 1000; i++){
           if (tokens[i].getType() == ""){
               break;
           }
           outputFile << "<" << tokens[i].getType() << ", " << tokens[i].getValue() << ">" << endl;
       } 
    }
    else {
        outputFile << "Error at line " << error_line << " at position " << error_pointer << ", can't resolve the symbol "<< error_symbol << endl;
        printBoldString(filename+":"+to_string(error_line)+":"+to_string(error_pointer)+":");
        printRedString("error: ");
        cout << "can't resolve the symbol ";
        printRedString(error_symbol);
        cout << endl;
        return -1;
    }
    outputFile.close();
    Node root = Node();
    bool is_warning = false;
    if(parser(tokens, root, &is_warning)){
        string parserTree = "";
        if (argc == 4) {
            parserTree = argv[3];
        }
        else{
            parserTree = "./out/parser_tree.txt";
        }
        printParserTree(&root, parserTree);
        if (is_warning){
           error_anounce(filename);
        }
        return 0;
    }
    else {
        error_anounce(filename);
        return -1;
    }
    
}