#include "ast.hpp"

Expr::Expr(Token* tkn) {
    this->token = tkn;
    this->type = nullptr;
}

void Module::print () {
    for (auto expr : this->contents) {
        expr->print(0);
    }
}

void Expr::print(u32 depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "|-";
    match(this->value) {
        holds(Literal, &lit) {
            std::cout << "Literal: \x1b[36m";
            match (lit) {
                holds(int, &integer) {
                    std::cout << "int\x1b[0m => \x1b[32m" << integer << "\x1b[0m\n";
                }
                holds(str, &string) {
                    std::cout << "str\x1b[0m => \x1b[31m\"" << string << "\"\x1b[0m\n";
                }
                _ {}
            }
        }
        holds(Identifier, &ident) {
            std::cout << "Identifier: \x1b[33m'" << ident.name << "'\x1b[0m\n";
        }
        holds(BinaryOp, &binOp) {
            std::cout << "BinaryOp:\n";
            binOp.lhs->print(depth + 1);
            binOp.rhs->print(depth + 1);
        }
        holds(UnaryOp, &unaryOp) {
            std::cout << "UnaryOp:\n";
            unaryOp.operand->print(depth + 1);
        }
        holds(FuncCall, &call) {
            std::cout << "FuncCall:\n";
            call.callee->print(depth + 1);
            for (auto arg : call.args) {
                arg->print(depth + 1);
            }
        }
        _ {}
    }
}