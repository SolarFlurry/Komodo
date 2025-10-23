#ifndef LEXER_H
#define LEXER_H

#include "compiler.h"
#include "../helper/help.h"
#include "../error.h"

const string keywords[] = {
	"score", "glob", "const", "if"
};

const SymbolEntry symbols[] = {
	{"+", TOK_PLUS}, {"-", TOK_MINUS},
	{"(", TOK_L_PAREN}, {")", TOK_R_PAREN},
	{"{", TOK_L_BRACE}, {"}", TOK_R_BRACE},
};

TokenType lookupSymbol(string val, TokenType fallback) {
	for (const auto entry : symbols) {
		if (entry.val == val) {
			return entry.type;
		}
	}
	return fallback;
}

void advance(Lexer* lx) {lx->idx++;}
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
		return newToken("", TOK_EOF, lx->line);
	}
	string acc = "";
	if (isalpha(lx->current()) || lx->current() == '_') {
		acc += lx->current();
		advance(lx);
		while (!isEnd(lx) && (isalnum(lx->current()) || lx->current() == '_')) {
			acc += lx->current();
			advance(lx);
		}
		return newToken(acc, TOK_ID, lx->line, lx->col);
	}
	acc += lx->current();
	advance(lx);
	TokenType type = lookupSymbol(acc, TOK_UNKNOWN);
	if (type != TOK_UNKNOWN) {
		return newToken(acc, type, lx->line);
	} else if (isEnd(lx)) {
		error("Unexpected character", lx->line);
		return nullptr;
	} else {
		acc += lx->current();
		type = lookupSymbol(acc, TOK_UNKNOWN);
		if (type == TOK_UNKNOWN) {
			error("Unexpected character", lx->line);
			return nullptr;
		} else {
			return newToken(acc, type, lx->line);
		}
	}
};

#endif // LEXER_H