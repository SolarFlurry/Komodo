#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

using namespace std;

#include "error.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"

int main () {
	string line;
	string program;
	
	while (getline(cin, line)) {
		program += line + '\n';
	}

	vector<Token*> tokList = tokenise(program);

	for (int i = 0; i < tokList.size(); i++) {
		printToken(tokList[i]);
	}

	cout << "\nLexer Errors:\n";
	for (int i = 0; i < errors.size(); i++) {
		printToken(errors[i]);
	}
	if (errors.size() == 0) {
		cout << "No Errors!\n\nAbstract Syntax Tree:\n";
		ASTNode* ast = parse(tokList);
		printAST(ast, 0);
	}
	cout << "\nAll Errors:\n";
	if (errors.size() == 0) {
		cout << "No Errors!\n";
	}
	for (int i = 0; i < errors.size(); i++) {
		printToken(errors[i]);
	}
}