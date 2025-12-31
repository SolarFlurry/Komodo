#include "parser.hpp"
#include "../base_definitions/ast.hpp"
#include "../helper/help.hpp"
#include "../helper/error.hpp"

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

void consume (TokenType type) {
	printToken(parseToks[0]);
	if (parseToks[0]->type != type) {
		string s = std::format("Unexpected '{}'", (parseToks[0]->type == TOK_EOF ? "EOF" : parseToks[0]->lexeme));
		if (type != TOK_UNKNOWN)
			s += std::format(", expected {}", typeToString(type));
		error(s, parseToks[0]->line, parseToks[0]->col);
	}
	// shift parseToks
	for (int i = 0; i < currentLookahead + 1; i++) {
		parseToks[i] = parseToks[i + 1];
	}
	currentLookahead--;
	if (currentLookahead < 0) {
		currentLookahead = 0;
		parseToks[0] = nextToken(parseLx);
	}
}

/*void match (TokenType type, string lexeme) {
	if (parseToks[0]->type != type && parseToks[0]->lexeme != lexeme) {
		string s = std::format("Unexpected '{}'", (parseToks[0]->type == TOK_EOF ? "EOF" : parseToks[0]->lexeme));
		if (type != TOK_UNKNOWN)
			s += std::format(", expected {}", typeToString(type));
		error(s, parseToks[0]->line, parseToks[0]->col);
	}
	// shift parseToks
	for (int i = 0; i < currentLookahead + 1; i++) {
		parseToks[i] = parseToks[i + 1];
	}
	currentLookahead--;
	if (currentLookahead < 0) currentLookahead = 0;
}*/

Module* parse (Lexer* lx) {
	//newScope();
	parseLx = lx;
	parseToks[0] = nextToken(parseLx);

	Module* program = new Module();

	currentLookahead = 0;

	while (!isEnd(lx)) {
		program->contents.push_back(parseExpression());
	}

	return program;
}

Expr* parseExpression(int minbp) {
	Expr* lhs;
	auto bp = prefixBindingPower(parseToks[0]->type);
	if (parseToks[0]->type == TOK_LPAREN) { // is a grouping
		consume(TOK_LPAREN);
		lhs = parseExpression(0);
		consume(TOK_RPAREN);
	}
	else if (bp.right > 0) { // is a prefix operator
		Token* op = parseToks[0];
		consume(op->type);
		Expr* rhs = parseExpression(bp.right);

		lhs = new Expr(op);
		lhs->value = UnaryOp {
			.op = UnaryOp::Invert,
			.operand = rhs,
		};
	} else { // is an atom
		lhs = parseAtom();
	}
	while (true) {
		Token* op = parseToks[0];
		if (op->type == TOK_EOF) break;
		auto bp = postfixBindingPower(op->type);
		if (bp.left > 0) { // is a postfix operator
			if (bp.left < minbp) break;
			consume(op->type);
			if (op->type == TOK_LPAREN) {
				Expr* rhs = parseExpression(0);
				Expr* temp = new Expr(op);
				temp->value = FuncCall {
					.callee = lhs,
					.args = {rhs},
				};
				lhs = temp;
				consume(TOK_RPAREN);
			} else {
				Expr* temp = new Expr(op);
				temp->value = UnaryOp {
					.op = UnaryOp::Invert,
					.operand = lhs,
				};
				lhs = temp;
			}
			op = parseToks[0];
		}
		bp = infixBindingPower(op->type);
		if (bp.left == 0 && bp.right == 0) {
			break;
		}
		if (bp.left < minbp) {
			break;
		}
		consume(op->type);
		Expr* rhs = parseExpression(bp.right);

		Expr* temp = new Expr(op);
		temp->value = BinaryOp {
			.op = BinaryOp::Add,
			.lhs = lhs,
			.rhs = rhs,
		};

		lhs = temp;
	}
	return lhs;
}

Expr* parseAtom() {
	Expr* atom = new Expr(parseToks[0]);
	if (parseToks[0]->type == TOK_INT) {
		atom->value = (Literal) std::stoi(parseToks[0]->lexeme);
		consume(TOK_INT);
		return atom;
	} else if (parseToks[0]->type == TOK_STRING) {
		atom->value = (Literal) parseToks[0]->lexeme;
		consume(TOK_STRING);
		return atom;
	} else if (parseToks[0]->type == TOK_ID) {
		atom->value = Identifier {
			.name = parseToks[0]->lexeme
		};
		consume(TOK_ID);
		return atom;
	}
	consume(TOK_UNKNOWN);
	return atom;
}

BindingPower infixBindingPower(TokenType op) {
	switch (op) {
		case TOK_PLUS: case TOK_MINUS: return BindingPower(1, 2);
		case TOK_ASTERISK: case TOK_SLASH: return BindingPower(3, 4);
		default: return BindingPower(0, 0);
	}
}

BindingPower prefixBindingPower(TokenType op) {
	switch (op) {
		case TOK_PLUS: case TOK_MINUS: return BindingPower(0, 5);
		default: return BindingPower(0, 0);
	}
}

BindingPower postfixBindingPower(TokenType op) {
	switch (op) {
		case TOK_LPAREN: return BindingPower(7, 0);
		default: return BindingPower(0, 0);
	}
}