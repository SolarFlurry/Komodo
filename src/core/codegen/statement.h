#ifndef STATEMENT_H
#define STATEMENT_H

#include "gen.h"

string genStatementList(ASTNode* list) {
	string acc = genStatement(list);
	if (list->sibling == nullptr) {
		return acc;
	}
	acc += genStatementList(list->sibling);
	return acc;
}

string genStatement(ASTNode* stmt) {
	if (stmt->content->type == CommandStatement) {
		return genCmdStmt(stmt);
	} else if (stmt->content->type == ExecuteStatement) {
		return genExecuteStmt(stmt);
	} else if (stmt->content->type == ComplexExpression) {
		return genStatementList(stmt->firstChild);
	} else if (stmt->content->type == VarDeclaration) {
		return genVarDeclaration(stmt);
	} else if (stmt->content->type == FunctionDeclaration) {
		return genFuncDeclaration(stmt);
	} else {
		auto [instr, type] = genExpression(stmt);
		return instr;
	}
}

string genCmdStmt(ASTNode* stmt) {
	if (stmt->content->type != CommandStatement) {
		fatalError("not a command statement");
		return "";
	}
	auto [cmd, type] = genExpression(stmt->firstChild);
	if (type != String) {
		fatalError("expected a string");
		return "";
	}
	string instr = checkExecute();
	return instr + cmd + '\n';
}

string genVarDeclaration(ASTNode* stmt) {
	if (stmt->content->type != VarDeclaration) {
		fatalError("not a variable declaration");
		return "";
	}
	if (stmt->firstChild->content->lexeme == "const" || stmt->firstChild->sibling->sibling == nullptr) {
		return "";
	}
	string instr = checkExecute();
	auto [cmd, type] = genExpression(stmt->firstChild->sibling->sibling);
	if (type != Integer) {
		fatalError("Expected integer");
	}
	instr += cmd;
	instr += checkExecute();
	if (stmt->firstChild->content->lexeme == "score") {
		instr += "scoreboard players operation @e ";
		instr += stmt->firstChild->sibling->content->lexeme;
		instr += " = R0 .komodo";
	} else {
		instr += "scoreboard players operation ";
		instr += stmt->firstChild->sibling->content->lexeme;
		instr += " .global = R0 .komodo";
	}
	instr += '\n';
	return instr;
}

#endif // STATEMENT_H