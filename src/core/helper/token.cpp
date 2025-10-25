#include "help.h"

string typeToString(TokenType type) {
	switch (type) {
		case TOK_INT: return "INT";
		case TOK_STRING: return "STRING";
		case TOK_ID: return "ID";
		case TOK_KEYWORD_SCORE: return "KEYWORD_SCORE";
		case TOK_KEYWORD_CONST: return "KEYWORD_CONST";
		case TOK_KEYWORD_GLOB: return "KEYWORD_GLOB";
		case TOK_KEYWORD_IF: return "KEYWORD_IF";
		case TOK_PLUS: return "PLUS";
		case TOK_MINUS: return "MINUS";
		case TOK_ASTERISK: return "ASTERISK";
		case TOK_SLASH: return "SLASH";
		case TOK_LPAREN: return "L_PAREN";
		case TOK_RPAREN: return "R_PAREN";
		case TOK_LBRACE: return "LeftBrace";
		case TOK_RBRACE: return "RightBrace";
		case TOK_SEMICOLON: return "Semicolon";
		case TOK_AT: return "AT";
		case TOK_COMMA: return "COMMA";
		case TOK_COLON: return "COLON";
		case TOK_EOF: return "EOF";
		case SyntaxError: return "SyntaxError";
		default: return "UNKNOWN";
	}
}

Token* newToken(string lexeme, TokenType type, Lexer* lx) {
	Token* tok = new Token;
	tok->lexeme = lexeme;
	tok->type = type;
	tok->line = lx->line;
	tok->col = lx->col;
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
		std::cout << "<\"NULL\">";
		return;
	}
    std::cout << "<\x1b[0;33m\"" << tok->lexeme
		<< "\"\x1b[0m, \x1b[0;32m" << typeToString(tok->type)
		<< "\x1b[0m, \x1b[0;36m" << tok->line
		<< "\x1b[0m, \x1b[36m" << tok->col
	<< "\x1b[0m>\n";
}