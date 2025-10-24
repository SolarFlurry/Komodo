#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <tuple>

using namespace std;

enum TokenType {
	TOK_INT, TOK_STRING, TOK_ID,
	
	TOK_KEYWORD_SCORE, TOK_KEYWORD_GLOB, TOK_KEYWORD_CONST,
	TOK_KEYWORD_IF,

	TOK_PLUS, TOK_MINUS, TOK_ASTERISK, TOK_SLASH, TOK_PERCENT,
	TOK_EQ,
	TOK_EQ_EQ, TOK_RARROW_EQ, TOK_LARROW_EQ, TOK_BANG_EQ,
	TOK_LPAREN, TOK_RPAREN,
	TOK_LBRACE, TOK_RBRACE,
	TOK_LBRACK, TOK_RBRACK,
	TOK_LARROW, TOK_RARROW,
	TOK_SEMICOLON, TOK_COLON,
	TOK_AT,
	TOK_COMMA,

	TOK_EOF,
	TOK_UNKNOWN,

	SyntaxError,
};

struct Token {
	string lexeme;
	TokenType type;
	unsigned int line;
	unsigned int col;
};

// lexer
struct SymbolEntry {
	string val;
	TokenType type;
};

struct Lexer {
	string src;
	int idx;
	int col;
	int line;
	Lexer(string src) {
		this->src = src;
		this->idx = 0;
		this->col = 0;
		this->line = 0;
	}
	char current() {
		return src[idx];
	}
};

TokenType lookupSymbol(string val);
TokenType lookupKeyword(string val);
void advance(Lexer*);
void skipWhitespace(Lexer*);
bool isEnd(Lexer*);
Token* nextToken(Lexer*);

// parser


/*ASTNode* parseImportStmt();
ASTNode* parseStatementList();
ASTNode* parseStatement();
ASTNode* parseCmdStmt();
ASTNode* parseReturnStmt();
ASTNode* parseExecuteStmt();
ASTNode* parseVarDeclaration();
ASTNode* parseAssignStmt();
ASTNode* parseParameterList(int);
ASTNode* parseFunctionDeclaration();
ASTNode* parseNamespaceStmt();
ASTNode* parseExpressionList();
ASTNode* parseExpression();
ASTNode* parseTerm();
ASTNode* parseFactor(string);*/