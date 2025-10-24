#include "parser.h"
#include "../helper/help.h"
#include "../helper/error.h"

Lexer* parseLx;

Token* parseToks[MAX_LOOKAHEAD + 1];
int currentLookahead;

Token* lookahead(int t) {
	if (t > MAX_LOOKAHEAD) {
		return nullptr;
	}
	if (t > currentLookahead) {
		for (int i = currentLookahead; i < t; i++) {
			parseToks[i+1] = nextToken(parseLx);
		}
	}
	return parseToks[t];
}

void match (TokenType type) {
	if (parseToks[0]->type != type) {
		stringstream ss;
		ss << "Unexpected '" << (parseToks[0]->type == TOK_EOF ? "EOF" : parseToks[0]->lexeme) << "', expected '" << typeToString(type) << "'";
		error(ss.str(), parseToks[0]->line, parseToks[0]->col);
	}
	// shift parseToks
	for (int i = 0; i < currentLookahead + 1; i++) {
		parseToks[i] = parseToks[i + 1];
	}
	currentLookahead--;
	if (currentLookahead < 0) {
		currentLookahead = 0;
		parseToks[0] = nextToken(parseLx);
	}
}

void match (TokenType type, string lexeme) {
	if (parseToks[0]->type != type && parseToks[0]->lexeme != lexeme) {
		string msg = "Unexpected '";
		msg += parseToks[0]->lexeme;
		msg += "', expected '";
		msg += typeToString(parseToks[0]->type);
		msg += "'";
		error(msg, parseToks[0]->line, parseToks[0]->col);
	}
	// shift parseToks
	for (int i = 0; i < currentLookahead + 1; i++) {
		parseToks[i] = parseToks[i + 1];
	}
	currentLookahead--;
	if (currentLookahead < 0) currentLookahead = 0;
}

ASTNode* parse (Lexer* lx) {
	//newScope();
	parseLx = lx;
	parseToks[0] = nextToken(parseLx);

	ASTNode* root = new ASTNode();
	root->content = newToken("", TOK_UNKNOWN, parseLx);
	root->type = AST_PROGRAM;

	currentLookahead = 0;

	root->firstChild = parseExpression();

	return root;
}

ASTNode* parseExpression(int minbp) {
	ASTNode* lhs;
	auto [_, rbp] = prefixBindingPower(parseToks[0]->type);
	if (parseToks[0]->type == TOK_LPAREN) { // is a grouping
		match(TOK_LPAREN);
		lhs = parseExpression(0);
		match(TOK_RPAREN);
	}
	else if (rbp > 0) { // is a prefix operator
		Token* op = parseToks[0];
		match(op->type);
		ASTNode* rhs = parseExpression(rbp);

		lhs = newNode(op);
		lhs->type = AST_EXPR_UNARY;
		lhs->firstChild = rhs;
	} else { // is an atom
		lhs = parseAtom();
	}
	while (true) {
		Token* op = parseToks[0];
		if (op->type == TOK_EOF) break;
		auto [lbp, rbp] = infixBindingPower(op->type);
		if (lbp == 0 && rbp == 0) {
			break;
		}
		if (lbp < minbp) {
			break;
		}
		match(op->type);
		ASTNode* rhs = parseExpression(rbp);

		ASTNode* temp = newNode(op);
		temp->firstChild = lhs;
		temp->firstChild->sibling = rhs;

		lhs = temp;
		lhs->type = AST_EXPR_BINARY;
	}
	return lhs;
}

ASTNode* parseAtom() {
	ASTNode* atom = newNode(parseToks[0]);
	atom->type = AST_EXPR_LITERAL;
	match(TOK_INT);
	return atom;
}

BindingPower infixBindingPower(TokenType op) {
	switch (op) {
		case TOK_PLUS: case TOK_MINUS: return make_tuple(1, 2);
		case TOK_ASTERISK: case TOK_SLASH: return make_tuple(3, 4);
		default: return make_tuple(0, 0);
	}
}

BindingPower prefixBindingPower(TokenType op) {
	switch (op) {
		case TOK_PLUS: case TOK_MINUS: return make_tuple(0, 5);
		default: return make_tuple(0, 0);
	}
}