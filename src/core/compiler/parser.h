#ifndef PARSER_H
#define PARSER_H

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

ASTNode* parseStatementList();
ASTNode* parseStatement();
ASTNode* parseCmdStmt();
ASTNode* parseReturnStmt();
ASTNode* parseExecuteStmt();
ASTNode* parseVarDeclaration();
ASTNode* parseAssignStmt();
ASTNode* parseFunctionDeclaration();
ASTNode* parseExpressionList();
ASTNode* parseExpression();
ASTNode* parseTerm();
ASTNode* parseFactor();

int parserIdx = 0;
Token* parserTok;
vector<Token*> parseToks;

void match (TokenType type) {
	if (parserTok->type != type) {
		string msg = "Unexpected token '";
		msg += parserTok->lexeme;
		msg += "'";
		error(msg, parserTok->line);
	}
	parserIdx++;
	parserTok = parseToks[parserIdx];
}

void match (TokenType type, string lexeme) {
	if (parserTok->type != type && parserTok->lexeme != lexeme) {
		string msg = "Unexpected token '";
		msg += parserTok->lexeme;
		msg += "'";
		error(msg, parserTok->line);
	}
	parserIdx++;
	parserTok = parseToks[parserIdx];
}

ASTNode* parse (vector<Token*> tokList) {
	parseToks = tokList;
	ASTNode* root = new ASTNode();
	root->content = newToken("", Program, 0);
	parserIdx = 0;
	parserTok = parseToks[parserIdx];

	root->firstChild = parseStatementList();

	return root;
}

// possibly returns nullptr
ASTNode* parseStatementList() {
	if (parserTok->type == Eof || parserTok->type == RightBrace) {
		return nullptr;
	}
	auto stmt = parseStatement();
	auto next = parseStatementList();
	if (next != nullptr) {
		stmt->sibling = next;
	}
	return stmt;
}

ASTNode* parseStatement() {
	ASTNode* stmt = nullptr;
	if (parserTok->type == At) {
		stmt = parseCmdStmt();
	} else if (parserTok->type == Keyword) {
		if (parserTok->lexeme == "return") {
			stmt = parseReturnStmt();
		} else if (parserTok->lexeme == "func") {
			stmt = parseFunctionDeclaration();
		} else if (parserTok->lexeme == "const" || parserTok->lexeme == "glob" || parserTok->lexeme == "score") {
			stmt = parseVarDeclaration();
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
	match(At);
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
		varDec->firstChild->sibling = newNode(parserTok);
		symtableEntry* identifier = symtabLookup(parserTok->lexeme);
		if (identifier == nullptr) {
			error("Unknown identifier", parserTok->line);
		} else if (parseToks[parserIdx-1]->lexeme == "const") {
			identifier->type = Constant;
		} else if (parseToks[parserIdx-1]->lexeme == "glob") {
			identifier->type = Global;
		} else if (parseToks[parserIdx-1]->lexeme == "score") {
			identifier->type = Score;
		}
		match(Identifier);
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
	match(BinaryOperator, "=");
	assignNode->firstChild->sibling = parseExpression();
	return assignNode;
}

ASTNode* parseFunctionDeclaration() {
	match(Keyword, "func");
	auto funcDec = newNode(newParseToken(FunctionDeclaration));
	funcDec->firstChild = newNode(parserTok);

	symtableEntry* identifier = symtabLookup(parserTok->lexeme);
	if (identifier == nullptr) {
		error("Unknown identifier", parserTok->line);
	} else {
		identifier->type = Function;
	}

	match(Identifier);
	match(LeftParen);
	match(RightParen);
	funcDec->firstChild->sibling = parseExpression();
	return funcDec;
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

ASTNode* parseFactor () {
	ASTNode* factor = nullptr;
	switch (parserTok->type) {
		case Identifier: {
			factor = newNode(parserTok);
			auto identifier = symtabLookup(parserTok->lexeme);
			if (identifier == nullptr || identifier->type == Uninitialised) {
				string msg = "Variable '";
				msg += parserTok->lexeme;
				msg += "' does not exist";
				error(msg, parserTok->line);
			}
			match(Identifier);
			if (parserTok->type == LeftParen) {
				auto funcNode = newNode(newParseToken(FunctionCall));
				funcNode->firstChild = factor;
				match(LeftParen);
				if (parserTok->type != RightParen) {
					factor->sibling = parseExpressionList();
				}
				match(RightParen);
				return funcNode;
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
			error("Expected expression", parserTok->line);
			factor = newNode(parserTok);
	}
	return factor;
}

#endif // PARSER_H