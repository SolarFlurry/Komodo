#ifndef EXPRESSION_H
#define EXPRESSION_H

pair<string, TokenType> genExpression(ASTNode* expr) {
	return genExpressionNode(expr, '0');
}

pair<string, TokenType> genExpressionNode(ASTNode* expr, char reg) {
	if (expr->content->type == BinaryOperator) {
		auto [instr1, firstType] = genExpressionNode(expr->firstChild, '1');
		auto [instr2, secondType] = genExpressionNode(expr->firstChild->sibling, '2');
		if (firstType != secondType) {
			fatalError("Operand types differ", expr->content->line);
		}
		if (firstType == String) {
			return {instr1 + instr2, String};
		} else {
			string regOp = "scoreboard players operation R";
			regOp += reg;
			regOp += " .komodo ";
			string instr;
			instr += instr1;
			instr += instr2;
			instr += checkExecute();
			instr += regOp;
			instr += "= R1 .komodo\n";
			instr += checkExecute();
			instr += regOp;
			if (expr->content->lexeme == "+") {
				instr += "+= R2 .komodo";
			} else if (expr->content->lexeme == "-") {
				instr += "-= R2 .komodo";
			} else if (expr->content->lexeme == "*") {
				instr += "*= R2 .komodo";
			} else if (expr->content->lexeme == "/") {
				instr += " /= R2 .komodo";
			}
			instr += '\n';
			return {instr, firstType};
		}
	} else {
		return genFactor(expr, reg);
	}
}

pair<string, TokenType> genFactor(ASTNode* factor, char reg) {
	if (factor->content->type == Integer) {
		string instr = checkExecute();
		instr += "scoreboard players set R";
		instr += reg;
		instr += " .komodo ";
		instr += factor->content->lexeme;
		instr += '\n';
		return {instr, factor->content->type};
	} else if (factor->content->type == Identifier) {
		auto symtabId = symtabLookup(factor->content->lexeme);
		if (symtabId == nullptr) {
			fatalError("Variable is not defined", factor->content->line);
		}
		if (symtabId->varType == Global) {
			string instr = checkExecute();
			instr += "scoreboard players operation R";
			instr += reg;
			instr += " .komodo = ";
			instr += symtabId->genName;
			instr += " .global";
			instr += '\n';
			return {instr, symtabId->type};
		} else if (symtabId->varType == Score) {
			string instr = checkExecute();
			instr += "scoreboard players operation R";
			instr += reg;
			instr += " .komodo = @s";
			instr += symtabId->genName;
			instr += '\n';
			return {instr, symtabId->type};
		} else if (symtabId->varType == Argument) {
			string instr = checkExecute();
			instr += "scoreboard players operation R";
			instr += reg;
			instr += " .komodo = ";
			instr += symtabId->genName;
			instr += " .komodo";
			instr += '\n';
			return {instr, symtabId->type};
		} else {
			fatalError("Variable is not defined", factor->content->line);
			return {"", SyntaxError};
		}
	} else if (factor->content->type == FunctionCall) {
		return {genFunctionCall(factor), Integer};
	} else if (factor->content->type == ComplexExpression) {
		return {genStatementList(factor->firstChild), Integer};
	} else {
		return {factor->content->lexeme, factor->content->type};
	}
}

#endif // EXPRESSION_H