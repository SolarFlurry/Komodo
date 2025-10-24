#include "compiler.h"
#include "../helper/help.h"
#include "../helper/error.h"

const SymbolEntry keywords[] = {
	{"score", TOK_KEYWORD_SCORE}, {"glob", TOK_KEYWORD_GLOB}, {"const", TOK_KEYWORD_CONST},
	{"if", TOK_KEYWORD_IF},
};

const SymbolEntry symbols[] = {
	{"+", TOK_PLUS}, {"-", TOK_MINUS}, {"*", TOK_ASTERISK}, {"/", TOK_SLASH}, {"%", TOK_PERCENT},
	{"=", TOK_EQ},
	{"==", TOK_EQ_EQ}, {">=", TOK_RARROW_EQ}, {"<=", TOK_LARROW_EQ},
	{"(", TOK_LPAREN}, {")", TOK_RPAREN},
	{"{", TOK_LBRACE}, {"}", TOK_RBRACE},
	{"[", TOK_LBRACK}, {"]", TOK_RBRACK},
	{"<", TOK_LARROW}, {">", TOK_RARROW},
	{";", TOK_SEMICOLON}, {":", TOK_COLON},
	{"@", TOK_AT}, {",", TOK_COMMA},
};

TokenType lookupSymbol(string val) {
	for (const auto entry : symbols) {
		if (entry.val == val) {
			return entry.type;
		}
	}
	return TOK_UNKNOWN;
}

TokenType lookupKeyword(string val) {
	for (const auto entry : keywords) {
		if (entry.val == val) {
			return entry.type;
		}
	}
	return TOK_ID;
}

void advance(Lexer* lx) {lx->idx++;lx->col++;}
bool isEnd(Lexer* lx) {return lx->idx >= lx->src.length();} 
void skipWhitespace(Lexer* lx) {
	while (isspace(lx->current()) && !isEnd(lx)) {
		if (lx->current() == '\n') {
			lx->line++;
			lx->col = 0;
		}
		advance(lx);
	}
}

Token* nextToken(Lexer* lx) {
	skipWhitespace(lx);
	if (isEnd(lx)) {
		return newToken("", TOK_EOF, lx);
	}
	string acc = "";
	if (isalpha(lx->current()) || lx->current() == '_') {
		acc += lx->current();
		advance(lx);
		while (!isEnd(lx) && (isalnum(lx->current()) || lx->current() == '_')) {
			acc += lx->current();
			advance(lx);
		}
		return newToken(acc, lookupKeyword(acc), lx);
	} else if (isdigit(lx->current())) {
		acc += lx->current();
		advance(lx);
		while (!isEnd(lx) && isdigit(lx->current())) {
			acc += lx->current();
			advance(lx);
		}
		return newToken(acc, TOK_INT, lx);
	}
	acc += lx->current();
	advance(lx);
	TokenType type = lookupSymbol(acc);
	if (type != TOK_UNKNOWN) {
		return newToken(acc, type, lx);
	} else if (isEnd(lx)) {
		stringstream ss;
		ss << "Unexpected character '" << acc[0] << "'";
		error(ss.str(), lx->line, lx->col);
		return newToken(acc, TOK_UNKNOWN, lx);
	} else {
		acc += lx->current();
		type = lookupSymbol(acc);
		if (type == TOK_UNKNOWN) {
			stringstream ss;
			ss << "Unexpected character '" << acc[0] << "'";
			error(ss.str(), lx->line, lx->col);
			// remove whitespace
			acc.erase(remove_if(acc.begin(), acc.end(), ::isspace), acc.end());
			
			return newToken(acc, TOK_UNKNOWN, lx);
		} else {
			return newToken(acc, type, lx);
		}
	}
};