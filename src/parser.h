#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <queue>

#include "token.h"
#include "lexer.h"
#include "ast.h"

using namespace std;

class Parser
{
    private:
        Lexer lexer;
        Token currentToken;
        void eat(int tokenType);

        AST factor();
        AST term();
        AST expr();
    public:
        Parser(Lexer);
        void genAST();
};

Parser::Parser(Lexer lexer)
{
    this->lexer = lexer;
    this->currentToken = this->lexer.next();
}

void Parser::genAST()
{
    AST result = expr();
    cout << result.value.value << endl;
    while (result.right != NULL)
    {
        cout << "Visiting " << result.value.value << endl;
        result = *result.left;
    }
}

void Parser::eat(int tokenType)
{
    if (tokenType == currentToken.type) this->currentToken = this->lexer.next();
    else {cout << "error gobs" << endl; exit(1);}
}

AST Parser::factor()
{
    Token token = currentToken;

    if (token.type == T_OPR && token.value == "+")
    {
        eat(T_OPR);
        AST ast(token);
        ast.setLeft(factor());
        return ast;
    }
    if (token.type == T_OPR && token.value == "-")
    {
        eat(T_OPR);
        AST ast(token);
        ast.setLeft(factor());
        return ast;
    }
    if (token.type == T_INT)
    {
        eat(T_INT);
        return AST(token);
    }
    if (token.type == T_FLOAT)
    {
        eat(T_INT);
        return AST(token);
    }
    if (token.type == T_LPAREN)
    {
        eat(T_LPAREN);
        AST ast = expr();
        eat(T_RPAREN);
        return ast;
    }
}

AST Parser::term()
{
    AST node = factor();

    while (currentToken.type == T_OPR && (currentToken.value == "*" || currentToken.value == "/"))
    {
        Token token = currentToken;
        eat(T_OPR);
        AST newNode(token);
        newNode.setLeft(node);
        newNode.setRight(factor());
        node = newNode;
    }
    return node;
}

AST Parser::expr()
{
    cout << "Expr " << currentToken.value << endl;
    AST node = term();

    while (currentToken.type == T_OPR && (currentToken.value == "+" || currentToken.value == "-"))
    {
        Token token = currentToken;
        eat(T_OPR);
        AST newNode(token);
        newNode.setLeft(node);
        newNode.setRight(term());
        node = newNode;
    }
    return node;
}

#endif