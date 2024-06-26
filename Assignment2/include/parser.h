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

typedef enum{
    EXPECTED_BEGIN,
    EXPECTED_END,
    EXPECTED_SEMI,
    EXPECTED_BRACKET_OPEN,
    EXPECTED_BRACKET_CLOSE,
    EXPECTED_PAREN_OPEN,
    EXPECTED_PAREN_CLOSE,
    UNDEFINED_STATEMENT,
    EXPECTED_ID,
    EXPECTED_ASSIGN,
    EXPECTED_EXPRESSION,
    UNDEFINED_SYMBOL,
    UNDEFINED_LITERAL,
    EXPECTED_THEN,
    EXPECTED_WHILE,
    REDEFINED_SYMBOL,
    WARNNING_UNCOMPATIBLE_TYPE,
} ErrorType;

class Error
{
    public:
        ErrorType errorType;
        int line;
        int position;
        Token token;
        Error(ErrorType errorType, int line, int position)
        {
            this->errorType = errorType;
            this->line = line;
            this->position = position;
        }
};

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

void printRedString(const std::string& str);
void printBoldString(const std::string& str);
bool parser(Token tokens[], Node &root, bool *is_warning);
void printParserTree(Node *node, std::string filename);
void error_anounce(string filename);

Node *P_Node(Token tokens[], bool *is_error);
Node *_P_Node(Token tokens[], bool *is_error);
Node *C_Node(Token tokens[], bool *is_error);
Node *SL_Node(Token tokens[], bool *is_error);
Node *S_Node(Token tokens[], bool *is_error);
Node *BS_Node(Token tokens[], bool *is_error);
Node *DS_Node(Token tokens[], bool *is_error);
Node *CS_Node(Token tokens[], bool *is_error);

Node *E_Node(Token tokens[], bool *is_error, int idType);
Node *K_Node(Token tokens[], bool *is_error, int idType);
Node *T_Node(Token tokens[], bool *is_error, int idType);
Node *F_Node(Token tokens[], bool *is_error, int idType);
Node *IF_Node(Token tokens[], bool *is_error);
Node *DW_Node(Token tokens[], bool *is_error);
Node *DStail_Node(Token tokens[], bool *is_error);
Node *IFtail_Node(Token tokens[], bool *is_error);

Node *_E_Node(Token tokens[], bool *is_error, int idType);
Node *_K_Node(Token tokens[], bool *is_error, int idType);
Node *_T_Node(Token tokens[], bool *is_error, int idType);

// void saveParserTree();
#endif