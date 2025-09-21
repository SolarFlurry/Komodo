#ifndef PARSER_H
#define PARSER_H

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

ASTNode* parseImportStmt();
ASTNode* parseStatementList();
ASTNode* parseStatement();
ASTNode* parseCmdStmt();
ASTNode* parseReturnStmt();
ASTNode* parseExecuteStmt();
ASTNode* parseVarDeclaration();
ASTNode* parseAssignStmt();
ASTNode* parseParameterList(int index);
ASTNode* parseFunctionDeclaration();
ASTNode* parseNamespaceStmt();
ASTNode* parseExpressionList();
ASTNode* parseExpression();
ASTNode* parseTerm();
ASTNode* parseFactor(string ns = nameSpaces.back());

int parserIdx = 0;
Token* parserTok;
vector<Token*> parseToks;

void match (TokenType type) {
	if (parserTok->type != type) {
		string msg = "Unexpected '";
		msg += parserTok->lexeme;
		msg += "', expected '";
		msg += typeToString(parserTok->type);
		msg += "'";
		error(msg, parserTok->line);
	}
	parserIdx++;
	parserTok = parseToks[parserIdx];
}

void match (TokenType type, string lexeme) {
	if (parserTok->type != type && parserTok->lexeme != lexeme) {
		string msg = "Unexpected '";
		msg += parserTok->lexeme;
		msg += "', expected '";
		msg += typeToString(parserTok->type);
		msg += "'";
		error(msg, parserTok->line);
	}
	parserIdx++;
	parserTok = parseToks[parserIdx];
}

ASTNode* parse (vector<Token*> tokList) {
	//newScope();
	parseToks = tokList;
	ASTNode* root = new ASTNode();
	root->content = newToken("", Program, 0);
	parserIdx = 0;
	parserTok = parseToks[parserIdx];

	newNamespace("");

	root->firstChild = parseStatementList();

	return root;
}

ASTNode* parseImportStmt() {
	match(Keyword, "import");
	if (parserTok->type != String) {
		fatalError("Expected a string", parserTok->line);
	}
	ifstream inputFile;
	if (!filesystem::exists(parserTok->lexeme)) {
		inputFile.open(filesystem::path(KOMODO_ENV) / parserTok->lexeme);
	} else {
		inputFile.open(parserTok->lexeme);
	}

	string instr = "function ";
	instr += parserTok->lexeme;
	instr += "/main";
	if (!inputFile.is_open()) {
		string msg = "Module '";
		msg += parserTok->lexeme;
		msg += "' does not exist";
		fatalError(msg, parserTok->line);
	}
	match(String);

	string line;
	vector<string> lines;
	string modProgram;

	int beforeParserIdx = parserIdx;
	auto beforeTokList = parseToks;
	auto beforeParserTok = parserTok;

	while (getline(inputFile, line)) {
		modProgram += line + '\n';
		lines.push_back(line);
	}
	inputFile.close();
	int namespacesNum = nameSpaces.size();
	vector<Token*> modTokList = tokenise(modProgram);
	if (errors.size() == 0) {
		ASTNode* modAst = parse(modTokList);
		if (errors.size() == 0) {
			if (nameSpaces.size() <= namespacesNum) {
				string msg = "Module does not contain a namespace";
				fatalError(msg, parserTok->line);
			}
			auto result = codeGen(modAst, nameSpaces.back());
		}
	}
	parserIdx = beforeParserIdx;
	parseToks = beforeTokList;
	parserTok = beforeParserTok;

	auto stmt = newNode(newParseToken(ImportStatement));
	stmt->firstChild = newNode(newToken(nameSpaces.back(), String, parserTok->line));
	if (nameSpaces.size() > namespacesNum) {
		for (int i = nameSpaces.size(); i > namespacesNum; i--) {
			nameSpaces.pop_back();
		}
	}
	return stmt;
}

// possibly returns nullptr
ASTNode* parseStatementList() {
	if (parserTok->type == Eof || parserTok->type == RightBrace) {
		return nullptr;
	}
	auto stmt = parseStatement();
	if (stmt == nullptr) {
		stmt = parseStatementList();
		return stmt;
	}
	auto next = parseStatementList();
	if (next != nullptr) {
		stmt->sibling = next;
	}
	return stmt;
}

// possible returns nullptr
ASTNode* parseStatement() {
	ASTNode* stmt = nullptr;
	if (parserTok->type == BinaryOperator && parserTok->lexeme == "/") {
		stmt = parseCmdStmt();
	} else if (parserTok->type == Keyword) {
		if (parserTok->lexeme == "return") {
			stmt = parseReturnStmt();
		} else if (parserTok->lexeme == "func") {
			stmt = parseFunctionDeclaration();
		} else if (parserTok->lexeme == "const" || parserTok->lexeme == "glob" || parserTok->lexeme == "score") {
			stmt = parseVarDeclaration();
		} else if (parserTok->lexeme == "namespace") {
			stmt = parseNamespaceStmt();
		} else if (parserTok->lexeme == "import") {
			stmt = parseImportStmt();
		} else {
			stmt = parseExecuteStmt();
		}
	} else if (parserTok->type == Identifier && parseToks[parserIdx+1]->type == BinaryOperator && parseToks[parserIdx+1]->lexeme == "=") {
		stmt = parseAssignStmt();
	} else {
		stmt = parseExpression();
	}
	if (parserTok->type == Semicolon) {
		match(Semicolon);
	}
	return stmt;
}

ASTNode* parseCmdStmt() {
	match(BinaryOperator, "/");
	auto cmdNode = newNode(newParseToken(CommandStatement));
	cmdNode->firstChild = parseExpression();
	return cmdNode;
}

ASTNode* parseReturnStmt() {
	ASTNode* returnNode = newNode(newParseToken(ReturnStatement));
	match(Keyword, "return");
	returnNode->firstChild = parseExpression();
	return returnNode;
}

ASTNode* parseExecuteStmt() {
	auto executeNode = newNode(newParseToken(ExecuteStatement));
	if (parserTok->type == Keyword && (parserTok->lexeme == "if" || parserTok->lexeme == "as" || parserTok->lexeme == "at" || parserTok->lexeme == "in")) {
		executeNode->firstChild = newNode(parserTok);
		match(Keyword);
		executeNode->firstChild->sibling = parseExpression();
		executeNode->firstChild->sibling->sibling = parseStatement();
	} else {
		match(Keyword);
	}
	return executeNode;
}

ASTNode* parseVarDeclaration() {
	auto varDec = newNode(newParseToken(VarDeclaration));
	if (parserTok->type == Keyword && (parserTok->lexeme == "const" || parserTok->lexeme == "glob" || parserTok->lexeme == "score")) {
		varDec->firstChild = newNode(parserTok);
		match(Keyword);
		
		if (varExists(parserTok->lexeme)) {
			string msg = "Variable '";
			msg += parserTok->lexeme;
			msg += "' already declared";
			error(msg, parserTok->line);
		}

		varDec->firstChild->sibling = newNode(parserTok);
		symtableEntry* symtabId = new symtableEntry(Uninitialised);
		symtabId->genName = parserTok->lexeme;
		symtabAdd(parserTok->lexeme, symtabId);

		if (parseToks[parserIdx-1]->lexeme == "const") {
			symtabId->varType = Constant;
		} else if (parseToks[parserIdx-1]->lexeme == "glob") {
			symtabId->varType = Global;
		} else if (parseToks[parserIdx-1]->lexeme == "score") {
			symtabId->varType = Score;
		}
		match(Identifier);
		if (isTypeKeyword(parserTok)) {
			if (parserTok->lexeme == "int") {
				symtabId->type = Integer;
			}
			match(Keyword);
		} else {
			symtabId->type = Integer;
		}
		if (parserTok->type == BinaryOperator && parserTok->lexeme == "=") {
			match(BinaryOperator, "=");
			varDec->firstChild->sibling->sibling = parseExpression();
		}
	} else {
		match(Keyword);
	}
	return varDec;
}

ASTNode* parseAssignStmt() {
	auto assignNode = newNode(newParseToken(AssignStatement));
	assignNode->firstChild = newNode(parserTok);
	match(Identifier);
	match(BinaryOperator, "=");
	assignNode->firstChild->sibling = parseExpression();
	return assignNode;
}

ASTNode* parseParameterList(int index) {
	ASTNode* paramList = newNode(parserTok);
	auto symtabId = new symtableEntry(Argument);
	symtabId->genName = "arg" + to_string(index);
	symtabAdd(parserTok->lexeme, symtabId);
	match(Identifier);
	if (parserTok->type == Comma) {
		match(Comma);
		paramList->sibling = parseParameterList(index + 1);
	}
	return paramList;
}

ASTNode* parseFunctionDeclaration() {
	match(Keyword, "func");
	auto funcDec = newNode(newParseToken(FunctionDeclaration));
	funcDec->firstChild = newNode(parserTok);

	if (varExists(parserTok->lexeme)) {
		string msg = "Variable '";
		msg += parserTok->lexeme;
		msg += "' already declared";
		error(msg, parserTok->line);
	}

	symtableEntry* symtabId = new symtableEntry(Uninitialised);
	symtabId->varType = Function;
	symtabAdd(parserTok->lexeme, symtabId);

	match(Identifier);
	match(LeftParen);
	if (parserTok->type != RightParen) {
		funcDec->firstChild->sibling = newNode(newParseToken(ParameterList));
		funcDec->firstChild->sibling->firstChild = parseParameterList(0);
		match(RightParen);
		funcDec->firstChild->sibling->sibling = parseExpression();
	} else {
		match(RightParen);
		funcDec->firstChild->sibling = parseExpression();
	}
	return funcDec;
}

// returns nullptr
ASTNode* parseNamespaceStmt() {
	match(Keyword, "namespace");
	newNamespace(parserTok->lexeme);
	match(Identifier);
	return nullptr;
}

ASTNode* parseExpressionList() {
	auto exprList = parseExpression();
	if (parserTok->type == Comma) {
		match(Comma);
		exprList->sibling = parseExpressionList();
	}
	return exprList;
}

ASTNode* parseExpression() {
	ASTNode* expr = parseTerm();
	if (parserTok->type == BinaryOperator && (parserTok->lexeme == "+" || parserTok->lexeme == "-")) {
		auto op = newNode(parserTok);
		op->firstChild = expr;
		match(BinaryOperator);
		expr->sibling = parseExpression();
		return op;
	}
	return expr;
}

ASTNode* parseTerm() {
	ASTNode* term = parseFactor();
	if (parserTok->type == BinaryOperator && (parserTok->lexeme == "*" || parserTok->lexeme == "/")) {
		auto op = newNode(parserTok);
		op->firstChild = term;
		match(BinaryOperator);
		term->sibling = parseTerm();
		return op;
	}
	return term;
}

ASTNode* parseFactor (string ns) {
	ASTNode* factor = nullptr;
	switch (parserTok->type) {
		case Identifier: {
			factor = newNode(parserTok);
			auto identifier = symtabLookup(parserTok->lexeme, ns);
			if ((identifier == nullptr || identifier->varType == Uninitialised) && parseToks[parserIdx + 1]->type != Colon) {
				string msg = "Use of undeclared variable '";
				msg += ns;
				msg += ':';
				msg += parserTok->lexeme;
				msg += "'";
				error(msg, parserTok->line);
			}
			match(Identifier);
			if (parserTok->type == LeftParen) {
				auto funcNode = newNode(newParseToken(FunctionCall));
				funcNode->firstChild = factor;
				match(LeftParen);
				if (parserTok->type != RightParen) {
					factor->firstChild = parseExpressionList();
				}
				match(RightParen);
				return funcNode;
			} else if (parserTok->type == Colon) {
				auto colonNode = newNode(parserTok);
				match(Colon);
				colonNode->firstChild = factor;
				colonNode->firstChild->sibling = parseFactor(factor->content->lexeme);
				return colonNode;
			}
		} break;
		case Integer: {
			factor = newNode(parserTok);
			match(Integer);
		} break;
		case String: {
			factor = newNode(parserTok);
			match(String);
		} break;
		case LeftParen: {
			match(LeftParen);
			factor = parseExpression();
			match(RightParen);
		} break;
		case LeftBrace: {
			match(LeftBrace);
			factor = newNode(newParseToken(ComplexExpression));
			auto stmts = parseStatementList();
			if (stmts != nullptr) {
				factor->firstChild = stmts;
			}
			match(RightBrace);
		} break;
		default:
			string msg = "Unexpected '";
			msg += parserTok->lexeme;
			msg += "'";
			error(msg, parserTok->line);
			factor = newNode(parserTok);
			match(parserTok->type);
	}
	return factor;
}

#endif // PARSER_H