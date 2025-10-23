#pragma once

#include "compiler/compiler.h"
#include <iostream>

vector<Token*> errors;

void error(string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = TokenType::SyntaxError;
	errors.push_back(tok);
}

void fatalError(string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = SyntaxError;
	errors.push_back(tok);
	exit(0);
}
