#include "help.h"

void printError(Token* error) {
	cout << "\x1b[31mError:\x1b[0m " << error->lexeme << "\n";
}

void printErrors(vector<Token*> errors) {
	for (Token* error : errors) {
		printError(error);
	}
}