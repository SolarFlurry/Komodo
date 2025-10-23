#ifndef PARSER_H
#define PARSER_H

#include "compiler.h"

Lexer* parseLx;

Token* parseToks[MAX_LOOKAHEAD + 1];
int currentLookahead;

Token* lookahead(int t) {
	if (t > MAX_LOOKAHEAD) {
		return nullptr;
	}
	if (t > currentLookahead) {
		for (int i = currentLookahead; i < t; i++) {
			parseToks[i+1] = nextToken(parseLx);
		}
	}
	return parseToks[t];
}

void match (TokenType type) {
	if (parseToks[0]->type != type) {
		string msg = "Unexpected '";
		msg += parseToks[0]->lexeme;
		msg += "', expected '";
		msg += typeToString(parseToks[0]->type);
		msg += "'";
		error(msg, parseToks[0]->line);
	}
	// shift parseToks
	for (int i = 0; i < currentLookahead + 1; i++) {
		parseToks[i] = parseToks[i + 1];
	}
	currentLookahead--;
	if (currentLookahead < 0) currentLookahead = 0;
}

void match (TokenType type, string lexeme) {
	if (parseToks[0]->type != type && parseToks[0]->lexeme != lexeme) {
		string msg = "Unexpected '";
		msg += parseToks[0]->lexeme;
		msg += "', expected '";
		msg += typeToString(parseToks[0]->type);
		msg += "'";
		error(msg, parseToks[0]->line);
	}
	// shift parseToks
	for (int i = 0; i < currentLookahead + 1; i++) {
		parseToks[i] = parseToks[i + 1];
	}
	currentLookahead--;
	if (currentLookahead < 0) currentLookahead = 0;
}

ASTNode* parse (Lexer* lx) {
	//newScope();
	parseLx = lx;

	ASTNode* root = new ASTNode();
	root->content = newToken("", Program, 0);

	currentLookahead = 0;

	root->firstChild = parseStatementList();

	return root;
}

ASTNode* parseImportStmt() {
	match(Keyword, "import");
	if (parseToks[0]->type != String) {
		fatalError("Expected a string", parseToks[0]->line);
	}
	ifstream inputFile;
	if (!filesystem::exists(parseToks[0]->lexeme)) {
		inputFile.open(filesystem::path(KOMODO_ENV) / parseToks[0]->lexeme);
	} else {
		inputFile.open(parseToks[0]->lexeme);
	}

	string instr = "function ";
	instr += parseToks[0]->lexeme;
	instr += "/main";
	if (!inputFile.is_open()) {
		string msg = "Module '";
		msg += parseToks[0]->lexeme;
		msg += "' does not exist";
		fatalError(msg, parseToks[0]->line);
	}
	match(String);

	string line;
	vector<string> lines;
	string modProgram;

	int beforeParserIdx = parserIdx;
	auto beforeTokList = parseToks;
	auto beforeparseToks[0] = parseToks[0];

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
				fatalError(msg, parseToks[0]->line);
			}
			auto result = codeGen(modAst, nameSpaces.back());
		}
	}
	parserIdx = beforeParserIdx;
	parseToks = beforeTokList;
	parseToks[0] = beforeparseToks[0];

	auto stmt = newNode(newParseToken(ImportStatement));
	stmt->firstChild = newNode(newToken(nameSpaces.back(), String, parseToks[0]->line));
	if (nameSpaces.size() > namespacesNum) {
		for (int i = nameSpaces.size(); i > namespacesNum; i--) {
			nameSpaces.pop_back();
		}
	}
	return stmt;
}

// possibly returns nullptr
ASTNode* parseStatementList() {
	if (parseToks[0]->type == Eof || parseToks[0]->type == RightBrace) {
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
	if (parseToks[0]->type == BinaryOperator && parseToks[0]->lexeme == "/") {
		stmt = parseCmdStmt();
	} else if (parseToks[0]->type == Keyword) {
		if (parseToks[0]->lexeme == "return") {
			stmt = parseReturnStmt();
		} else if (parseToks[0]->lexeme == "func") {
			stmt = parseFunctionDeclaration();
		} else if (parseToks[0]->lexeme == "const" || parseToks[0]->lexeme == "glob" || parseToks[0]->lexeme == "score") {
			stmt = parseVarDeclaration();
		} else if (parseToks[0]->lexeme == "namespace") {
			stmt = parseNamespaceStmt();
		} else if (parseToks[0]->lexeme == "import") {
			stmt = parseImportStmt();
		} else {
			stmt = parseExecuteStmt();
		}
	} else if (parseToks[0]->type == Identifier && lookahead(1)->type == BinaryOperator && lookahead(1)->lexeme == "=") {
		stmt = parseAssignStmt();
	} else {
		stmt = parseExpression();
	}
	if (parseToks[0]->type == Semicolon) {
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
	if (parseToks[0]->type == Keyword && (parseToks[0]->lexeme == "if" || parseToks[0]->lexeme == "as" || parseToks[0]->lexeme == "at" || parseToks[0]->lexeme == "in")) {
		executeNode->firstChild = newNode(parseToks[0]);
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
	if (parseToks[0]->type == Keyword && (parseToks[0]->lexeme == "const" || parseToks[0]->lexeme == "glob" || parseToks[0]->lexeme == "score")) {
		varDec->firstChild = newNode(parseToks[0]);
		match(Keyword);
		
		if (varExists(parseToks[0]->lexeme)) {
			string msg = "Variable '";
			msg += parseToks[0]->lexeme;
			msg += "' already declared";
			error(msg, parseToks[0]->line);
		}

		varDec->firstChild->sibling = newNode(parseToks[0]);
		symtableEntry* symtabId = new symtableEntry(Uninitialised);
		symtabId->genName = parseToks[0]->lexeme;
		symtabAdd(parseToks[0]->lexeme, symtabId);

		if (parseToks[parserIdx-1]->lexeme == "const") {
			symtabId->varType = Constant;
		} else if (parseToks[parserIdx-1]->lexeme == "glob") {
			symtabId->varType = Global;
		} else if (parseToks[parserIdx-1]->lexeme == "score") {
			symtabId->varType = Score;
		}
		match(Identifier);
		if (isTypeKeyword(parseToks[0])) {
			if (parseToks[0]->lexeme == "int") {
				symtabId->type = Integer;
			}
			match(Keyword);
		} else {
			symtabId->type = Integer;
		}
		if (parseToks[0]->type == BinaryOperator && parseToks[0]->lexeme == "=") {
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
	assignNode->firstChild = newNode(parseToks[0]);
	match(Identifier);
	match(BinaryOperator, "=");
	assignNode->firstChild->sibling = parseExpression();
	return assignNode;
}

ASTNode* parseParameterList(int index) {
	ASTNode* paramList = newNode(parseToks[0]);
	auto symtabId = new symtableEntry(Argument);
	symtabId->genName = "arg" + to_string(index);
	symtabAdd(parseToks[0]->lexeme, symtabId);
	match(Identifier);
	if (parseToks[0]->type == Comma) {
		match(Comma);
		paramList->sibling = parseParameterList(index + 1);
	}
	return paramList;
}

ASTNode* parseFunctionDeclaration() {
	match(Keyword, "func");
	auto funcDec = newNode(newParseToken(FunctionDeclaration));
	funcDec->firstChild = newNode(parseToks[0]);

	if (varExists(parseToks[0]->lexeme)) {
		string msg = "Variable '";
		msg += parseToks[0]->lexeme;
		msg += "' already declared";
		error(msg, parseToks[0]->line);
	}

	symtableEntry* symtabId = new symtableEntry(Uninitialised);
	symtabId->varType = Function;
	symtabAdd(parseToks[0]->lexeme, symtabId);

	match(Identifier);
	match(LeftParen);
	if (parseToks[0]->type != RightParen) {
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
	newNamespace(parseToks[0]->lexeme);
	match(Identifier);
	return nullptr;
}

ASTNode* parseExpressionList() {
	auto exprList = parseExpression();
	if (parseToks[0]->type == Comma) {
		match(Comma);
		exprList->sibling = parseExpressionList();
	}
	return exprList;
}

ASTNode* parseExpression() {
	ASTNode* expr = parseTerm();
	if (parseToks[0]->type == BinaryOperator && (parseToks[0]->lexeme == "+" || parseToks[0]->lexeme == "-")) {
		auto op = newNode(parseToks[0]);
		op->firstChild = expr;
		match(BinaryOperator);
		expr->sibling = parseExpression();
		return op;
	}
	return expr;
}

ASTNode* parseTerm() {
	ASTNode* term = parseFactor();
	if (parseToks[0]->type == BinaryOperator && (parseToks[0]->lexeme == "*" || parseToks[0]->lexeme == "/")) {
		auto op = newNode(parseToks[0]);
		op->firstChild = term;
		match(BinaryOperator);
		term->sibling = parseTerm();
		return op;
	}
	return term;
}

ASTNode* parseFactor (string ns) {
	ASTNode* factor = nullptr;
	switch (parseToks[0]->type) {
		case Identifier: {
			factor = newNode(parseToks[0]);
			auto identifier = symtabLookup(parseToks[0]->lexeme, ns);
			if ((identifier == nullptr || identifier->varType == Uninitialised) && lookahead(1)->type != Colon) {
				string msg = "Use of undeclared variable '";
				msg += ns;
				msg += ':';
				msg += parseToks[0]->lexeme;
				msg += "'";
				error(msg, parseToks[0]->line);
			}
			match(Identifier);
			if (parseToks[0]->type == LeftParen) {
				auto funcNode = newNode(newParseToken(FunctionCall));
				funcNode->firstChild = factor;
				match(LeftParen);
				if (parseToks[0]->type != RightParen) {
					factor->firstChild = parseExpressionList();
				}
				match(RightParen);
				return funcNode;
			} else if (parseToks[0]->type == Colon) {
				auto colonNode = newNode(parseToks[0]);
				match(Colon);
				colonNode->firstChild = factor;
				colonNode->firstChild->sibling = parseFactor(factor->content->lexeme);
				return colonNode;
			}
		} break;
		case Integer: {
			factor = newNode(parseToks[0]);
			match(Integer);
		} break;
		case String: {
			factor = newNode(parseToks[0]);
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
			msg += parseToks[0]->lexeme;
			msg += "'";
			error(msg, parseToks[0]->line);
			factor = newNode(parseToks[0]);
			match(parseToks[0]->type);
	}
	return factor;
}

#endif // PARSER_H