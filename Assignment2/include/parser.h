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
    DWtail,
    _E,
    _K,
    _T,
    _F,

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
    _then
} NodeType;

class Node
{
private:
    Node *child;
    NodeType type;

public:
    Node(NodeType type)
    {
        this->type = type;
        this->child = NULL;
    }
    void setChild(Node *child)
    {
        this->child = child;
    }
    Node *getChild()
    {
        return this->child;
    }
    NodeType getType()
    {
        return this->type;
    }
};

void parser(Token tokens[]);
void printParserTree();

#endif