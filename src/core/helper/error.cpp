#include "error.h"

vector<Token*> errors;

void error(string message, int line, int col) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->col = col;
	tok->type = TokenType::SyntaxError;
	errors.push_back(tok);
}

void fatalError(string message, int line, int col) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->col = col;
	tok->type = SyntaxError;
	errors.push_back(tok);
	printErrors();
	exit(0);
}


void printError(Token* error) {
	cout << "\x1b[31mError:\x1b[0m " << error->lexeme << "\n";
}

void printErrors() {
	for (Token* error : errors) {
		printError(error);
	}
}

bool hasAnyErrors() {
	return errors.size() > 0;
}