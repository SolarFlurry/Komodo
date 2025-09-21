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
vector<string> nameSpaces;

const char* KOMODO_ENV;

#include "error.h"
#include "symtable.h"
#include "codegen/gen.cpp"
#include "compiler/lexer.h"
#include "compiler/parser.h"

int main () {
	KOMODO_ENV = getenv("KOMODO_ENV");
	if (KOMODO_ENV == nullptr) {
		cout << "\x1b[31mFATAL ERROR: Missing environment variable KOMODO_ENV\x1b[0m\n";
		exit(0);
	}
	string line;
	
	while (getline(cin, line)) {
		program += line + '\n';
		lines.push_back(line);
	}

	vector<Token*> tokList = tokenise(program);

	if (errors.size() == 0) {
		/*for (auto i : tokList) {
			printToken(i);
		}*/
		cout << "[ 33%] \x1b[32mTokenisation complete\x1b[0m\n";
		ASTNode* ast = parse(tokList);
		if (errors.size() == 0) {
			cout << "[ 67%] \x1b[32mParsing complete\x1b[0m\n";
			auto result = codeGen(ast, nameSpaces.back());
			if (errors.size() == 0) {
				if (result == "") {
					cout << "[100%] \x1b[32;1mEmpty program, functions not generated\x1b[0m\n";
				} else {
					cout << "[100%] \x1b[32;1mGenerated functions\x1b[0m\n";
				}
			}
		}
	}
	if (errors.size() > 0) {
		cout << "\x1b[31mCompilation failed with " << errors.size() << " errors\x1b[0m\n";
	}
}