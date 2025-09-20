#ifndef ERROR_H
#define ERROR_H

#include "token.h"

vector<Token*> errors;

void printError(Token* error) {
	cout << "\x1b[31mError:\x1b[0m " << error->lexeme << "\n" << (error->line)+1 << " | " << lines[error->line] << "\n\n";
}

void printErrors() {
	for (Token* error : errors) {
		printError(error);
	}
}

void error(string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = TokenType::SyntaxError;
	errors.push_back(tok);
	printError(tok);
}

void fatalError(string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = SyntaxError;
	errors.push_back(tok);
	printError(tok);
	exit(0);
}

#endif // ERROR_H
