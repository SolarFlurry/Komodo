#pragma once

#include "../compiler/compiler.h"
#include <iostream>

void error(string message, int line, int col);
void fatalError(string message, int line, int col);
void printError(Token* error);
void printErrors();