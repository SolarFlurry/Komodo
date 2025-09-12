#ifndef PARSER_H
#define PARSER_H

#include "../token.h"
#include "../ast.h"
#include "../error.h"

ASTNode* parseStatementList();
ASTNode* parseStatement();
ASTNode* parseCmdStmt();
ASTNode* parseReturnStmt();
ASTNode* parseExecuteStmt();
ASTNode* parseExpression();
ASTNode* parseTerm();
ASTNode* parseFactor();

int parserIdx = 0;
Token* parserTok;
vector<Token*> parseToks;

void match (TokenType type) {
	if (parserTok->type != type) {
		error("Unexpected token", parserTok->line);
	}
	parserIdx++;
	parserTok = parseToks[parserIdx];
}

void match (TokenType type, string lexeme) {
	if (parserTok->type != type && parserTok->lexeme != lexeme) {
		error("Unexpected token", parserTok->line);
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
		} else {
			stmt = parseExecuteStmt();
		}
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
			match(Identifier);
			if (parserTok->type == LeftParen) {
				match(LeftParen);
				if (parserTok->type != RightParen) {
					//parseExpressionList()
				}
				match(RightParen);
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