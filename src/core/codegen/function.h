#ifndef FUNCTION_H
#define FUNCTION_H

string genFuncDeclaration(ASTNode* stmt) {
	if (stmt->content->type != FunctionDeclaration) {
		fatalError("not a function declaration", stmt->content->line);
	}
	string func;
	string funcName = stmt->firstChild->content->lexeme;

	ASTNode* funcContent;
	if (stmt->firstChild->sibling->content->type == ParameterList) {
		funcContent = stmt->firstChild->sibling->sibling;
	} else {
		funcContent = stmt->firstChild->sibling;
	}

	auto [cmd, type] = genExpression(funcContent);
	if (type != Integer) {
		fatalError("Expected integer type", funcContent->content->line);
	}
	func = cmd;

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
		fatalError("not a function call", stmt->content->line);
	}
	auto symtabId = symtabLookup(stmt->firstChild->content->lexeme);
	if (symtabId == nullptr || symtabId->varType != Function) {
		fatalError("identifier does not exist", stmt->content->line);
	}
	string instr;
	if (stmt->firstChild->firstChild != nullptr) {
		instr = genFunctionArguments(stmt->firstChild->firstChild, 0);
	}
	instr += checkExecute();
	instr += "function ";
	instr += stmt->firstChild->content->lexeme;
	instr += '\n';
	return instr;
}

string genFunctionArguments(ASTNode* stmt, int index) {
	auto [instr, type] = genExpression(stmt);
	instr += checkExecute();
	instr += "scoreboard players operation ";
	instr += "arg" + to_string(index);
	instr += " .komodo = RO .komodo\n";
	if (stmt->sibling != nullptr) {
		instr += genFunctionArguments(stmt->sibling, index + 1);
	}
	return instr;
}

#endif // FUNCTION_H