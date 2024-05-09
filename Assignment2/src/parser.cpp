#include <iostream>
#include <string>
#include "parser.h"

using namespace std;

int index = 0;

void parser(Token tokens[])
{
    Node *root = new Node(P);

    for (int i = 0; i < 100; i++)
    {
        if (tokens[i].getType() == "begin")
        {
            index = i;
            break;
        }
    }

    if (index != 0)
    {
    }

    root = P_Node(tokens);
}

void error_anounce(string error, int line, int position)
{
    cout << error << endl;
}

Node *P_Node(Token token[])
{
    // TODO: Implement generation rules for non-terminal P
    // P -> CP'C

    Node *node = new Node(P);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getType
}

Node *_P_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal P
    //  P' -> begin SL end
    //  Next token = ?
    //  If next token is begin -> SL_Node(tokens)
    //  If next token is end -> return root

    Node *node = new Node(_P);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getType() == "_begin")
    {
        index++;
        Node *temp = new Node(NodeType::_begin);
        child[child_id] = *temp;
        child_id++;

        Node *temp1 = new Node(NodeType::SL);
        child[child_id] = *temp1;
        child_id++;

        if (tokens[index].getType() == "_end")
        {
            Node *temp2 = new Node(NodeType::_end);
            child[child_id] = *temp2;
            child_id++;
            index++;
            return node;
        }
        else
        {
            // TODO : Error handling missing end
        }
    }
    else
    {
        // TODO : Error handling missing begin
    }

    node->setChild(child);
}

Node *SL_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal SL
    //  SL -> S ; SL | BS SL | cmt SL | epsilon

    Node *node = new Node(SL);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;
    if (tokens[index].getType() == "id" || tokens[index].getType() == "type" || tokens[index].getType() == "_print")
    {
        index++;
        Node *temp = new Node(NodeType::S);
        child[child_id] = *temp;
        child_id++;
        if (tokens[index].getType() == "semicolon")
        {
            index++;
            // add semicolon & SL nodes
            Node *temp1 = new Node(NodeType::semicolon);
            child[child_id] = *temp1;
            child_id++;

            Node *temp2 = new Node(NodeType::SL);
            child[child_id] = *temp2;
            child_id++;
        }
        else
        {
            // TODO : Error handling missing semicolon
        }
    }
    else if (tokens[index].getType() == "_if" || tokens[index].getType() == "_do")
    {
        index++;
        // add BS SL node to child
        Node *temp = new Node(NodeType::BS);
        child[child_id] = *temp;
        child_id++;

        Node *temp1 = new Node(NodeType::SL);
        child[child_id] = *temp;
        child_id++;
    }
    else if (tokens[index].getType() == "cmt")
    {
        index++;
        // add cmt and SL to child
        Node *temp = new Node(NodeType::cmt);
        child[child_id] = *temp;
        child_id++;

        Node *temp1 = new Node(NodeType::SL);
        child[child_id] = *temp;
        child_id++;
    }
    else
    {
        return node;
    }

    // add child to node
    node->setChild(child);
}

Node *S_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal S
    //  S -> DS | CS

    Node *node = new Node(S);
    Node *child = (Node *)calloc(sizeof(Node), 1);

    if (tokens[index].getType() == "type")
    {
        index++;
        node->setChild(DS_Node(tokens));
    }
    else if (tokens[index].getType() == "id" || tokens[index].getType() == "_print")
    {
        index++;
        node->setChild(CS_Node(tokens));
    }
}

Node *BS_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal BS
    //  BS -> IF | DW

    Node *node = new Node(BS);
    Node *child = (Node *)calloc(sizeof(Node), 1);

    if (tokens[index].getType() == "_if")
    {
        index++;
        node->setChild(IF_Node(tokens));
    }
    else if (tokens[index].getType() == "_do")
    {
        index++;
        node->setChild(DW_Node(tokens));
    }
    else
    {
        // TODO : Error handling missing _if, _do
    }
}

Node *DS_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal DS
    //  DS -> type id DStail

    Node *node = new Node(DS);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getType() == "type")
    {
        index++;
        Node *temp = new Node(NodeType::type);
        child[child_id] = *temp;
        child_id++;

        if (tokens[index].getType() == "id")
        {
            index++;
            Node *temp1 = new Node(NodeType::id);
            child[child_id] = *temp1;
            child_id++;

            Node *temp2 = new Node(NodeType::DStail);
            child[child_id] = *temp2;
            child_id++;
        }
        else
        {
            // TODO : Error handling missing id
        }
    }
    else
    {
        // TODO : Error handling missing type
    }

    node->setChild(child);
}

Node *CS_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal CS
    //  CS -> id = E | print (E)

    Node *node = new Node(CS);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getType() == "id")
    {
        index++;
        Node *temp = new Node(NodeType::id);
        child[child_id] = *temp;
        child_id++;

        if (tokens[index].getType() == "assign")
        {
            index++;
            Node *temp1 = new Node(NodeType::assign);
            child[child_id] = *temp1;
            child_id++;

            Node *temp = new Node(NodeType::type);
            child[child_id] = *temp;
            child_id++;
        }
        else
        {
            // TODO: Error handling missing assign
        }
    }
    else if (tokens[index].getType() == "_print")
    {
        index++;
        Node *temp = new Node(NodeType::_print);
        child[child_id] = *temp;
        child_id++;

        if (tokens[index].getType() == "parenthesis_open")
        {
            index++;
            Node *temp1 = new Node(NodeType::parenthesis_open);
            child[child_id] = *temp1;
            child_id++;

            Node *temp2 = new Node(NodeType::E);
            child[child_id] = *temp2;
            child_id++;

            if (tokens[index].getType() == "parenthesis_close")
            {
                index++;
                Node *temp3 = new Node(NodeType::parenthesis_close);
                child[child_id] = *temp3;
                child_id++;
            }
            else
            {
                // TODO: Error handling missing parenthesis_close
            }
        }
        else
        {
            // TODO: Error handling missing parenthesis_open
        }
    }
    else
    {
        // TODO: Error handling missing id, _print
    }

    node->setChild(child);
}

Node *E_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal E
    //  E -> KE'

    Node *node = new Node(E);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    int child_id = 0;

    if (tokens[index].getType() == "id" || tokens[index].getType() == "number" || tokens[index].getType() == "parenthesis_open")
    {
        Node *temp = new Node(NodeType::K);
        child[child_id] = *temp;
        child_id++;

        Node *temp1 = new Node(NodeType::_E);
        child[child_id] = *temp1;
        child_id++;
    }
    else
    {
        // TODO: Error handling missing id, number, parenthesis_open
    }

    node->setChild(child);
}

Node *K_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal K
    //  K -> TK'

    Node *node = new Node(K);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    int child_id = 0;

    if (tokens[index].getType() == "id" || tokens[index].getType() == "number" || tokens[index].getType() == "parenthesis_open")
    {
        Node *temp = new Node(NodeType::T);
        child[child_id] = *temp;
        child_id++;

        Node *temp1 = new Node(NodeType::_K);
        child[child_id] = *temp1;
        child_id++;
    }
    else
    {
        // TODO: Error handling missing id, number, parenthesis_open
    }

    node->setChild(child);
}

Node *T_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal T
}

Node *F_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal F
}

Node *_E_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal E'
}

Node *_K_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal K'
}

Node *_T_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal T'
}

Node *_F_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal F'
}

Node *IF_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal IF
}

Node *DW_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal DW
}

Node *DStail_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal DStail
}

Node *IFtail_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal IFtail
}

Node *DWtail_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal DWtail
    // DStail -> = E | e

    // if next element is not a token
}
