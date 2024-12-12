#ifndef SYM_TYPE_H
#define SYM_TYPE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token types for bottom-up parsing
enum TokenType {
    // Literals
    TOKEN_LITERAL_VALUE,
    TOKEN_LITERAL_KEYWORD,
    TOKEN_LITERAL_DELIMITER,
    TOKEN_LITERAL_OPERATOR,
    TOKEN_LITERAL_IDENTIFIER,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_CHAR,
    TOKEN_LITERAL_INTEGER,
    TOKEN_LITERAL_FLOAT,
    TOKEN_LITERAL_BOOL,

    // Expressions
    TOKEN_EXPR_BINARY,
    TOKEN_EXPR_UNARY,
    TOKEN_EXPR_ASSIGNMENT,
    TOKEN_EXPR_FUNCTION_CALL,
    TOKEN_EXPR_ARRAY_ACCESS,

    // Statements
    TOKEN_STMT_IF,
    TOKEN_STMT_ELSE,
    TOKEN_STMT_WHILE,
    TOKEN_STMT_FOR,
    TOKEN_STMT_RETURN,
    TOKEN_STMT_BREAK,
    TOKEN_STMT_CONTINUE,

    // Declarations
    TOKEN_DECL_VARIABLE,
    TOKEN_DECL_FUNCTION,
    TOKEN_DECL_STRUCT,
    TOKEN_DECL_ENUM,

    // Scope and blocks
    TOKEN_SCOPE_BEGIN,
    TOKEN_SCOPE_END,
    TOKEN_BLOCK_BEGIN,
    TOKEN_BLOCK_END,

    // Punctuation
    TOKEN_PUNCT_SEMICOLON,
    TOKEN_PUNCT_COMMA,
    TOKEN_PUNCT_DOT,
    TOKEN_PUNCT_COLON,

    // Types
    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_CHAR,
    TOKEN_TYPE_VOID,
    TOKEN_TYPE_STRUCT,

    // Special
    TOKEN_EOF,
    TOKEN_ERROR
};

#endif // SYM_TYPE_H
