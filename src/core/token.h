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
	Semicolon,
	At,
	Comma,

	Eof,

	// Errors
	SyntaxError,

	// Parser Nodes
	Program,
	CommandStatement,
	ReturnStatement,
	ExecuteStatement,
	VarDeclaration,
	AssignStatement,
	FunctionCall,
	ParameterList,
	FunctionDeclaration,
	ComplexExpression,
	Variable
};

string typeToString(TokenType type) {
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
		case Semicolon: return "Semicolon";
		case At: return "At";
		case Comma: return "Comma";
		case SyntaxError: return "SyntaxError";
		case Program: return "Program";
		case CommandStatement: return "CommandStatement";
		case ReturnStatement: return "ReturnStatement";
		case ExecuteStatement: return "ExecuteStatement";
		case VarDeclaration: return "VarDeclaration";
		case AssignStatement: return "AssignStatement";
		case FunctionCall: return "FunctionCall";
		case FunctionDeclaration: return "FunctionDeclaration";
		case ComplexExpression: return "ComplexExpression";
		case Variable: return "Variable";
		default: return "Unknown";
	}
}

struct Token {
	string lexeme;
	TokenType type;
	unsigned int line;
	unsigned int lineIdx;
};

Token* newToken(string lexeme, TokenType type, unsigned int line) {
	Token* tok = new Token;
	tok->lexeme = lexeme;
	tok->type = type;
	tok->line = line;
	return tok;
}

Token* newToken(string lexeme, TokenType type, unsigned int line, unsigned int lineIdx) {
	Token* tok = new Token;
	tok->lexeme = lexeme;
	tok->type = type;
	tok->line = line;
	tok->lineIdx = lineIdx;
	return tok;
}

Token* newParseToken(TokenType type) {
	Token* tok = new Token;
	tok->type = type;
	return tok;
}

void deleteToken(Token* tok) {
	delete tok;
}

void printToken (struct Token* tok) {
    cout << "<\x1b[0;33m\"" << tok->lexeme << "\"\x1b[0m, \x1b[0;32m" << typeToString(tok->type) << "\x1b[0m, Line \x1b[0;36m" << tok->line << "\x1b[0m>\n";
}

bool isTypeKeyword (Token* tok) {
	return tok->type == Keyword && tok->lexeme == "int";
}

#endif //TOKEN_H