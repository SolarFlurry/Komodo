#ifndef FUNCTION_H
#define FUNCTION_H

string genFuncDeclaration(ASTNode* stmt) {
	if (stmt->content->type != FunctionDeclaration) {
		fatalError("not a function declaration");
	}
	string func;
	string funcName = stmt->firstChild->content->lexeme;

	if (stmt->firstChild->sibling->content->type == ComplexExpression) {
		func = genStatementList(stmt->firstChild->sibling->firstChild);
	} else {
		auto [cmd, type] = genExpression(stmt->firstChild->sibling);
		if (type != Integer) {
			fatalError("Expected integer type");
		}
		func = cmd;
	}

	string filename = "functions/";
	filename += funcName;
	filename += ".mcfunction";

	ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		cout << "\x1b[31mFATAL ERROR: Could not write to file\x1b[0m\n";
		exit(0);
	}
	outputFile << func;

	return "";
}

string genFunctionCall(ASTNode* stmt) {
	if (stmt->content->type != FunctionCall) {
		fatalError("not a function call");
	}
	auto symtabId = symtabLookup(stmt->firstChild->content->lexeme);
	if (symtabId == nullptr || symtabId->type != Function) {
		fatalError("identifier does not exist");
	}
	string instr = checkExecute();
	instr += "function ";
	instr += stmt->firstChild->content->lexeme;
	instr += '\n';
	return instr;
}

#endif // FUNCTION_H