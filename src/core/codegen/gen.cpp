#ifndef GEN_CPP
#define GEN_CPP

filesystem::path namespacePath;

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

#include "gen.h"

#include "function.h"
#include "execute.h"
#include "statement.h"
#include "expression.h"

string codeGen(ASTNode* ast, string nameSpace) {
	if (ast->firstChild == nullptr) {
		return "";
	}
	ifstream inputFile(filesystem::path(KOMODO_ENV) / "main.mcfunction");
	if (!inputFile.is_open()) {
		cout << "\x1b[31mFATAL ERROR: Make sure the KOMODO_ENV variable is the right path\x1b[0m\n";
		exit(0);
	}
	string line;
	string main;
	while (getline(inputFile, line)) {
		main += line + "\n";
	}

	namespacePath = filesystem::path("functions");

	if (nameSpace != "") {
		namespacePath /= nameSpace;
		filesystem::create_directory(namespacePath);
	};

	inputFile.close();

	filesystem::create_directory("functions");

	main += genStatementList(ast->firstChild);

	ofstream outputFile(namespacePath / "main.mcfunction");
	if (!outputFile.is_open()) {
		cout << "\x1b[31mFatal Error: Could not write to main.mcfunction\x1b[0m\n";
		exit(0);
	}
	outputFile << main;
	outputFile.close();
	
	return main;
}

#endif // GEN_CPP