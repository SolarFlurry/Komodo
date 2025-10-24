#pragma once

#include "../lexer/lexer.h"

struct ASTNode {
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
void match(TokenType type, string lexeme);

ASTNode* parse(Lexer* lx);

ASTNode* parseExpression(int minbp = 0);
ASTNode* parseAtom();
tuple<int, int> infixBindingPower(TokenType op);