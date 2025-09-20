#ifndef LEXER_H
#define LEXER_H

#include "../token.h"
#include "../error.h"
#include "../symtable.h"

enum LexerState {
	Start,
	// Identifier
	IdStart,
	IdMid,
	IdEnd,
	// Number
	NumStart,
	NumMid,
	NumEnd,
	// String
	StrStart,
	StrMid,
	StrEnd,
	// Arithmetic operatoe
	OpStart,
	OpEnd,
	// Comment
	CommentStart,
	CommentMid,
	CommentEnd,
	// LParen
	LParenStart,
	LParenEnd,
	// RParen
	RParenStart,
	RParenEnd,
	// LBrace
	LBraceStart,
	LBraceEnd,
	// RBrace
	RBraceStart,
	RBraceEnd,
	// Semicolon
	SemicolonStart,
	SemicolonEnd,
	// At
	AtStart,
	AtEnd,
	// Comma
	CommaStart,
	CommaEnd,
	// ERROR
	Unknown
};

void fail (LexerState &state) {
	switch (state) {
		case Start: state = IdStart; return;
		case IdStart: state = NumStart; return;
		case NumStart: state = StrStart; return;
		case StrStart: state = OpStart; return;
		case OpStart: state = CommentStart; return;
		case CommentStart: state = LParenStart; return;
		case LParenStart: state = RParenStart; return;
		case RParenStart: state = LBraceStart; return;
		case LBraceStart: state = RBraceStart; return;
		case RBraceStart: state = SemicolonStart; return;
		case SemicolonStart: state = AtStart; return;
		case AtStart: state = CommaStart; return;
		default: state = Unknown;
	}
}

vector<Token*> tokenise(string program) {
	program += ' ';
	unsigned int i = 0;
	unsigned int line = 0;
	unsigned int lineIdx = 0;
	LexerState state = Start;
	vector<Token*> tokList;
	string acc = "";

	while (i < program.length()) {
		switch (state) {
			case Start: { // starting
				if (isspace(program[i])) {
					if (program[i] == '\n') {line++; lineIdx = 0;}
					i++;
				} else {
					fail(state);
				}
				break;
			}
			case IdStart: { // identifer
				if (isalpha(program[i])) {
					acc += program[i];
					i++;
					state = IdMid;
				} else {
					fail(state);
				}
				break;
			}
			case IdMid: {
				if (isalnum(program[i])) {
					acc += program[i];
					i++;
				} else {
					state = IdEnd;
				}
				break;
			}
			case IdEnd: {
				Token* tok = newToken(acc, TokenType::Identifier, line);
				if (acc == "glob" || acc == "score" || acc == "const" || acc == "func" ||
					acc == "import" || acc == "if" || acc == "as" || acc == "at" ||
					acc == "in" || acc == "return" || acc == "tick" || acc == "int") {
					tok->type = TokenType::Keyword;
				}
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case NumStart: { // number
				if (isdigit(program[i])) {
					acc += program[i];
					i++;
					state = NumMid;
				} else {
					fail(state);
				}
				break;
			}
			case NumMid: {
				if (isdigit(program[i])) {
					acc += program[i];
					i++;
				} else {
					state = NumEnd;
				}
				break;
			}
			case NumEnd: {
				Token* tok = newToken(acc, TokenType::Integer, line, i - lineIdx);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case StrStart: { // string
				if (program[i] == '"') {
					state = StrMid;
					i++;
				} else {
					fail(state);
				}
				break;
			}
			case StrMid: {
				if (program[i] == '"') {
					state = StrEnd;
				} else {
					if (program[i] == '\\') {
						i++;
						if (i >= program.length()) {
							error("Unterminated string ", line);
							break;
						}
					}
					acc += program[i];
					i++;
					if (i >= program.length()) {
						error("Unterminated string ", line);
					}
				}
				break;
			}
			case StrEnd: {
				Token* tok = newToken(acc, TokenType::String, line);
				tok->type = TokenType::String;
				tokList.push_back(tok);
				acc = "";
				state = Start;
				i++;
				break;
			}
			case OpStart: { // + - *
				switch (program[i]) {
					case '+': case '-': case '*': case '=': case ':':
						acc += program[i];
						i++;
						state = OpEnd;
						break;
					default:
						fail(state);
				}
				break;
			}
			case OpEnd: {
				Token* tok = newToken(acc, TokenType::BinaryOperator, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case CommentStart: { // '/' or comment
				if (program[i] == '/') {
					acc += program[i];
					i++;
					state = CommentMid;
				} else {
					fail(state);
				}
				break;
			}
			case CommentMid: {
				if (program[i] == '/') {
					state = CommentEnd;
				} else {
					Token* tok = newToken(acc, TokenType::BinaryOperator, line);
					tokList.push_back(tok);
					acc = "";
					state = Start;
				}
				break;
			}
			case CommentEnd: {
				if (program[i] == '\n') {
					acc = "";
					state = Start;
				} else {
					i++;
				}
				break;
			}
			case LParenStart: { // (
				if (program[i] == '(') {
					acc += program[i];
					i++;
					state = LParenEnd;
				} else {
					fail(state);
				}
				break;
			}
			case LParenEnd: {
				Token* tok = newToken(acc, TokenType::LeftParen, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case RParenStart: { // )
				if (program[i] == ')') {
					acc += program[i];
					i++;
					state = RParenEnd;
				} else {
					fail(state);
				}
				break;
			}
			case RParenEnd: {
				Token* tok = newToken(acc, TokenType::RightParen, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case LBraceStart: { // {
				if (program[i] == '{') {
					acc += program[i];
					i++;
					state = LBraceEnd;
				} else {
					fail(state);
				}
				break;
			}
			case LBraceEnd: {
				Token* tok = newToken(acc, TokenType::LeftBrace, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case RBraceStart: { // }
				if (program[i] == '}') {
					acc += program[i];
					i++;
					state = RBraceEnd;
				} else {
					fail(state);
				}
				break;
			}
			case RBraceEnd: {
				Token* tok = newToken(acc, TokenType::RightBrace, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case SemicolonStart: {
				if (program[i] == ';') {
					acc += program[i];
					i++;
					state = SemicolonEnd;
				} else {
					fail(state);
				}
				break;
			}
			case SemicolonEnd: {
				Token* tok = newToken(acc, Semicolon, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case AtStart: {
				if (program[i] == '@') {
					acc += program[i];
					i++;
					state = AtEnd;
				} else {
					fail(state);
				}
				break;
			}
			case AtEnd: {
				Token* tok = newToken(acc, At, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			case CommaStart: {
				if (program[i] == ',') {
					acc += program[i];
					i++;
					state = CommaEnd;
				} else {
					fail(state);
				}
				break;
			}
			case CommaEnd: {
				Token* tok = newToken(acc, Comma, line);
				tokList.push_back(tok);
				acc = "";
				state = Start;
				break;
			}
			default: {
				string message = "Unexpected character '";
				message += program[i];
				message += '\'';
				error(message, line);
				i++;
				state = Start;
			}
		}
	}
	tokList.push_back(newToken("EOF", Eof, line));
	return tokList;
};

#endif // LEXER_H