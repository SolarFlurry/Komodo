#pragma once

#include "../lexer/lexer.h"

typedef std::tuple<int, int> BindingPower;

enum NodeType {
	AST_PROGRAM,

	AST_EXPR_LITERAL,
	AST_EXPR_IDENTIFIER,
	AST_EXPR_BINARY,
	AST_EXPR_UNARY,
};

struct ASTNode {
	NodeType type;
	Token* content;
	ASTNode* sibling;
	ASTNode* firstChild;
	ASTNode () {
		content = nullptr;
		sibling = nullptr;
		firstChild = nullptr;
	}
};

const int MAX_LOOKAHEAD = 1;

Token* lookahead(int t);
void match(TokenType type);
//void match(TokenType type, string lexeme);

ASTNode* parse(Lexer* lx);

ASTNode* parseExpression(int minbp = 0);
ASTNode* parseAtom();
BindingPower infixBindingPower(TokenType op);
BindingPower prefixBindingPower(TokenType op);