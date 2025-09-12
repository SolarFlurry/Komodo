#ifndef ERROR_H
#define ERROR_H

#include "token.h"

vector<Token*> errors;

void error(string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = TokenType::SyntaxError;
	errors.push_back(tok);
}

#endif // ERROR_H
