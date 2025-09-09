#include "token.h"

void fail (unsigned int &state) {
	switch (state) {
		case 0: state = 1; return;
		case 1: state = 4; return;
		case 4: state = 7; return;
		case 7: state = 10; return;
		case 10: state = 12; return;
		default: state = 256;
	}
}

std::vector<Token*> tokenise(std::string program) {
	program += ' ';
	unsigned int i = 0;
	unsigned int line = 0;
	unsigned int state = 0;
	std::vector<Token*> tokList;
	std::string acc = "";

	while (i < program.length()) {
		switch (state) {
			case 0: { // starting
				if (isspace(program[i])) {
					if (program[i] == '\n') line++;
					i++;
				} else {
					fail(state);
				}
				break;
			}
			case 1: { // identifer
				if (isalpha(program[i])) {
					acc += program[i];
					i++;
					state = 2;
				} else {
					fail(state);
				}
				break;
			}
			case 2: {
				if (isalnum(program[i])) {
					acc += program[i];
					i++;
				} else {
					state = 3;
				}
				break;
			}
			case 3: {
				Token* tok = new Token;
				tok->lexeme = acc;
				tok->line = line;
				if (acc == "glob" || acc == "property" || acc == "const" || acc == "func" || acc == "import" || acc == "if") {
					tok->type = TokenType::Keyword;
				} else {
					tok->type = TokenType::Identifier;
				}
				tokList.push_back(tok);
				acc = "";
				state = 0;
				break;
			}
			case 4: { // number
				if (isdigit(program[i])) {
					acc += program[i];
					i++;
					state = 5;
				} else {
					fail(state);
				}
				break;
			}
			case 5: {
				if (isdigit(program[i])) {
					acc += program[i];
					i++;
				} else {
					state = 6;
				}
				break;
			}
			case 6: {
				Token* tok = new Token;
				tok->lexeme = acc;
				tok->line = line;
				tok->type = TokenType::Integer;
				tokList.push_back(tok);
				acc = "";
				state = 0;
				break;
			}
			case 7: { // string
				if (program[i] == '"') {
					state = 8;
					i++;
				} else {
					fail(state);
				}
				break;
			}
			case 8: {
				if (program[i] == '"') {
					state = 9;
				} else {
					acc += program[i];
					i++;
					if (i >= program.length()) {
						tokList.push_back(error("Unterminated string ", line));
					}
				}
				break;
			}
			case 9: {
				Token* tok = new Token;
				tok->lexeme = acc;
				tok->line = line;
				tok->type = TokenType::String;
				tokList.push_back(tok);
				acc = "";
				state = 0;
				i++;
				break;
			}
			case 10: { // +
				if (program[i] == '+' || program[i] == '-' || program[i] == '*') {
					acc += program[i];
					i++;
					state = 11;
				} else {
					fail(state);
				}
				break;
			}
			case 11: {
				Token* tok = new Token;
				tok->lexeme = acc;
				tok->line = line;
				tok->type = TokenType::BinaryOperator;
				tokList.push_back(tok);
				acc = "";
				state = 0;
				break;
			}
			case 12: {
				if (program[i] == '/') {
					acc += program[i];
					i++;
					state = 13;
				} else {
					fail(state);
				}
				break;
			}
			case 13: {
				if (program[i] == '/') {
					state = 14;
				} else {
					Token* tok = new Token;
					tok->lexeme = acc;
					tok->line = line;
					tok->type = TokenType::BinaryOperator;
					tokList.push_back(tok);
					acc = "";
					state = 0;
				}
				break;
			}
			case 14: {
				if (program[i] == '\n') {
					acc = "";
					state = 0;
				} else {
					i++;
				}
				break;
			}
			default: {
				std::string message = "Unexpected character '";
				message += program[i];
				message += '\'';
				tokList.push_back(error(message, line));
				i++;
				state = 0;
			}
		}
	}
	return tokList;
};