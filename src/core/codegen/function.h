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

	string filename = funcName;
	filename += ".mcfunction";
	auto fullpath = filesystem::path("functions");
	if (nameSpaces.back() != "") fullpath /= nameSpaces.back();
	fullpath /= filename;

	ofstream outputFile(fullpath);
	if (!outputFile.is_open()) {
		string msg = "\x1b[31mFatal Error: Could not write to file '";
		msg += fullpath;
		msg += "'\x1b[0m\n";
		cout << msg;
		exit(0);
	}
	outputFile << func;

	return "";
}

string genFunctionCall(ASTNode* stmt, string ns) {
	if (stmt->content->type != FunctionCall) {
		fatalError("not a function call", stmt->content->line);
	}
	auto symtabId = symtabLookup(stmt->firstChild->content->lexeme, ns);
	if (symtabId == nullptr || symtabId->varType != Function) {
		string msg = "Function '";
		msg += ns;
		msg += ':';
		msg += stmt->firstChild->content->lexeme;
		msg += "' does not exist";
		fatalError(msg, stmt->content->line);
	}
	string instr;
	if (stmt->firstChild->firstChild != nullptr) {
		instr = genFunctionArguments(stmt->firstChild->firstChild, 0);
	}
	instr += checkExecute();
	instr += "function ";
	if (ns != "") {
		instr += ns;
		instr += '/';
	}
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