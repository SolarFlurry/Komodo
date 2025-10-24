#pragma once

#include "../lexer/lexer.h"
#include <iostream>

using namespace std;

void error(string message, int line, int col);
void fatalError(string message, int line, int col);
void printError(Token* error);
void printErrors();
bool hasAnyErrors();