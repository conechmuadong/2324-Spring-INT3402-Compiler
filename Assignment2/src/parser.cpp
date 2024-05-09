#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "parser.h"

using namespace std;

int index = 0;

string nodeType[]{
    "P",
    "P'",
    "C",
    "SL",
    "S",
    "BS",
    "DS",
    "CS",
    "E",
    "K",
    "T",
    "F",
    "IF",
    "DW",
    "DStail",
    "IFtail",
    "E'",
    "K'",
    "T'",
    "begin",
    "end",
    "cmt",
    "type",
    "id",
    "number",
    "assign",
    ";",
    "{",
    "}",
    "(",
    ")",
    "+",
    "*",
    "rel_op",
    "print",
    "if",
    "else",
    "while",
    "do",
    "then",
    "comment",
    "ε"
};

string ErrorMessage[]{
    "program must start with \"begin\"",
    "program must end with \"end\"",
    "expected a ;",
    "expected a {",
    "expected a }",
    "expected a (",
    "expected a )",
    "can't resolve the symbol",
    "expected an identifier",
    "expected an =",
    "expected an expression",
    "",
    "user-defined literal operator not found",
    "if statement must contain \"then\" after condition",
    "do-while statement must contain \"while\" in front of condition",
    "",
};

vector<Error> errors = vector<Error>();

bool parser(Token tokens[], Node &node){
    // node = new Node(P);
    bool is_error = false;
    node = *P_Node(tokens, &is_error);
    return !is_error;
}
void printRedString(const std::string& str) {
    cout << "\033[1;31m" << str << "\033[0m";
}

void printBoldString(const std::string& str) {
    cout << "\033[1m" << str << "\033[0m";
}

void printError(Error error, string filename){
    printBoldString(filename+":"+to_string(error.line)+":"+to_string(error.position)+":");
    printRedString(" error: ");
    if (error.errorType == ErrorType::UNDEFINED_SYMBOL){
        cout << "identifier \'"+error.token.getValue()+"\' is undefined"<<endl;
        return;
    }
    else if (error.errorType == REDEFINED_SYMBOL)
    {
        cout << "identifier \'"+error.token.getValue()+"\' has conflicting declaration"<<endl;
    }
    
    cout << ErrorMessage[error.errorType] << endl;
}

void error_anounce(string filename){
    for (auto error: errors){
        printError(error, filename);
    }
}

void add_error(ErrorType error, Token token){
    Error e = Error(error, token.getLine(), token.getPosition());
    if (ErrorType::UNDEFINED_SYMBOL == error){
        e.token = token;
    }
    errors.push_back(e);
    
}

Node *P_Node(Token token[], bool *is_error)
{
    // P -> CP'C
    Node *node = new Node(P);
    node->child->push_back(*C_Node(token, is_error));
    node->child->push_back(*_P_Node(token, is_error));
    node->child->push_back(*C_Node(token, is_error));
    return node;
}

Node *C_Node(Token tokens[], bool *is_error)
{
    //C -> cmt C | ε;
    Node *node = new Node(C);
    if (tokens[index].getTokenType() == _comment_)
    {
        index++;
        Node *temp = new Node(NodeType::_comment);
        node->child->push_back(*temp);
        Node *temp1 = C_Node(tokens, is_error);
        node->child->push_back(*temp1);
    }
    else{
        Node *temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);
    }
    return node;
}

Node *_P_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal P
    //  P' -> begin SL end
    //  Next token = ?
    //  If next token is begin -> SL_Node(tokens)
    //  If next token is end -> return node

    Node *node = new Node(_P);
    

    cout << "P" << endl;
    if (tokens[index].getTokenType() == _begin_)
    {
        index++;
        Node *temp = new Node(NodeType::_begin);
        node->child->push_back(*temp);

        Node *temp1 = SL_Node(tokens, is_error);
        node->child->push_back(*temp1);

        if (tokens[index].getTokenType() == _end_)
        if (tokens[index].getTokenType() == _end_)
        {
            Node *temp2 = new Node(NodeType::_end);
            node->child->push_back(*temp2);
            index++;
        }
        else
        {
            add_error(ErrorType::EXPECTED_END, tokens[index]);
            *is_error = true;   
        }
    }
    else
    {
       add_error(ErrorType::EXPECTED_BEGIN, tokens[index]); 
       *is_error = true;
    }
    return node;
}

Node *SL_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal SL
    //  SL -> S ; SL | BS SL | cmt SL | epsilon
    cout << "SL: " << tokens[index].getType()<<","<<tokens[index].getValue()<< endl;
    Node *node = new Node(SL);
    
    if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _type_ || tokens[index].getTokenType() == _print_)
    {
        Node *temp = S_Node(tokens, is_error);
        node->child->push_back(*temp);
        if (tokens[index].getTokenType() == _semicolon_)
        {
            index++;
            // add semicolon & SL nodes
            Node *temp1 = new Node(NodeType::semicolon);
            node->child->push_back(*temp1);

            Node * temp2 = SL_Node(tokens, is_error);
            node->child->push_back(*temp2);
        }
        else
        {
            cout << "Error: " << tokens[index].getType() << endl;
            add_error(ErrorType::EXPECTED_SEMI, tokens[index]);
            *is_error = true;
        }
    }
    else if (tokens[index].getTokenType() == _if_ || tokens[index].getTokenType() == _do_)
    {
        index++;
        // add BS SL node to child
        Node *temp = BS_Node(tokens, is_error);
        node->child->push_back(*temp);

        Node *temp1 = SL_Node(tokens, is_error);
        node->child->push_back(*temp1);    
    }
    else if (tokens[index].getTokenType() == _comment_)
    {
        index++;
        // add cmt and SL to child
        Node *temp = new Node(NodeType::cmt);
        node->child->push_back(*temp);

        Node *temp1 = SL_Node(tokens, is_error);
        node->child->push_back(*temp1);
    }
    // SL -> e
    else if (tokens[index].getTokenType() == _end_ || tokens[index].getTokenType() == _brace_close_)
    {
        Node * temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::UNDEFINED_STATEMENT, tokens[index]);
        *is_error = true;
    }
    return node;
}

Node *S_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal S
    //  S -> DS | CS

    Node *node = new Node(S);

    if (tokens[index].getTokenType() == _type_)
    {
        node->child->push_back(*DS_Node(tokens, is_error));
    }
    else if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _print_)
    {
        node->child->push_back(*CS_Node(tokens, is_error));
    }
    return node;
}

Node *BS_Node(Token tokens[], bool *is_error)
{
    Node *node = new Node(BS);
    
    if (tokens[index].getTokenType()==TokenType::_if_){
        node->child->push_back(*IF_Node(tokens, is_error));
    }
    else if (tokens[index].getTokenType()==TokenType::_do_){
        node->child->push_back(*DW_Node(tokens, is_error));
    } 
    return node;
}

Node *DS_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal DS
    //  DS -> type id DStail

    Node *node = new Node(DS);
    cout << "DS: "<<tokens[index].getType() << endl;
    if (tokens[index].getTokenType() == _type_)
    {
        index++;
        Node *temp = new Node(NodeType::type);
        node->child->push_back(*temp);

        if (tokens[index].getTokenType() == _id_)
        {
            Node *temp1 = new Node(NodeType::id);
            node->child->push_back(*temp1);
            cout << tokens[index].getType() << endl;
            if (checkDefine(tokens[index], tokens, index)){
                add_error(ErrorType::REDEFINED_SYMBOL, tokens[index]);
                *is_error = true;
            }
            index++;
            node->child->push_back(*DStail_Node(tokens, is_error));
            
        }
        else
        {
            add_error(ErrorType::EXPECTED_ID, tokens[index]);
            *is_error = true;
        }
    }    
    return node;
}

Node *CS_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal CS
    //  CS -> id = E | print (E)

    Node *node = new Node(CS);
    cout << "CS: "<<tokens[index].getType() << endl;
    if (tokens[index].getTokenType() == _id_)
    {
        cout << tokens[index].getValue() << endl;
        Node *temp = new Node(NodeType::id);
        if (!checkDefine(tokens[index], tokens, index)){
            add_error(ErrorType::UNDEFINED_SYMBOL, tokens[index]);
            *is_error = true;
        }
        node->child->push_back(*temp);
        index++;
        if (tokens[index].getTokenType() == _assign_)
        {
            index++;
            Node *temp1 = new Node(NodeType::assign);
            node->child->push_back(*temp1);
            

            Node *temp = E_Node(tokens, is_error);
            node->child->push_back(*temp);
            
        }
        else
        {
            add_error(ErrorType::EXPECTED_ASSIGN, tokens[index]);
            *is_error = true;
        }
    }
    else if (tokens[index].getTokenType() == _print_)
    {
        index++;
        Node *temp = new Node(NodeType::_print);
        node->child->push_back(*temp);

        if (tokens[index].getTokenType() == _parenthesis_open_)
        {
            index++;
            Node *temp1 = new Node(NodeType::parenthesis_open);
            node->child->push_back(*temp1);
            

            Node *temp2 = E_Node(tokens, is_error);
            node->child->push_back(*temp2);
            

            if (tokens[index].getTokenType() == _parenthesis_close_)
            {
                index++;
                Node *temp3 = new Node(NodeType::parenthesis_close);
                node->child->push_back(*temp3);
                
            }
            else
            {
                add_error(ErrorType::EXPECTED_PAREN_CLOSE, tokens[index]);
                *is_error = true;
            }
        }
        else
        {
            add_error(ErrorType::EXPECTED_PAREN_OPEN, tokens[index]);
            *is_error = true;
        }
    }
    return node;
}

Node *E_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal E
    //  E -> KE'

    Node *node = new Node(E);
    
    int child_id = 0;

    if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _number_ || tokens[index].getTokenType() == _parenthesis_open_)
    {
        Node *temp = K_Node(tokens, is_error);
        node->child->push_back(*temp);

        Node *temp1 = _E_Node(tokens, is_error);
        node->child->push_back(*temp1);
    }
    else{
        add_error(ErrorType::EXPECTED_EXPRESSION, tokens[index]);
        *is_error = true;
    }
    return node;
}

Node *K_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal K
    //  K -> TK'

    Node *node = new Node(K);

    if (tokens[index].getTokenType() == _id_ || tokens[index].getTokenType() == _number_ || tokens[index].getTokenType() == _parenthesis_open_)
    {
        Node *temp = T_Node(tokens, is_error);
        node->child->push_back(*temp);

        Node *temp1 = _K_Node(tokens, is_error);
        node->child->push_back(*temp1);
        
    }    
    return node;
}

Node *T_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal T
    // T -> FT'
    Node *node = new Node(T);
    
    if (tokens[index].getTokenType()==_id_ || tokens[index].getTokenType()==_number_ || tokens[index].getTokenType()==_parenthesis_open_){
        node->child->push_back(*F_Node(tokens, is_error));
        node->child->push_back(*_T_Node(tokens, is_error));
    }
    return node;
}

Node *F_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal F
    // F-> id | number | (E) 
    Node *node = new Node(F);
    
    if (tokens[index].getTokenType()==_id_){
        Node * temp = new Node(NodeType::id);
        node->child->push_back(*temp);
        if (!checkDefine(tokens[index], tokens, index)){
            add_error(ErrorType::UNDEFINED_SYMBOL, tokens[index]);
            *is_error = true;
        }
        index++;
    }
    else if (tokens[index].getTokenType()==_number_){
        index++;
        Node * temp = new Node(NodeType::number);
        node->child->push_back(*temp);
        return node;
    }
    else if (tokens[index].getTokenType()==_parenthesis_open_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        node->child->push_back(*temp);
        node->child->push_back(*E_Node(tokens, is_error));
        if (tokens[index].getTokenType()==_parenthesis_close_){
            index++;
            Node * temp = new Node(NodeType::parenthesis_close);
            node->child->push_back(*temp);
        }
        else
        {
            add_error(ErrorType::EXPECTED_PAREN_CLOSE, tokens[index]);
            *is_error = true;
        }
    }
    return node;
}

Node *_E_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal E'
    //E' -> e | rel_op KE'
    Node *node = new Node(_E);
    
    int child_index = 0;
    if (tokens[index].getTokenType()==_rel_op_){
        index++;
        Node * temp = new Node(NodeType::rel_op);
        node->child->push_back(*temp);
        node->child->push_back(*K_Node(tokens, is_error));
        node->child->push_back(*_E_Node(tokens, is_error));
    }
    // E' -> e
    else if (tokens[index].getTokenType()==_semicolon_||tokens[index].getTokenType()==_parenthesis_close_
            ||tokens[index].getTokenType()==_add_){
        Node * temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);
        return node;
    }
    else{
        add_error(ErrorType::UNDEFINED_LITERAL, tokens[index]);
        *is_error = true;
    }
    return node;
}

Node *_K_Node(Token tokens[], bool *is_error)
{
    // TODO: Implement generation rules for non-terminal K'
    // K' -> e | +TK'
    Node *node = new Node(_K);
    
    int child_index = 0;
    if (tokens[index].getTokenType()==_add_){
        index++;
        Node * temp = new Node(NodeType::add);
        node->child->push_back(*temp);
        node->child->push_back(*T_Node(tokens, is_error));
        node->child->push_back(*_K_Node(tokens, is_error));
    }
    // K' -> e
    else if (tokens[index].getTokenType()==_semicolon_||tokens[index].getTokenType()==_parenthesis_close_
            ||tokens[index].getTokenType()==_rel_op_){
        Node * temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);
        return node;
    }
    else
    {
        add_error(ErrorType::UNDEFINED_LITERAL, tokens[index]);
        *is_error = true;
    }
    
    return node;
}

Node *_T_Node(Token tokens[], bool *is_error){
    //TODO: Implement generation rules for non-terminal T'
    // T' -> e | *FT'
    Node *node = new Node(_T);
    
    int child_index = 0;
    if (tokens[index].getTokenType()==_mult_){
        index++;
        Node * temp = new Node(NodeType::mult);
        node->child->push_back(*temp);
        node->child->push_back(*F_Node(tokens, is_error));
        node->child->push_back(*_T_Node(tokens, is_error));
    }
    // T' -> e
    else if (tokens[index].getTokenType()==_semicolon_||tokens[index].getTokenType()==_parenthesis_close_
            ||tokens[index].getTokenType()==_rel_op_ || tokens[index].getTokenType()==_add_){
        Node * temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);;
        return node;
    }
    else
    {
        add_error(ErrorType::UNDEFINED_LITERAL, tokens[index]);
        *is_error = true;
    }
    
    return node;
}

Node *IF_Node(Token tokens[], bool *is_error){
    // IF -> if (E) then {SL} IFtail
    Node *node = new Node(IF);
    node->child->push_back(* new Node(NodeType::_if));
    index++;
    if (tokens[index].getTokenType()==_parenthesis_open_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_PAREN_OPEN, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*E_Node(tokens, is_error));
    if (tokens[index].getTokenType()==_parenthesis_close_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_close);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_PAREN_CLOSE, tokens[index]);
        *is_error = true;
    }
    if (tokens[index].getTokenType()==_then_){
        index++;
        Node * temp = new Node(NodeType::_then);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_THEN, tokens[index]);
        *is_error = true;
    }
    if (tokens[index].getTokenType()==_brace_open_){
        index++;
        Node * temp = new Node(NodeType::brace_open);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_BRACKET_OPEN, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*SL_Node(tokens, is_error));
    if (tokens[index].getTokenType()==_brace_close_){
        index++;
        Node * temp = new Node(NodeType::brace_close);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_BRACKET_CLOSE, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*IFtail_Node(tokens, is_error));
    
    return node;
}

Node *DW_Node(Token tokens[], bool *is_error){
    // DW -> do SL while (E);
    Node *node = new Node(DW);
    
    Node * temp = new Node(NodeType::_do);
    node->child->push_back(*temp);
    if (tokens[index].getTokenType()==_brace_open_){
        index++;
        Node * temp = new Node(NodeType::brace_open);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_BRACKET_OPEN, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*SL_Node(tokens, is_error));
    if (tokens[index].getTokenType()==_brace_close_){
        index++;
        Node * temp = new Node(NodeType::brace_close);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_BRACKET_CLOSE, tokens[index]);
        *is_error = true;
    }
    if (tokens[index].getTokenType()==_while_){
        index++;
        Node * temp = new Node(NodeType::_while);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_WHILE, tokens[index]);
        *is_error = true;
    }
    if (tokens[index].getTokenType()==_parenthesis_open_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_open);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_PAREN_OPEN, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*E_Node(tokens, is_error));
    if (tokens[index].getTokenType()==_parenthesis_close_){
        index++;
        Node * temp = new Node(NodeType::parenthesis_close);
        node->child->push_back(*temp);
    }
    else
    {
        add_error(ErrorType::EXPECTED_PAREN_CLOSE, tokens[index]);
        *is_error = true;
    }
    
    return node;
}

Node *DStail_Node(Token tokens[], bool *is_error){
    //DSTail -> e | = E
    Node *node = new Node(DStail);
    
    // cout << "DStail: "<<tokens[index].getType() << endl;

    if (tokens[index].getTokenType()==_assign_){
        index++;
        Node * temp = new Node(NodeType::assign);
        node->child->push_back(*temp);
    }
    // DSTail -> e
    else if (tokens[index].getTokenType()==_semicolon_){
        Node * temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);
        return node;
    }
    else
    {
        // cout << "Error: " << tokens[index].getType() << endl;
        add_error(ErrorType::EXPECTED_SEMI, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*E_Node(tokens, is_error));
    return node;
}

Node *IFtail_Node(Token tokens[], bool *is_error){
    //IFtail -> e | else SL
    Node *node = new Node(IFtail);
    
    if (tokens[index].getTokenType()==_else_){
        index++;
        Node * temp = new Node(NodeType::_else);
        node->child->push_back(*temp);
    }
    // IFtail -> e
    else if (tokens[index].getTokenType()==_end_ || tokens[index].getTokenType()==_comment_
            || tokens[index].getTokenType()==_type_ || tokens[index].getTokenType()==_id_|| tokens[index].getTokenType()==_print_
            || tokens[index].getTokenType()==_do_ || tokens[index].getTokenType()==_if_){
        Node * temp = new Node(NodeType::_epsilon);
        node->child->push_back(*temp);
        return node;
    }
    else
    {
        add_error(ErrorType::UNDEFINED_LITERAL, tokens[index]);
        *is_error = true;
    }
    node->child->push_back(*SL_Node(tokens, is_error));
    
    return node;
}

string parser_tree(Node *node)
{
    string output = "";
    cout << nodeType[node->getType()] << endl;
    // vector<Node> *child = node->getChild();
    for (int i = 0; i < node->child->size(); i++)
    {
        output += nodeType[node->getType()] + " -> " + nodeType[node->child->at(i).getType()] + "\n";
        output += parser_tree(&node->child->at(i));
    }
    return output;
}

void printParserTree(Node *node, string filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to create the output file: " << filename << endl;
    }
    cout << "Printing parser tree to file: " << filename << endl;
    file << parser_tree(node);
    file.close();
}
