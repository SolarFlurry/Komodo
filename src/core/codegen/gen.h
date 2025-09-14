#ifndef GEN_H
#define GEN_H

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

pair<string, TokenType> genExpression (ASTNode* expr);
pair<string, TokenType> genExpressionNode(ASTNode* expr, char reg);
pair<string, TokenType> genFactor(ASTNode* factor, char reg);

string genStatementList(ASTNode* list);
string genStatement(ASTNode* stmt);
string genCmdStmt(ASTNode* stmt);
string genExecuteStmt(ASTNode* stmt);
string genVarDeclaration(ASTNode* stmt);

#endif // GEN_H