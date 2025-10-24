#include "help.h"

std::string repeatStr (const string &s, unsigned int n) {
	string result = "";
	for (int i = 0; i < n; i++) {
		result += s;
	}
	return result;
}

ASTNode* newNode(Token* tok) {
	ASTNode* node = new ASTNode();
	node->content = tok;
	return node;
}

void deleteAST(ASTNode* ast) {
	if (ast->sibling != nullptr) {
		deleteAST(ast->sibling);
	}
	if (ast->firstChild != nullptr) {
		deleteAST(ast->firstChild);
	}
	deleteToken(ast->content);
	delete ast;
	ast = nullptr;
}

void printAST(ASTNode* ast, int indent) {
	if (indent > 0) {
		cout << repeatStr("  ", indent - 1) << "↳ ";
	}
	string type;
	switch (ast->type) {
		case AST_PROGRAM: type = "Program"; break;
		case AST_EXPR_BINARY: type = "BinaryOp"; break;
		case AST_EXPR_UNARY: type = "UnaryOp"; break;
		case AST_EXPR_LITERAL: type = "Literal"; break;
		default: type = "Unknown";
	}
	cout << "\x1b[36m" << type << "\x1b[0m: \x1b[33m\"" << ast->content->lexeme << "\"\x1b[0m\n";
	if (ast->firstChild != nullptr) {
		printAST(ast->firstChild, indent + 1);
	}
	if (ast->sibling != nullptr) {
		printAST(ast->sibling, indent);
	}
}