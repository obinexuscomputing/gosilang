#include "sym_type.h"

// Convert token type to string for debugging and error messages
const char* TokenType_toString(enum TokenType type) {
    switch (type) {
        // Literals
        case TOKEN_LITERAL_VALUE: return "LITERAL_VALUE";
        case TOKEN_LITERAL_KEYWORD: return "LITERAL_KEYWORD";
        case TOKEN_LITERAL_DELIMITER: return "LITERAL_DELIMITER";
        case TOKEN_LITERAL_OPERATOR: return "LITERAL_OPERATOR";
        case TOKEN_LITERAL_IDENTIFIER: return "LITERAL_IDENTIFIER";
        case TOKEN_LITERAL_STRING: return "LITERAL_STRING";
        case TOKEN_LITERAL_CHAR: return "LITERAL_CHAR";
        case TOKEN_LITERAL_INTEGER: return "LITERAL_INTEGER";
        case TOKEN_LITERAL_FLOAT: return "LITERAL_FLOAT";
        case TOKEN_LITERAL_BOOL: return "LITERAL_BOOL";

        // Expressions
        case TOKEN_EXPR_BINARY: return "EXPR_BINARY";
        case TOKEN_EXPR_UNARY: return "EXPR_UNARY";
        case TOKEN_EXPR_ASSIGNMENT: return "EXPR_ASSIGNMENT";
        case TOKEN_EXPR_FUNCTION_CALL: return "EXPR_FUNCTION_CALL";
        case TOKEN_EXPR_ARRAY_ACCESS: return "EXPR_ARRAY_ACCESS";

        // Statements
        case TOKEN_STMT_IF: return "STMT_IF";
        case TOKEN_STMT_ELSE: return "STMT_ELSE";
        case TOKEN_STMT_WHILE: return "STMT_WHILE";
        case TOKEN_STMT_FOR: return "STMT_FOR";
        case TOKEN_STMT_RETURN: return "STMT_RETURN";
        case TOKEN_STMT_BREAK: return "STMT_BREAK";
        case TOKEN_STMT_CONTINUE: return "STMT_CONTINUE";

        // Declarations
        case TOKEN_DECL_VARIABLE: return "DECL_VARIABLE";
        case TOKEN_DECL_FUNCTION: return "DECL_FUNCTION";
        case TOKEN_DECL_STRUCT: return "DECL_STRUCT";
        case TOKEN_DECL_ENUM: return "DECL_ENUM";

        // Scope and blocks
        case TOKEN_SCOPE_BEGIN: return "SCOPE_BEGIN";
        case TOKEN_SCOPE_END: return "SCOPE_END";
        case TOKEN_BLOCK_BEGIN: return "BLOCK_BEGIN";
        case TOKEN_BLOCK_END: return "BLOCK_END";

        // Punctuation
        case TOKEN_PUNCT_SEMICOLON: return "PUNCT_SEMICOLON";
        case TOKEN_PUNCT_COMMA: return "PUNCT_COMMA";
        case TOKEN_PUNCT_DOT: return "PUNCT_DOT";
        case TOKEN_PUNCT_COLON: return "PUNCT_COLON";

        // Types
        case TOKEN_TYPE_INT: return "TYPE_INT";
        case TOKEN_TYPE_FLOAT: return "TYPE_FLOAT";
        case TOKEN_TYPE_CHAR: return "TYPE_CHAR";
        case TOKEN_TYPE_VOID: return "TYPE_VOID";
        case TOKEN_TYPE_STRUCT: return "TYPE_STRUCT";

        // Special
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";

        default: return "UNKNOWN_TOKEN_TYPE";
    }
}

// Check if token type is a literal
bool TokenType_isLiteral(enum TokenType type) {
    return (type >= TOKEN_LITERAL_VALUE && type <= TOKEN_LITERAL_BOOL);
}

// Check if token type is an expression
bool TokenType_isExpression(enum TokenType type) {
    return (type >= TOKEN_EXPR_BINARY && type <= TOKEN_EXPR_ARRAY_ACCESS);
}

// Check if token type is a statement
bool TokenType_isStatement(enum TokenType type) {
    return (type >= TOKEN_STMT_IF && type <= TOKEN_STMT_CONTINUE);
}

// Check if token type is a declaration
bool TokenType_isDeclaration(enum TokenType type) {
    return (type >= TOKEN_DECL_VARIABLE && type <= TOKEN_DECL_ENUM);
}

// Check if token type is a type specifier
bool TokenType_isType(enum TokenType type) {
    return (type >= TOKEN_TYPE_INT && type <= TOKEN_TYPE_STRUCT);
}

// Check if token type is a scope/block delimiter
bool TokenType_isScope(enum TokenType type) {
    return (type >= TOKEN_SCOPE_BEGIN && type <= TOKEN_BLOCK_END);
}

// Check if token type is punctuation
bool TokenType_isPunctuation(enum TokenType type) {
    return (type >= TOKEN_PUNCT_SEMICOLON && type <= TOKEN_PUNCT_COLON);
}

// Get precedence level for operator tokens
int TokenType_getOperatorPrecedence(enum TokenType type) {
    if (type == TOKEN_LITERAL_OPERATOR) {
        // You would typically look at the specific operator value
        // This is a simplified example
        return 1;
    }
    return 0;
}

// Check if token type requires a closing match
bool TokenType_requiresClosing(enum TokenType type) {
    switch (type) {
        case TOKEN_SCOPE_BEGIN:
        case TOKEN_BLOCK_BEGIN:
            return true;
        default:
            return false;
    }
}

// Get the corresponding closing token type
enum TokenType TokenType_getClosingType(enum TokenType type) {
    switch (type) {
        case TOKEN_SCOPE_BEGIN:
            return TOKEN_SCOPE_END;
        case TOKEN_BLOCK_BEGIN:
            return TOKEN_BLOCK_END;
        default:
            return TOKEN_ERROR;
    }
}

// Validate token type transitions for bottom-up parsing
bool TokenType_isValidTransition(enum TokenType from, enum TokenType to) {
    // Example transition rules:

    // Can transition from identifier to operator
    if (from == TOKEN_LITERAL_IDENTIFIER && to == TOKEN_LITERAL_OPERATOR)
        return true;

    // Can transition from operator to identifier or literal
    if (from == TOKEN_LITERAL_OPERATOR &&
        (to == TOKEN_LITERAL_IDENTIFIER || TokenType_isLiteral(to)))
        return true;

    // Can transition from type to identifier (for declarations)
    if (TokenType_isType(from) && to == TOKEN_LITERAL_IDENTIFIER)
        return true;

    // Add more transition rules as needed

    return false;
}

// Initialize a new token type context (if needed)
void TokenType_init() {
    // Add any necessary initialization code
}

// Cleanup token type context (if needed)
void TokenType_cleanup() {
    // Add any necessary cleanup code
}
