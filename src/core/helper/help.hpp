#pragma once

#include <iostream>
#include <string>
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

using std::string;

string typeToString(TokenType);
Token* newToken(string lexeme, TokenType type, Lexer* lx);
void deleteToken(Token*);

void printToken(Token*);

ASTNode* newNode(Token* tok);

void deleteAST(ASTNode* ast);

void printAST(ASTNode* ast, int indent);