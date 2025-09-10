#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
	// Basic
	Integer,
	String,
	Identifier,
	Keyword,

	// Symbols
	BinaryOperator,
	UnaryOperator,
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,

	// Errors
	SyntaxError,

	// Parser Nodes
	Program
};

std::string typeToString(TokenType type) {
	switch (type) {
		case Integer: return "Integer";
		case String: return "String";
		case Identifier: return "Identifier";
		case Keyword: return "Keyword";
		case BinaryOperator: return "BinaryOperator";
		case UnaryOperator: return "UnaryOperator";
		case LeftParen: return "LeftParen";
		case RightParen: return "RightParen";
		case LeftBrace: return "LeftBrace";
		case RightBrace: return "RightBrace";
		case SyntaxError: return "SyntaxError";
		default: return "Unknown";
	}
}

struct Token {
	std::string lexeme;
	TokenType type;
	unsigned int line;
};

Token* newToken(std::string lexeme, TokenType type, unsigned int line) {
	Token* tok = new Token();
	tok->lexeme = lexeme;
	tok->type = type;
	tok->line = line;
	return tok;
}

void deleteToken(Token* tok) {
	delete tok;
}

void printToken (struct Token* tok) {
    std::cout << "<\x1b[0;33m\"" << tok->lexeme << "\"\x1b[0m, \x1b[0;32m" << typeToString(tok->type) << "\x1b[0m, Line \x1b[0;36m" << tok->line << "\x1b[0m>\n";
}

Token* error(std::string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = TokenType::SyntaxError;
	return tok;
}

#endif //TOKEN_H