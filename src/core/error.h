#ifndef ERROR_H
#define ERROR_H

#include "token.h"

vector<Token*> errors;

void printErrors() {
	for (Token* error : errors) {
		printToken(error);
	}
}

void error(string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = TokenType::SyntaxError;
	errors.push_back(tok);
}

void fatalError(string message) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = 0;
	tok->type = SyntaxError;
	errors.push_back(tok);
	printErrors();
	exit(0);
}

#endif // ERROR_H
