#pragma once

#include <iostream>
#include <string>
#include "../compiler/compiler.h"

using namespace std;

string typeToString(TokenType);
Token* newToken(string lexeme, TokenType type, unsigned int);
Token* newToken(string lexeme, TokenType type, unsigned int, unsigned int);
void deleteToken(Token*);

void printToken(Token*);