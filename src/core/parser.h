#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

ASTNode* parse (std::vector<Token*> tokList) {
	ASTNode* root = new ASTNode;
	root->content = new Token;
	int index = 0;

	return root;
}

#endif // PARSER_H