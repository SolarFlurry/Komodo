#ifndef GEN_H
#define GEN_H

#include "../token.h"
#include "../ast.h"
#include "../error.h"
#include "../symtable.h"

string checkExecute();

pair<string, TokenType> genExpression (ASTNode* expr);
pair<string, TokenType> genExpressionNode(ASTNode* expr, char reg);
pair<string, TokenType> genFactor(ASTNode* factor, char reg, string ns = nameSpaces.back());

string genStatementList(ASTNode* list);
string genStatement(ASTNode* stmt);
string genCmdStmt(ASTNode* stmt);
string genImportStmt(ASTNode* stmt);
string genExecuteStmt(ASTNode* stmt);
string genVarDeclaration(ASTNode* stmt);
string genAssignStatement(ASTNode* stmt);

string genFuncDeclaration(ASTNode* stmt);
string genFunctionCall(ASTNode* stmt, string ns = nameSpaces.back());
string genFunctionArguments(ASTNode* stmt, int index);

#endif // GEN_H