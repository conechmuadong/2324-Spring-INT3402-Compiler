#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"

using namespace std;

int index = 0;

void parser(Token tokens[]){
    Node *root = new Node(P);
    root = P_Node(tokens);
}

// void error_anounce(ErrorType error, int line, int position){
//     switch (error)
//     {
//     case ERROR_MISSING_BEGIN:
//         /* code */
//         break;
    
//     default:
//         break;
//     }
// }

Node *P_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal P
    //  P -> begin SL end
    //  Next token = ?
    //  If next token is begin -> SL_Node(tokens)
    //  If next token is end -> return root

    Node *node = new Node(P);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getTokenType() == _begin_)
    {
        index++;
        Node *temp = new Node(NodeType::_begin);
        child[child_id] = *temp;
        child_id++;

        Node *temp1 = new Node(NodeType::SL);
        child[child_id] = *temp1;
        child_id++;

        if (tokens[index].getTokenType() == _end_)
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
    return node;
}

Node *SL_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal SL
    //  SL -> S ; SL | BS SL | cmt SL | epsilon

    Node *node = new Node(SL);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;
    if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _type_ || tokens[index].getTokenType() == _print_)
    {
        index++;
        Node *temp = new Node(NodeType::S);
        child[child_id] = *temp;
        child_id++;
        if (tokens[index].getTokenType() == _semicolon_)
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
    else if (tokens[index].getTokenType() == _if_ || tokens[index].getTokenType() == _do_)
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
    else if (tokens[index].getTokenType() == _comment_)
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
    // SL -> e
    else if (tokens[index].getTokenType() == _end_ || tokens[index].getTokenType() == _brace_close_)
    {
        child[child_id] = *new Node(NodeType::_epsilon);
        return node;
    }
    else
    {
        // TODO : Error handling Error in SL
    }

    // add child to node
    node->setChild(child);
    return node;
}

Node *S_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal S
    //  S -> DS | CS

    Node *node = new Node(S);
    Node *child = (Node *)calloc(sizeof(Node), 1);

    if (tokens[index].getTokenType() == _type_)
    {
        index++;
        node->setChild(DS_Node(tokens));
    }
    else if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _print_)
    {
        index++;
        node->setChild(CS_Node(tokens));
    }
}

Node *BS_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal BS
    //BS -> DW | IF
    Node *root = new Node(BS);
    Node *child = (Node *)calloc(sizeof(Node), 1);
    if (tokens[index].getTokenType()==TokenType::_if_){
        child = IF_Node(tokens);
    }
    else if (tokens[index].getTokenType()==TokenType::_do_){
        child = DW_Node(tokens);
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *DS_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal DS
    //  DS -> type id DStail

    Node *node = new Node(DS);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getTokenType() == _type_)
    {
        index++;
        Node *temp = new Node(NodeType::type);
        child[child_id] = *temp;
        child_id++;

        if (tokens[index].getTokenType() == _id_)
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
    return node;
}

Node *CS_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal CS
    //  CS -> id = E | print (E)

    Node *node = new Node(CS);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_id = 0;

    if (tokens[index].getTokenType() == _id_)
    {
        index++;
        Node *temp = new Node(NodeType::id);
        child[child_id] = *temp;
        child_id++;

        if (tokens[index].getTokenType() == _assign_)
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
    else if (tokens[index].getTokenType() == _print_)
    {
        index++;
        Node *temp = new Node(NodeType::_print);
        child[child_id] = *temp;
        child_id++;

        if (tokens[index].getTokenType() == _parenthesis_open_)
        {
            index++;
            Node *temp1 = new Node(NodeType::parenthesis_open);
            child[child_id] = *temp1;
            child_id++;

            Node *temp2 = new Node(NodeType::E);
            child[child_id] = *temp2;
            child_id++;

            if (tokens[index].getTokenType() == _parenthesis_close_)
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
    return node;
}

Node *E_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal E
    //  E -> KE'

    Node *node = new Node(E);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    int child_id = 0;

    if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _number_ || tokens[index].getTokenType() == _parenthesis_open_)
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
    return node;
}

Node *K_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal K
    //  K -> TK'

    Node *node = new Node(K);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    int child_id = 0;

    if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _number_ || tokens[index].getTokenType() == _parenthesis_open_)
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
    return node;
}

Node *T_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal T
    // T -> FT'
    Node *root = new Node(T);
    Node *child = (Node *)calloc(sizeof(Node), 2);
    if (tokens[index].getTokenType()==_id_ || tokens[index].getTokenType()==_number_ || tokens[index].getTokenType()==_parenthesis_open_){
        child[0] = *F_Node(tokens);
        child[1] = *_T_Node(tokens);
    }
    else{
        //TODO: Add error handling
    }
    root->setChild(child);
    return root;
}

Node *F_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal F
    // F-> id | number | (E) 
    Node *root = new Node(F);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==_id_){
        index++;
        Node * temp = new Node(NodeType::id);
        root->setChild(temp);
        return root;
    }
    else if (tokens[index].getTokenType()==_number_){
        index++;
        Node * temp = new Node(NodeType::number);
        root->setChild(temp);
        return root;
    }
    else if (tokens[index].getTokenType()==_parenthesis_open_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        child[child_index++] = *temp;
        child[child_index++] = *E_Node(tokens);
        if (tokens[index].getTokenType()==_parenthesis_close_){
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

Node *_E_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal E'
    //E' -> e | rel_op KE'
    Node *root = new Node(_E);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==_rel_op_){
        index++;
        Node * temp = new Node(NodeType::rel_op);
        child[child_index++] = *temp;
        child[child_index++] = *K_Node(tokens);
        child[child_index++] = *_E_Node(tokens);
    }
    // E' -> e
    else if (tokens[index].getTokenType()==_semicolon_||tokens[index].getTokenType()==_parenthesis_close_
            ||tokens[index].getTokenType()==_add_){
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

Node *_K_Node(Token tokens[])
{
    // TODO: Implement generation rules for non-terminal K'
    // K' -> e | +TK'
    Node *root = new Node(_K);
    Node *child = (Node *)calloc(sizeof(Node), 3);
    int child_index = 0;
    if (tokens[index].getTokenType()==_add_){
        index++;
        Node * temp = new Node(NodeType::add);
        child[child_index++] = *temp;
        child[child_index++] = *T_Node(tokens);
        child[child_index++] = *_K_Node(tokens);
    }
    // K' -> e
    else if (tokens[index].getTokenType()==_semicolon_||tokens[index].getTokenType()==_parenthesis_close_
            ||tokens[index].getTokenType()==_rel_op_){
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
    if (tokens[index].getTokenType()==_mult_){
        index++;
        Node * temp = new Node(NodeType::mult);
        child[child_index++] = *temp;
        child[child_index++] = *F_Node(tokens);
        child[child_index++] = *_T_Node(tokens);
    }
    // T' -> e
    else if (tokens[index].getTokenType()==_semicolon_||tokens[index].getTokenType()==_parenthesis_close_
            ||tokens[index].getTokenType()==_rel_op_ || tokens[index].getTokenType()==_add_){
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
    if (tokens[index].getTokenType()==_parenthesis_open_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *E_Node(tokens);
    if (tokens[index].getTokenType()==_parenthesis_close_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_close);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    if (tokens[index].getTokenType()==_then_){
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
    if (tokens[index].getTokenType()==_brace_open_){
        index++;
        Node * temp = new Node(NodeType::brace_open);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *SL_Node(tokens);
    if (tokens[index].getTokenType()==_brace_close_){
        index++;
        Node * temp = new Node(NodeType::brace_close);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    if (tokens[index].getTokenType()==_while_){
        index++;
        Node * temp = new Node(NodeType::_while);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    if (tokens[index].getTokenType()==_parenthesis_open_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        child[child_index++] = *temp; 
    }
    else{
        //TODO: Add error handling
    }
    child[child_index++] = *E_Node(tokens);
    if (tokens[index].getTokenType()==_parenthesis_close_){
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
    if (tokens[index].getTokenType()==_assign_){
        index++;
        Node * temp = new Node(NodeType::assign);
    }
    // DSTail -> e
    else if (tokens[index].getTokenType()==_semicolon_){
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
    if (tokens[index].getTokenType()==_else_){
        index++;
        Node * temp = new Node(NodeType::_else);
        child[child_index++] = *temp;
    }
    // IFtail -> e
    else if (tokens[index].getTokenType()==_semicolon_){
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

string parser_tree(Node *root)
{
    string output = "";

    Node *child = root->getChild();
    if (child == NULL)
    {
        return output;
    }
    for (int i = 0; i < sizeof(child); i++)
    {
        Node *temp = &child[i];
        if (temp != NULL)
        {
            output += temp->getType();
            output += "\t";
        }
    }
    output += "\n";
    for (int i = 0; i < sizeof(child); i++)
    {
        Node *temp = &child[i];
        if (temp != NULL)
        {
            output += parser_tree(temp);
        }
    }
    return output;
}

void saveParserTree(Node *root, string filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to create the output file: " << filename << endl;
    }
    file << parser_tree(root);
    file.close();
}
