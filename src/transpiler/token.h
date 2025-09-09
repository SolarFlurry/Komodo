#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
	Integer,
	String,
	Identifier,

	Keyword,

	BinaryOperator,

	// Errors
	SyntaxError
};

std::string typeToString(TokenType type) {
	switch (type) {
		case Integer: return "Integer";
		case String: return "String";
		case Identifier: return "Identifier";
		case Keyword: return "Keyword";
		case BinaryOperator: return "BinaryOperator";
		case SyntaxError: return "SyntaxError";
		default: return "Unknown";
	}
}

struct Token {
	std::string lexeme;
	TokenType type;
	unsigned int line;
};

void printToken (struct Token* tok) {
    std::cout << "<\x1b[0;33m\"" << tok->lexeme << "\"\x1b[0m, \x1b[0;32m" << typeToString(tok->type) << "\x1b[0m, Line " << tok->line << ">\n";
}

Token* error(std::string message, int line) {
	Token* tok = new Token;
	tok->lexeme = message;
	tok->line = line;
	tok->type = TokenType::SyntaxError;
	return tok;
}

#endif //TOKEN_H