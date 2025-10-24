#pragma once

#include <iostream>
#include <string>
#include "../compiler/compiler.h"

using namespace std;

string typeToString(TokenType);
Token* newToken(string lexeme, TokenType type, Lexer* lx);
void deleteToken(Token*);

void printToken(Token*);