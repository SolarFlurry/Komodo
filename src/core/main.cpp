#include <iostream>
#include <vector>
#include <string>
#include <cstddef>
#include <utility>
#include <fstream>
#include <filesystem>

using namespace std;

#include "error.h"
#include "symtable.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "codegen/gen.cpp"

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
		cout << "\nSymbol Table:\n";
		printSymtable();
		if (errors.size() == 0) {
			auto _ = codeGen(ast);
		}
	}
	cout << "All Errors:\n";
	if (errors.size() == 0) {
		cout << "No Errors!\n";
	}
	for (int i = 0; i < errors.size(); i++) {
		printToken(errors[i]);
	}
}