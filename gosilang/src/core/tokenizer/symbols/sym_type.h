#ifndef SYM_TYPE_H
#define SYM_TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Token;
struct TokenContext;

// Token stack definition
#define MAX_STACK_SIZE 100
typedef struct TokenStack {
    struct Token* items[MAX_STACK_SIZE];
    int top;
} TokenStack;

// Token categories for grouping similar token types
typedef enum {
    TOKEN_CATEGORY_LITERAL,
    TOKEN_CATEGORY_EXPRESSION,
    TOKEN_CATEGORY_STATEMENT,
    TOKEN_CATEGORY_DECLARATION,
    TOKEN_CATEGORY_SCOPE,
    TOKEN_CATEGORY_PUNCTUATION,
    TOKEN_CATEGORY_TYPE,
    TOKEN_CATEGORY_SPECIAL
} TokenCategory;

// Expanded token types for more precise parsing
typedef enum TokenType {
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
    TOKEN_LITERAL_NULL,
    TOKEN_LITERAL_ARRAY,

    // Expressions
    TOKEN_EXPR_BINARY,
    TOKEN_EXPR_UNARY,
    TOKEN_EXPR_ASSIGNMENT,
    TOKEN_EXPR_FUNCTION_CALL,
    TOKEN_EXPR_ARRAY_ACCESS,
    TOKEN_EXPR_MEMBER_ACCESS,
    TOKEN_EXPR_CONDITIONAL,  // Ternary operator
    TOKEN_EXPR_COMMA,
    TOKEN_EXPR_CAST,
    TOKEN_EXPR_SIZEOF,

    // Statements
    TOKEN_STMT_IF,
    TOKEN_STMT_ELSE,
    TOKEN_STMT_WHILE,
    TOKEN_STMT_FOR,
    TOKEN_STMT_DO,
    TOKEN_STMT_SWITCH,
    TOKEN_STMT_CASE,
    TOKEN_STMT_DEFAULT,
    TOKEN_STMT_RETURN,
    TOKEN_STMT_BREAK,
    TOKEN_STMT_CONTINUE,
    TOKEN_STMT_GOTO,
    TOKEN_STMT_LABEL,

    // Declarations
    TOKEN_DECL_VARIABLE,
    TOKEN_DECL_FUNCTION,
    TOKEN_DECL_STRUCT,
    TOKEN_DECL_UNION,
    TOKEN_DECL_ENUM,
    TOKEN_DECL_TYPEDEF,
    TOKEN_DECL_EXTERN,
    TOKEN_DECL_STATIC,
    TOKEN_DECL_AUTO,
    TOKEN_DECL_REGISTER,

    // Scope and blocks
    TOKEN_SCOPE_BEGIN,
    TOKEN_SCOPE_END,
    TOKEN_BLOCK_BEGIN,
    TOKEN_BLOCK_END,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_BRACKET_OPEN,
    TOKEN_BRACKET_CLOSE,

    // Punctuation
    TOKEN_PUNCT_SEMICOLON,
    TOKEN_PUNCT_COMMA,
    TOKEN_PUNCT_DOT,
    TOKEN_PUNCT_COLON,
    TOKEN_PUNCT_ARROW,      // ->
    TOKEN_PUNCT_ELLIPSIS,   // ...

    // Types
    TOKEN_TYPE_VOID,
    TOKEN_TYPE_CHAR,
    TOKEN_TYPE_SHORT,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_LONG,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_DOUBLE,
    TOKEN_TYPE_SIGNED,
    TOKEN_TYPE_UNSIGNED,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_COMPLEX,
    TOKEN_TYPE_STRUCT,
    TOKEN_TYPE_UNION,
    TOKEN_TYPE_ENUM,

    // Special
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_COMMENT_SINGLE,
    TOKEN_COMMENT_MULTI,
    TOKEN_PREPROCESSOR
} TokenType;


// Token attributes for additional type information
typedef struct TokenAttributes {
    bool is_const;
    bool is_volatile;
    bool is_static;
    bool is_extern;
    bool is_signed;
    int pointer_level;    // Number of pointer indirections
    int array_dimensions; // Number of array dimensions
} TokenAttributes;

// Token structure with enhanced information
typedef struct Token {
    TokenType type;
    TokenCategory category;
    char* value;
    int line_number;
    int column_number;
    const char* file_name;
    TokenAttributes attributes;
    struct Token* next;     // For linked list implementation
    struct Token* prev;     // For bidirectional traversal
} Token;
// Context structure for token processing
typedef struct TokenContext {
    Token* current;
    Token* peek;
    int depth;              // For nested structures
    int error_count;
    char* error_message;
    bool in_preprocessing;
    bool in_comment;
} TokenContext;

// Stack operations
TokenStack* CreateStack(void);
void DestroyStack(TokenStack* stack);
bool IsStackEmpty(const TokenStack* stack);
bool PushToken(TokenStack* stack, struct Token* token);
struct Token* PopToken(TokenStack* stack);
struct Token* PeekToken(TokenStack* stack);
int GetOperatorPrecedence(const struct Token* token);

// Function declarations for token type operations
const char* TokenType_toString(TokenType type);
TokenCategory TokenType_getCategory(TokenType type);
bool TokenType_isLiteral(TokenType type);
bool TokenType_isExpression(TokenType type);
bool TokenType_isStatement(TokenType type);
bool TokenType_isDeclaration(TokenType type);
bool TokenType_isType(TokenType type);
bool TokenType_isScope(TokenType type);
bool TokenType_isPunctuation(TokenType type);
int TokenType_getOperatorPrecedence(TokenType type);
bool TokenType_requiresClosing(TokenType type);
TokenType TokenType_getClosingType(TokenType type);
bool TokenType_isValidTransition(TokenType from, TokenType to);

// Token creation and management functions
Token* Token_create(TokenType type, const char* value);
void Token_destroy(Token* token);
void Token_print(const Token* token, FILE* stream);
Token* Token_copy(const Token* token);
Token* ParseExpression(Token* tokens[], int count);

// Context management functions
TokenContext* TokenContext_create(void);
void TokenContext_destroy(TokenContext* context);
bool TokenContext_advance(TokenContext* context);
Token* TokenContext_peek(TokenContext* context, int ahead);
void TokenContext_setError(TokenContext* context, const char* message);

// Attribute management functions
void TokenAttributes_init(TokenAttributes* attrs);
void TokenAttributes_copy(TokenAttributes* dest, const TokenAttributes* source);
bool TokenAttributes_equals(const TokenAttributes* a1, const TokenAttributes* a2);

// Error handling macros
#define TOKEN_ERROR_BUFFER_SIZE 256
#define TOKEN_SET_ERROR(ctx, fmt, ...) \
    do { \
        char buffer[TOKEN_ERROR_BUFFER_SIZE]; \
        snprintf(buffer, TOKEN_ERROR_BUFFER_SIZE, fmt, ##__VA_ARGS__); \
        TokenContext_setError(ctx, buffer); \
    } while(0)

#endif // SYM_TYPE_H
