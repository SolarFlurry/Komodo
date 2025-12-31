#include "../utils/include.hpp"

enum TokenType {
    #define TokenType_Enum                                                  \
    TOK_INT, TOK_STRING, TOK_ID,                                            \
                                                                            \
    TOK_KEYWORD_SCORE, TOK_KEYWORD_GLOB, TOK_KEYWORD_CONST,                 \
    TOK_KEYWORD_FUNC,                                                       \
    TOK_KEYWORD_IF, TOK_KEYWORD_AS, TOK_KEYWORD_AT, TOK_KEYWORD_UNLESS,     \
    TOK_KEYWORD_IMPORT, TOK_KEYWORD_NAMESPACE,                              \
                                                                            \
    TOK_PLUS, TOK_MINUS, TOK_ASTERISK, TOK_SLASH, TOK_PERCENT,              \
    TOK_EQ,                                                                 \
    TOK_EQ_EQ, TOK_RARROW_EQ, TOK_LARROW_EQ, TOK_BANG_EQ,                   \
    TOK_AMP_AMP, TOK_PIPE_PIPE,                                             \
    TOK_LPAREN, TOK_RPAREN,                                                 \
    TOK_LBRACE, TOK_RBRACE,                                                 \
    TOK_LBRACK, TOK_RBRACK,                                                 \
    TOK_LARROW, TOK_RARROW,                                                 \
    TOK_SEMICOLON, TOK_COLON,                                               \
    TOK_AT,                                                                 \
    TOK_COMMA,                                                              \
                                                                            \
    TOK_EOF,                                                                \
    TOK_UNKNOWN,                                                            \
                                                                            \
    SyntaxError,

    TokenType_Enum
};

struct Token {
    str value;
    TokenType type;
    u32 line;
    u32 col;
};

#undef TokenType_Enum