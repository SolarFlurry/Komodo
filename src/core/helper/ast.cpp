#include "help.h"

std::string operator* (const string &s, unsigned int n) {
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
	cout << (string("  ") * indent);
	printToken(ast->content);
	if (ast->firstChild != NULL) {
		printAST(ast->firstChild, indent + 1);
	}
	if (ast->sibling != NULL) {
		printAST(ast->sibling, indent);
	}
}