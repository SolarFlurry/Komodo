#include "help.h"

string typeToString(TokenType type) {
	switch (type) {
		case TOK_INT: return "INTEGER";
		case TOK_STRING: return "STRING";
		case TOK_ID: return "IDENTIFIER";
		case TOK_KEYWORD: return "KEYWORD";
		case TOK_PLUS: return "PLUS";
		case TOK_MINUS: return "MINUS";
		case TOK_L_PAREN: return "L_PAREN";
		case TOK_R_PAREN: return "RightParen";
		case TOK_L_BRACE: return "LeftBrace";
		case TOK_R_BRACE: return "RightBrace";
		case TOK_SEMICOLON: return "Semicolon";
		case TOK_AT: return "At";
		case TOK_COMMA: return "Comma";
		case TOK_COLON: return "Colon";
		case TOK_EOF: return "EOF";
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

void printToken (Token* tok) {
	if (tok == nullptr) {
		cout << "<\"NULL\">";
		return;
	}
    cout << "<\x1b[0;33m\"" << tok->lexeme << "\"\x1b[0m, \x1b[0;32m" << typeToString(tok->type) << "\x1b[0m, Line \x1b[0;36m" << tok->line << "\x1b[0m>\n";
}

bool isTypeKeyword (Token* tok) {
	return tok->type == TOK_KEYWORD && tok->lexeme == "int";
}