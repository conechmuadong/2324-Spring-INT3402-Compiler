#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "scanner.h"

typedef enum
{
    P,
    _P,
    C,
    SL,
    S,
    BS,
    DS,
    CS,
    E,
    K,
    T,
    F,
    IF,
    DW,
    DStail,
    IFtail,
    _E,
    _K,
    _T,

    _begin,
    _end,
    cmt,
    type,
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
    _comment,
    _epsilon
} NodeType;

class Node
{
private:
    NodeType type;

    public:
        vector<Node> *child;
        Node(){
            this->child = new vector<Node>();
        }
        Node(NodeType type){
            this->type = type;
            this->child = new vector<Node>();
        }
        void setChild(vector<Node> * child){
            this->child = child;
        }
        void setType(NodeType type){
            this->type = type;
        }
        vector<Node>* getChild(){
            return this->child;
        }
        NodeType getType(){
            return this->type;
        }
};

bool parser(Token tokens[], Node &root);
void printParserTree(Node *node, std::string filename);

Node *P_Node(Token tokens[], bool *is_error);
Node *SL_Node(Token tokens[], bool *is_error);
Node *S_Node(Token tokens[], bool *is_error);
Node *BS_Node(Token tokens[], bool *is_error);
Node *DS_Node(Token tokens[], bool *is_error);
Node *CS_Node(Token tokens[], bool *is_error);

Node *E_Node(Token tokens[], bool *is_error);
Node *K_Node(Token tokens[], bool *is_error);
Node *T_Node(Token tokens[], bool *is_error);
Node *F_Node(Token tokens[], bool *is_error);
Node *IF_Node(Token tokens[], bool *is_error);
Node *DW_Node(Token tokens[], bool *is_error);
Node *DStail_Node(Token tokens[], bool *is_error);
Node *IFtail_Node(Token tokens[], bool *is_error);

Node *_E_Node(Token tokens[], bool *is_error);
Node *_K_Node(Token tokens[], bool *is_error);
Node *_T_Node(Token tokens[], bool *is_error);

// void saveParserTree();


#endif