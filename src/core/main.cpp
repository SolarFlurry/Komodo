#include <iostream>
#include <vector>
#include <unordered_map>
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

	cout << "\nLexer Errors:\n";
	printErrors();
	if (errors.size() == 0) {
		cout << "No Errors!\n";
		ASTNode* ast = parse(tokList);
		cout << "Parser Errors:\n";
		printErrors();
		if (errors.size() == 0) {
			cout << "No Errors!\n";
			auto _ = codeGen(ast);
		}
	}
	cout << "All Errors:\n";
	if (errors.size() == 0) {
		cout << "No Errors!\n";
	}
	printErrors();
}