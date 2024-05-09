#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "scanner.h"

typedef enum
{
    P,
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
    Node *child;
    NodeType type;

    public:
        Node(){
            this->child = NULL;
        }
        Node(NodeType type){
            this->type = type;
            this->child = NULL;
        }
        void setChild(Node *child){
            this->child = child;
        }
        void setType(NodeType type){
            this->type = type;
        }
        Node *getChild(){
            return this->child;
        }
        NodeType getType(){
            return this->type;
        }
};

void parser(Token tokens[]);
void printParserTree();

Node *P_Node(Token tokens[]);
Node *SL_Node(Token tokens[]);
Node *S_Node(Token tokens[]);
Node *BS_Node(Token tokens[]);
Node *DS_Node(Token tokens[]);
Node *CS_Node(Token tokens[]);

Node *E_Node(Token tokens[]);
Node *K_Node(Token tokens[]);
Node *T_Node(Token tokens[]);
Node *F_Node(Token tokens[]);
Node *IF_Node(Token tokens[]);
Node *DW_Node(Token tokens[]);
Node *DStail_Node(Token tokens[]);
Node *IFtail_Node(Token tokens[]);

Node *_E_Node(Token tokens[]);
Node *_K_Node(Token tokens[]);
Node *_T_Node(Token tokens[]);

void saveParserTree(Node *node, std::string filename);


#endif