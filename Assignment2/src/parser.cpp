#include <iostream>
#include <string>
#include "parser.h"

using namespace std;

int index = 0;

void parser(Token tokens[]){
    Node *root = new Node(P);

    for (int i = 0; i < 100; i++){
        if (tokens[i].getType() == "begin"){
            index = i;
            break;
        }
    }

    if (index != 0){
        
    }

    root = P_Node(tokens);

}

void error_anounce(string error, int line, int position){
    cout << error << endl;
}


Node *P_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal P
    // P -> begin SL end
    // Next token = ? 
    // If next token is begin -> SL_Node(tokens)
    // If next token is end -> return root
}

Node *SL_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal SL
}

Node *S_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal S
}

Node *BS_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal BS
}

Node *DS_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal DS
}

Node *CS_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal CS
}

Node *E_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal E
}

Node *K_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal K
}

Node *T_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal T
}

Node *F_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal F
}

Node *_E_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal E'
}

Node *_K_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal K'
}

Node *_T_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal T'
}

Node *_F_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal F'
}

Node *IF_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal IF
}

Node *DW_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal DW
}

Node *DStail_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal DStail
}

Node *IFtail_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal IFtail
}

Node *DWtail_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal DWtail
}
