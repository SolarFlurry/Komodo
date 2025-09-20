#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstddef>
#include <utility>
#include <fstream>
#include <filesystem>

using namespace std;

string program;
vector<string> lines;

#include "error.h"
#include "symtable.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "codegen/gen.cpp"

int main () {
	string line;
	
	while (getline(cin, line)) {
		program += line + '\n';
		lines.push_back(line);
	}

	vector<Token*> tokList = tokenise(program);

	if (errors.size() == 0) {
		cout << "[ 33%] \x1b[32mTokenisation complete\x1b[0m\n";
		ASTNode* ast = parse(tokList);
		if (errors.size() == 0) {
			cout << "[ 67%] \x1b[32mParsing complete\x1b[0m\n";
			auto _ = codeGen(ast);
			if (errors.size() == 0) {
				cout << "[100%] \x1b[32;1mGenerated functions\x1b[0m\n";
			}
		}
	}
	if (errors.size() > 0) {
		cout << "\x1b[31mCompilation failed with " << errors.size() << " errors\x1b[0m\n";
	}
}