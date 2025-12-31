#include "../utils/include.hpp"
#include "../lexer/lexer.hpp"

struct Identifier {
	str name = "";
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
	ASTNode* returnType;
	vec<ASTNode*> params;
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
	ASTNode* lhs;
	ASTNode* rhs;
};

struct UnaryOp {
	enum Ops {
		Invert,
		Not,
	} op;
	ASTNode* operand;
};

struct ASTNode {
	std::variant<VarDecl> contents;
	Token token;
};