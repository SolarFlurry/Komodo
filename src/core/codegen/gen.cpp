#ifndef GEN_CPP
#define GEN_CPP

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

#include "gen.h"

#include "function.h"
#include "execute.h"
#include "statement.h"
#include "expression.h"

const char* KOMODO_ENV;

string codeGen(ASTNode* ast) {
	KOMODO_ENV = getenv("KOMODO_ENV");
	if (KOMODO_ENV == nullptr) {
		cout << "\x1b[31mFATAL ERROR: Missing environment variable KOMODO_ENV\x1b[0m\n";
		exit(0);
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

	inputFile.close();

	filesystem::create_directory("functions");

	main += genStatementList(ast->firstChild);

	ofstream outputFile("functions/main.mcfunction");
	if (!outputFile.is_open()) {
		cout << "\x1b[31mFATAL ERROR: Could not write to main.mcfunction\x1b[0m\n";
		exit(0);
	}
	outputFile << main;
	outputFile.close();
	
	return main;
}

#endif // GEN_CPP