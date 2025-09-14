#ifndef EXECUTE_H
#define EXECUTE_H

#include "gen.h"

vector<string> executeChain;

string checkExecute() {
	if (executeChain.size() > 0) {
		string acc = "execute ";
		for (string execute : executeChain) {
			acc += execute;
			acc += ' ';
		}
		acc += "run ";
		return acc;
	} else {
		return "";
	}
}

string genExecuteStmt (ASTNode* stmt) {
	if (stmt->content->type != ExecuteStatement) {
		error("not a command statement", stmt->content->line);
		return "";
	}
	string execute = stmt->firstChild->content->lexeme;
	execute += ' ';
	auto [cmd, type] = genExpression(stmt->firstChild->sibling);
	if (type != String) {
		fatalError("Expected string");
	}
	execute += cmd;
	executeChain.push_back(execute);
	string acc = genStatement(stmt->firstChild->sibling->sibling);
	executeChain.pop_back();
	return acc;
}

#endif // EXECUTE_H