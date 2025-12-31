#pragma once

#include "../lexer/lexer.hpp"
#include "../utils/include.hpp"
#include "../base_definitions/ast.hpp"

struct BindingPower {
	u32 left;
	u32 right;
	BindingPower(u32 left, u32 right) {
		this->left = left;
		this->right = right;
	}
};

const int MAX_LOOKAHEAD = 1;

Token* lookahead(int t);
void consume(TokenType type);
//void match(TokenType type, string lexeme);

Module* parse(Lexer* lx);

Expr* parseExpression(int minbp = 0);
Expr* parseAtom();
BindingPower infixBindingPower(TokenType op);
BindingPower prefixBindingPower(TokenType op);
BindingPower postfixBindingPower(TokenType op);