#pragma once

#include "../utils/include.hpp"
#include "../lexer/lexer.hpp"
#include "./type.hpp"

struct Expr;

struct Module {
	vec<Expr*> contents = vec<Expr*>();
	void print ();
};

struct Identifier {
	str_view name;
};

struct RValue {
	Identifier ident;
};

struct VarDecl {
	Identifier variable;
	enum Kind {
		Normal,
		Const,
		Global,
		FuncParam,
		MemberVar,
	} kind;
};

struct FuncDecl {
	enum Kind {
		Normal,
		MemberFunc,
	} kind;
	Identifier func;
	Type* returnType;
	vec<Identifier*> params;
	vec<Expr*> body;
};

typedef std::variant<int, float, bool, char, str> Literal;

struct BinaryOp {
	enum Ops {
		Add,
		Sub,
		Mul,
		Div,
		Mod,
		Equals,
		Nequals,
		Greater,
		Less,
		GreaterEq,
		LessEq,
		Assign,
		And,
		Or,
	} op;
	Expr* lhs;
	Expr* rhs;
};

struct UnaryOp {
	enum Ops {
		Invert,
		Not,
	} op;
	Expr* operand;
};

struct FuncCall {
	Expr* callee;
	vec<Expr*> args;
};

struct Expr {
	std::variant<Literal, Identifier, BinaryOp, UnaryOp, FuncCall> value;
	Type* type;
	Token* token;
	Expr (Token* tkn);
	void print(u32 depth);
};