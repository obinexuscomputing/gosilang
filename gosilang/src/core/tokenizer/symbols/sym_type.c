#include "sym_type.h"

// Token creation and management
Token* Token_create(TokenType type, const char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (!token) return NULL;

    token->type = type;
    token->category = TokenType_getCategory(type);
    token->value = value ? strdup(value) : NULL;
    token->line_number = 0;
    token->column_number = 0;
    token->file_name = NULL;
    token->next = NULL;
    token->prev = NULL;

    return token;
}

void Token_destroy(Token* token) {
    if (!token) return;
    if (token->value) free(token->value);
    free(token);
}

Token* Token_copy(const Token* source) {
    if (!source) return NULL;
    Token* copy = Token_create(source->type, source->value);
    if (copy) {
        copy->line_number = source->line_number;
        copy->column_number = source->column_number;
        copy->file_name = source->file_name;
    }
    return copy;
}

void Token_print(const Token* token, FILE* stream) {
    if (!token) return;
    fprintf(stream, "Token{type=%d, value='%s', line=%d, col=%d}",
            token->type,
            token->value ? token->value : "null",
            token->line_number,
            token->column_number);
}

// Token category mapping
TokenCategory TokenType_getCategory(TokenType type) {
    if (type >= TOKEN_LITERAL_VALUE && type <= TOKEN_LITERAL_ARRAY)
        return TOKEN_CATEGORY_LITERAL;
    if (type >= TOKEN_EXPR_BINARY && type <= TOKEN_EXPR_SIZEOF)
        return TOKEN_CATEGORY_EXPRESSION;
    if (type >= TOKEN_STMT_IF && type <= TOKEN_STMT_LABEL)
        return TOKEN_CATEGORY_STATEMENT;
    if (type >= TOKEN_DECL_VARIABLE && type <= TOKEN_DECL_REGISTER)
        return TOKEN_CATEGORY_DECLARATION;
    if (type >= TOKEN_SCOPE_BEGIN && type <= TOKEN_BRACKET_CLOSE)
        return TOKEN_CATEGORY_SCOPE;
    if (type >= TOKEN_PUNCT_SEMICOLON && type <= TOKEN_PUNCT_ELLIPSIS)
        return TOKEN_CATEGORY_PUNCTUATION;
    if (type >= TOKEN_TYPE_VOID && type <= TOKEN_TYPE_ENUM)
        return TOKEN_CATEGORY_TYPE;
    return TOKEN_CATEGORY_SPECIAL;
}

// Convert token type to string
const char* TokenType_toString(TokenType type) {
    static const char* token_names[] = {
        // Literals
        "VALUE", "KEYWORD", "DELIMITER", "OPERATOR", "IDENTIFIER",
        "STRING", "CHAR", "INTEGER", "FLOAT", "BOOL", "NULL", "ARRAY",

        // Expressions
        "BINARY_EXPR", "UNARY_EXPR", "ASSIGNMENT_EXPR", "FUNCTION_CALL",
        "ARRAY_ACCESS", "MEMBER_ACCESS", "CONDITIONAL", "COMMA_EXPR",
        "CAST_EXPR", "SIZEOF_EXPR",

        // Statements
        "IF", "ELSE", "WHILE", "FOR", "DO", "SWITCH", "CASE", "DEFAULT",
        "RETURN", "BREAK", "CONTINUE", "GOTO", "LABEL",

        // Declarations
        "VARIABLE_DECL", "FUNCTION_DECL", "STRUCT_DECL", "UNION_DECL",
        "ENUM_DECL", "TYPEDEF_DECL", "EXTERN_DECL", "STATIC_DECL",
        "AUTO_DECL", "REGISTER_DECL",

        // Scope and blocks
        "SCOPE_BEGIN", "SCOPE_END", "BLOCK_BEGIN", "BLOCK_END",
        "PAREN_OPEN", "PAREN_CLOSE", "BRACKET_OPEN", "BRACKET_CLOSE",

        // Punctuation
        "SEMICOLON", "COMMA", "DOT", "COLON", "ARROW", "ELLIPSIS",

        // Types
        "VOID", "CHAR", "SHORT", "INT", "LONG", "FLOAT", "DOUBLE",
        "SIGNED", "UNSIGNED", "BOOL", "COMPLEX", "STRUCT", "UNION", "ENUM",

        // Special
        "EOF", "ERROR", "SINGLE_COMMENT", "MULTI_COMMENT", "PREPROCESSOR"
    };

    if (type >= 0 && type < sizeof(token_names)/sizeof(token_names[0])) {
        return token_names[type];
    }
    return "UNKNOWN";
}


bool Token_equals(const Token* t1, const Token* t2) {
    if (!t1 || !t2) return false;

    return t1->type == t2->type &&
           strcmp(t1->value ? t1->value : "", t2->value ? t2->value : "") == 0 &&
           TokenAttributes_equals(&t1->attributes, &t2->attributes);
}



// Context management
TokenContext* TokenContext_create(void) {
    TokenContext* context = (TokenContext*)malloc(sizeof(TokenContext));
    if (!context) return NULL;

    context->current = NULL;
    context->peek = NULL;
    context->depth = 0;
    context->error_count = 0;
    context->error_message = NULL;
    context->in_preprocessing = false;
    context->in_comment = false;

    return context;
}

void TokenContext_destroy(TokenContext* context) {
    if (context) {
        free(context->error_message);
        free(context);
    }
}

bool TokenContext_advance(TokenContext* context) {
    if (!context || !context->peek) return false;

    context->current = context->peek;
    context->peek = context->peek->next;
    return true;
}

Token* TokenContext_peek(TokenContext* context, int ahead) {
    if (!context || ahead < 0) return NULL;

    Token* current = context->peek;
    for (int i = 0; i < ahead && current; i++) {
        current = current->next;
    }
    return current;
}

void TokenContext_setError(TokenContext* context, const char* message) {
    if (!context) return;

    free(context->error_message);
    context->error_message = strdup(message);
    context->error_count++;
}


// Attribute management
void TokenAttributes_init(TokenAttributes* attrs) {
    if (!attrs) return;

    attrs->is_const = false;
    attrs->is_volatile = false;
    attrs->is_static = false;
    attrs->is_extern = false;
    attrs->is_signed = true;
    attrs->pointer_level = 0;
    attrs->array_dimensions = 0;
}

void TokenAttributes_copy(TokenAttributes* dest, const TokenAttributes* source) {
    if (!dest || !source) return;

    *dest = *source;
}

bool TokenAttributes_equals(const TokenAttributes* a1, const TokenAttributes* a2) {
    if (!a1 || !a2) return false;

    return a1->is_const == a2->is_const &&
           a1->is_volatile == a2->is_volatile &&
           a1->is_static == a2->is_static &&
           a1->is_extern == a2->is_extern &&
           a1->is_signed == a2->is_signed &&
           a1->pointer_level == a2->pointer_level &&
           a1->array_dimensions == a2->array_dimensions;
}

// Type checking functions remain similar but with updated types
bool TokenType_isLiteral(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_LITERAL;
}

bool TokenType_isExpression(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_EXPRESSION;
}

bool TokenType_isStatement(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_STATEMENT;
}

bool TokenType_isDeclaration(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_DECLARATION;
}

bool TokenType_isType(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_TYPE;
}

bool TokenType_isScope(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_SCOPE;
}

bool TokenType_isPunctuation(TokenType type) {
    return TokenType_getCategory(type) == TOKEN_CATEGORY_PUNCTUATION;
}

int TokenType_getOperatorPrecedence(TokenType type) {
    // Simplified precedence table
    switch (type) {
        case TOKEN_EXPR_MEMBER_ACCESS:
            return 13;
        case TOKEN_EXPR_FUNCTION_CALL:
        case TOKEN_EXPR_ARRAY_ACCESS:
            return 12;
        case TOKEN_EXPR_UNARY:
            return 11;
        case TOKEN_EXPR_CAST:
            return 10;
        case TOKEN_EXPR_BINARY:
            return 4;  // Varies based on actual operator
        case TOKEN_EXPR_CONDITIONAL:
            return 3;
        case TOKEN_EXPR_ASSIGNMENT:
            return 2;
        case TOKEN_EXPR_COMMA:
            return 1;
        default:
            return 0;
    }
}

bool TokenType_requiresClosing(TokenType type) {
    switch (type) {
        case TOKEN_SCOPE_BEGIN:
        case TOKEN_BLOCK_BEGIN:
        case TOKEN_PAREN_OPEN:
        case TOKEN_BRACKET_OPEN:
            return true;
        default:
            return false;
    }
}

TokenType TokenType_getClosingType(TokenType type) {
    switch (type) {
        case TOKEN_SCOPE_BEGIN:
            return TOKEN_SCOPE_END;
        case TOKEN_BLOCK_BEGIN:
            return TOKEN_BLOCK_END;
        case TOKEN_PAREN_OPEN:
            return TOKEN_PAREN_CLOSE;
        case TOKEN_BRACKET_OPEN:
            return TOKEN_BRACKET_CLOSE;
        default:
            return TOKEN_ERROR;
    }
}

bool TokenType_isValidTransition(TokenType from, TokenType to) {
    // Implement more sophisticated transition rules here
    // This is a basic implementation that should be expanded based on language grammar

    // Can't transition to error or EOF except in special cases
    if (to == TOKEN_ERROR || to == TOKEN_EOF)
        return false;

    // Can't transition from EOF
    if (from == TOKEN_EOF)
        return false;

    // Basic transitions based on categories
    TokenCategory fromCat = TokenType_getCategory(from);
    TokenCategory toCat = TokenType_getCategory(to);

    switch (fromCat) {
        case TOKEN_CATEGORY_TYPE:
            return toCat == TOKEN_CATEGORY_LITERAL ||
                   to == TOKEN_PUNCT_SEMICOLON ||
                   to == TOKEN_PUNCT_COMMA;

        case TOKEN_CATEGORY_LITERAL:
            return toCat == TOKEN_CATEGORY_EXPRESSION ||
                   toCat == TOKEN_CATEGORY_PUNCTUATION;

        case TOKEN_CATEGORY_EXPRESSION:
            return toCat == TOKEN_CATEGORY_PUNCTUATION ||
                   toCat == TOKEN_CATEGORY_EXPRESSION;

        case TOKEN_CATEGORY_PUNCTUATION:
            return toCat == TOKEN_CATEGORY_LITERAL ||
                   toCat == TOKEN_CATEGORY_TYPE ||
                   toCat == TOKEN_CATEGORY_EXPRESSION;

        default:
            return true; // Allow other transitions by default
    }
}

// Stack implementation for expression parsing
#define MAX_STACK_SIZE 100

typedef struct {
    Token* items[MAX_STACK_SIZE];
    int top;
} TokenStack;

static TokenStack* CreateStack() {
    TokenStack* stack = (TokenStack*)malloc(sizeof(TokenStack));
    if (stack) {
        stack->top = -1;
    }
    return stack;
}

static void DestroyStack(TokenStack* stack) {
    if (!stack) return;
    // Don't destroy tokens here as they're managed elsewhere
    free(stack);
}

static bool IsStackEmpty(TokenStack* stack) {
    return stack->top < 0;
}

static bool PushToken(TokenStack* stack, Token* token) {
    if (stack->top >= MAX_STACK_SIZE - 1) return false;
    stack->items[++stack->top] = token;
    return true;
}

static Token* PopToken(TokenStack* stack) {
    if (IsStackEmpty(stack)) return NULL;
    return stack->items[stack->top--];
}

static Token* PeekToken(TokenStack* stack) {
    if (IsStackEmpty(stack)) return NULL;
    return stack->items[stack->top];
}

// Get operator precedence
static int GetOperatorPrecedence(Token* token) {
    if (!token) return -1;

    switch (token->type) {
        case TOKEN_EXPR_BINARY:
            if (strcmp(token->value, "*") == 0 ||
                strcmp(token->value, "/") == 0) {
                return 2;
            }
            if (strcmp(token->value, "+") == 0 ||
                strcmp(token->value, "-") == 0) {
                return 1;
            }
            break;
        default:
            break;
    }
    return 0;
}

// Create a new token for a binary operation result
static Token* CreateBinaryOpResult(Token* left, Token* operator, Token* right) {
    if (!left || !operator || !right) return NULL;

    // Create a new token representing the operation
    Token* result = Token_create(TOKEN_EXPR_BINARY, operator->value);
    if (!result) return NULL;

    // Store additional information if needed
    result->line_number = operator->line_number;
    result->column_number = operator->column_number;

    return result;
}

// Enhanced ParseExpression implementation
Token* ParseExpression(Token* tokens[], int count) {
    if (!tokens || count <= 0) return NULL;

    TokenStack* operandStack = CreateStack();
    TokenStack* operatorStack = CreateStack();

    if (!operandStack || !operatorStack) {
        DestroyStack(operandStack);
        DestroyStack(operatorStack);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        Token* current = tokens[i];
        if (!current) continue;

        switch (current->type) {
            case TOKEN_LITERAL_IDENTIFIER:
            case TOKEN_LITERAL_INTEGER:
            case TOKEN_LITERAL_FLOAT:
                // Push operands directly to operand stack
                PushToken(operandStack, Token_copy(current));
                break;

            case TOKEN_EXPR_BINARY: {
                // Process operators according to precedence
                while (!IsStackEmpty(operatorStack) &&
                       GetOperatorPrecedence(PeekToken(operatorStack)) >=
                       GetOperatorPrecedence(current)) {

                    Token* op = PopToken(operatorStack);
                    Token* right = PopToken(operandStack);
                    Token* left = PopToken(operandStack);

                    Token* result = CreateBinaryOpResult(left, op, right);

                    Token_destroy(left);
                    Token_destroy(right);
                    Token_destroy(op);

                    if (!result || !PushToken(operandStack, result)) {
                        // Cleanup on error
                        Token_destroy(result);
                        goto cleanup;
                    }
                }
                PushToken(operatorStack, Token_copy(current));
                break;
            }

            default:
                // Invalid token type for expression
                goto cleanup;
        }
    }

    // Process remaining operators
    while (!IsStackEmpty(operatorStack)) {
        Token* op = PopToken(operatorStack);
        Token* right = PopToken(operandStack);
        Token* left = PopToken(operandStack);

        Token* result = CreateBinaryOpResult(left, op, right);

        Token_destroy(left);
        Token_destroy(right);
        Token_destroy(op);

        if (!result || !PushToken(operandStack, result)) {
            Token_destroy(result);
            goto cleanup;
        }
    }

    // Get final result
    Token* result = PopToken(operandStack);

    // Check if there are any remaining tokens (error condition)
    if (!IsStackEmpty(operandStack)) {
        Token_destroy(result);
        result = NULL;
    }

cleanup:
    // Clean up any remaining tokens in stacks
    while (!IsStackEmpty(operandStack)) {
        Token_destroy(PopToken(operandStack));
    }
    while (!IsStackEmpty(operatorStack)) {
        Token_destroy(PopToken(operatorStack));
    }

    DestroyStack(operandStack);
    DestroyStack(operatorStack);

    return result;
}
