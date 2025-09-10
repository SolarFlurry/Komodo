#ifndef AST_H
#define AST_H

#include "token.h"

struct ASTNode {
	Token* content;
	ASTNode* sibling;
	ASTNode* firstChild;
};

void deleteAST(ASTNode* ast) {
	if (ast->sibling != NULL) {
		deleteAST(ast->sibling);
	}
	if (ast->firstChild != NULL) {
		deleteAST(ast->firstChild);
	}
	deleteToken(ast->content);
	delete ast;
	ast = nullptr;
}

#endif // AST_H