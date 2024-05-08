#include <iostream>
#include <string>
#include "parser.h"

using namespace std;

int index = 0;

typedef enum{
    ERROR_MISSING_BEGIN,
    ERROR_MISSING_END,
    ERROR_MISSING_SEMICOLON,
    ERROR_MISSING_ASSIGN,
    ERROR_MISSING_BRACE_OPEN,
    ERROR_MISSING_BRACE_CLOSE,
    ERROR_MISSING_PARENTHESIS_OPEN,
    ERROR_MISSING_PARENTHESIS_CLOSE,
    ERROR_MISSING_REL_OP,
    ERROR_UNDEFINE_SYMBOL,
    ERROR_REDEFINE_SYMBOL
} ErrorType;

void parser(Token tokens[]){
    Node *root = new Node(P);
    root = P_Node(tokens);
}

void error_anounce(ErrorType error, int line, int position){
    switch (error)
    {
    case ERROR_MISSING_BEGIN:
        /* code */
        break;
    
    default:
        break;
    }
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
    //BS -> DW | IF
    Node *root = new Node(BS);
    Node *child = (Node *)calloc(sizeof(Node), 1);
    if (tokens[index].getTokenType()==TokenType::_if){
        child = IF_Node(tokens);
    }
    else if (tokens[index].getTokenType()==TokenType::_do){
        child = DW_Node(tokens);
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
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
    // T -> FT'
    Node *root = new Node(T);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    if (tokens[index].getTokenType()==id || tokens[index].getTokenType()==number || tokens[index].getTokenType()==parenthesis_open){
        child[0] = *F_Node(tokens);
        child[1] = *_T_Node(tokens);
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *F_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal F
    // F-> id | number | (E) 
    Node *root = new Node(F);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==id){
        index++;
        Node * temp = new Node(NodeType::id);
        root->setChild(temp);
        return root;
    }
    else if (tokens[index].getTokenType()==number){
        index++;
        Node * temp = new Node(NodeType::number);
        root->setChild(temp);
        return root;
    }
    else if (tokens[index].getTokenType()==parenthesis_open){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        child[child_index++] = *temp;
        child[child_index++] = *E_Node(tokens);
        if (tokens[index].getTokenType()==parenthesis_close){
            index++;
            Node * temp = new Node(NodeType::parenthesis_close);
            child[child_index++] = *temp;
        }
        else{
            //TODO: Add error handling
        }
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *_E_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal E'
}

Node *_K_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal K'
    // K' -> e | +TK'
    Node *root = new Node(_K);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==add){
        index++;
        Node * temp = new Node(NodeType::add);
        child[child_index++] = *temp;
        child[child_index++] = *T_Node(tokens);
        child[child_index++] = *_K_Node(tokens);
    }
    // K' -> e
    else if (tokens[index].getTokenType()==semicolon||tokens[index].getTokenType()==parenthesis_close
            ||tokens[index].getTokenType()==rel_op){
        Node * temp = new Node(NodeType::_epsilon);
        root->setChild(temp);
        return root;
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *_T_Node(Token tokens[]){
    //TODO: Implement generation rules for non-terminal T'
    // T' -> e | *FT'
    Node *root = new Node(_T);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==mult){
        index++;
        Node * temp = new Node(NodeType::mult);
        child[child_index++] = *temp;
        child[child_index++] = *F_Node(tokens);
        child[child_index++] = *_T_Node(tokens);
    }
    // T' -> e
    else if (tokens[index].getTokenType()==semicolon||tokens[index].getTokenType()==parenthesis_close
            ||tokens[index].getTokenType()==rel_op || tokens[index].getTokenType()==add){
        Node * temp = new Node(NodeType::_epsilon);
        root->setChild(temp);
        return root;
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *IF_Node(Token tokens[]){
    // IF -> if (E) then SL IFtail
    Node *root = new Node(IF);
    Node *child = (Node *)calloc(sizeof(Node), 7);
    int child_index = 1;
    child = new Node(NodeType::_if);
    if (tokens[index].getTokenType()==parenthesis_open){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *E_Node(tokens);
    if (tokens[index].getTokenType()==parenthesis_close){
        index++;
        Node * temp = new Node(NodeType::parenthesis_close);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    if (tokens[index].getTokenType()==_then){
        index++;
        Node * temp = new Node(NodeType::_then);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *SL_Node(tokens);
    child[child_index++] = *IFtail_Node(tokens);
    root->setChild(child);
    return root;
}

Node *DW_Node(Token tokens[]){
    // DW -> do {SL} while (E);
    Node *root = new Node(DW);
    Node *child = (Node *)calloc(sizeof(Node), 9);
    int child_index = 1;
    index++;
    child = new Node(NodeType::_do);
    if (tokens[index].getTokenType()==brace_open){
        index++;
        Node * temp = new Node(NodeType::brace_open);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *SL_Node(tokens);
    if (tokens[index].getTokenType()==brace_close){
        index++;
        Node * temp = new Node(NodeType::brace_close);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    if (tokens[index].getTokenType()==_while){
        index++;
        Node * temp = new Node(NodeType::_while);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    if (tokens[index].getTokenType()==parenthesis_open){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *E_Node(tokens);
    if (tokens[index].getTokenType()==parenthesis_close){
        index++;
        Node * temp = new Node(NodeType::parenthesis_close);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *DStail_Node(Token tokens[]){
    //DSTail -> e | = E
    Node *root = new Node(DStail);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==assign){
        index++;
        Node * temp = new Node(NodeType::assign);
    }
    // DSTail -> e
    else if (tokens[index].getTokenType()==semicolon){
        Node * temp = new Node(NodeType::_epsilon);
        root->setChild(temp);
        return root;
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *E_Node(tokens);
    root->setChild(child);
    return root;
}

Node *IFtail_Node(Token tokens[]){
    //IFtail -> e | else SL
    Node *root = new Node(IFtail);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    int child_index = 0;
    if (tokens[index].getTokenType()==_else){
        index++;
        Node * temp = new Node(NodeType::_else);
        child[child_index++] = *temp;
    }
    // IFtail -> e
    else if (tokens[index].getTokenType()==semicolon){
        Node * temp = new Node(NodeType::_epsilon);
        root->setChild(temp);
        return root;
    }
    else{
    //TODO: Add error handling
    }
    child[child_index++] = *SL_Node(tokens);
    root->setChild(child);
    return root;
}
