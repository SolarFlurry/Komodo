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
	} else if (stmt->content->type == AssignStatement) {
		return genAssignStatement(stmt);
	} else if (stmt->content->type == ImportStatement) {
		return genImportStmt(stmt);
	} else {
		auto [instr, type] = genExpression(stmt);
		return instr;
	}
}

string genCmdStmt(ASTNode* stmt) {
	if (stmt->content->type != CommandStatement) {
		fatalError("not a command statement", stmt->content->line);
		return "";
	}
	auto [cmd, type] = genExpression(stmt->firstChild);
	if (type != String) {
		fatalError("expected a string", stmt->firstChild->content->line);
		return "";
	}
	string instr = checkExecute();
	instr += cmd;
	return instr + '\n';
}

string genImportStmt(ASTNode* stmt) {
	if (stmt->content->type != ImportStatement) {
		fatalError("not a command statement", stmt->content->line);
		return "";
	}
	string instr = checkExecute();
	instr += "function ";
	instr += stmt->firstChild->content->lexeme;
	instr += "/main\n";
	return instr;
}

string genVarDeclaration(ASTNode* stmt) {
	if (stmt->content->type != VarDeclaration) {
		fatalError("not a variable declaration", stmt->content->line);
		return "";
	}
	if (stmt->firstChild->content->lexeme == "const" || stmt->firstChild->sibling->sibling == nullptr) {
		return "";
	}
	string instr = checkExecute();
	auto [cmd, type] = genExpression(stmt->firstChild->sibling->sibling);
	if (type != Integer) {
		fatalError("Expected integer", stmt->content->line);
	}
	instr += cmd;
	instr += checkExecute();
	auto symtabId = symtabLookup(stmt->firstChild->sibling->content->lexeme);
	if (symtabId->varType == Score) {
		instr += "scoreboard players operation @e ";
		if (nameSpaces.back() != "") {instr += nameSpaces.back(); instr += ":";}
		instr += stmt->firstChild->sibling->content->lexeme;
		instr += " = R0 .komodo";
	} else {
		instr += "scoreboard players operation ";
		if (nameSpaces.back() != "") {instr += nameSpaces.back(); instr += ":";}
		instr += symtabId->genName;
		instr += " .global = R0 .komodo";
	}
	instr += '\n';
	return instr;
}

string genAssignStatement(ASTNode* stmt) {
	if (stmt->content->type != AssignStatement) {
		fatalError("not an assign statement", stmt->content->line);
		return "";
	}
	auto [instr, type] = genExpression(stmt->firstChild->sibling);
	instr += checkExecute();
	instr += "scoreboard players operation ";
	auto symtabId = symtabLookup(stmt->firstChild->content->lexeme);
	if (type != symtabId->type) {
		fatalError("mismatched types", stmt->content->line);
	}
	if (symtabId == nullptr) {
		fatalError("variable does not exist", stmt->firstChild->content->line);
	}
	if (symtabId->varType == Score) {
		instr += "@s ";
		if (nameSpaces.back() != "") {instr += nameSpaces.back(); instr += ":";}
		instr += symtabId->genName;
	} else if (symtabId->varType == Global) {
		if (nameSpaces.back() != "") {instr += nameSpaces.back(); instr += ":";}
		instr += symtabId->genName;
		instr += " .global";
	} else {
		fatalError("cannot assign to variable", stmt->content->line);
		return "";
	}
	instr += " = R0 .komodo\n";
	return instr;
}

#endif // STATEMENT_H