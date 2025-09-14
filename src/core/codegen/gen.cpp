#ifndef GEN_CPP
#define GEN_CPP

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

#include "gen.h"

#include "execute.h"
#include "statement.h"
#include "expression.h"

string codeGen(ASTNode* ast) {
	const char* KOMODO_ENV = getenv("KOMODO_ENV");
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
	cout << "\nCode Generation:\n";
	main += genStatementList(ast->firstChild);
	cout << main << "\n";
	return main;
}

#endif // GEN_CPP