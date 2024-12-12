#include "sym_type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Token type to string conversion
const char* TokenType_toString(TokenType type) {
    switch (type) {
        case TOKEN_LITERAL_VALUE: return "VALUE";
        case TOKEN_LITERAL_KEYWORD: return "KEYWORD";
        case TOKEN_LITERAL_DELIMITER: return "DELIMITER";
        case TOKEN_LITERAL_OPERATOR: return "OPERATOR";
        case TOKEN_LITERAL_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_LITERAL_STRING: return "STRING";
        case TOKEN_LITERAL_CHAR: return "CHAR";
        case TOKEN_LITERAL_INTEGER: return "INTEGER";
        case TOKEN_LITERAL_FLOAT: return "FLOAT";
        case TOKEN_LITERAL_BOOL: return "BOOL";
        case TOKEN_LITERAL_NULL: return "NULL";
        case TOKEN_LITERAL_ARRAY: return "ARRAY";
        case TOKEN_EXPR_BINARY: return "BINARY_EXPR";
        case TOKEN_EXPR_UNARY: return "UNARY_EXPR";
        // Add other cases as needed
        default: return "UNKNOWN";
    }
}

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

    // Initialize attributes
    memset(&token->attributes, 0, sizeof(TokenAttributes));
    token->attributes.pointer_level = 0;
    token->attributes.array_dimensions = 0;

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
    if (!copy) return NULL;

    copy->category = source->category;
    copy->line_number = source->line_number;
    copy->column_number = source->column_number;
    copy->file_name = source->file_name;

    // Copy attributes
    copy->attributes = source->attributes;

    return copy;
}

// Token attribute initialization
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

// Stack operations implementation
TokenStack* CreateStack(void) {
    TokenStack* stack = (TokenStack*)malloc(sizeof(TokenStack));
    if (stack) {
        stack->top = -1;
    }
    return stack;
}

void DestroyStack(TokenStack* stack) {
    if (!stack) return;
    // Note: We don't destroy the tokens here as they're managed elsewhere
    free(stack);
}

bool IsStackEmpty(const TokenStack* stack) {
    return (!stack || stack->top < 0);
}

bool PushToken(TokenStack* stack, Token* token) {
    if (!stack || !token || stack->top >= MAX_STACK_SIZE - 1) {
        return false;
    }
    stack->items[++stack->top] = token;
    return true;
}

Token* PopToken(TokenStack* stack) {
    if (IsStackEmpty(stack)) {
        return NULL;
    }
    return stack->items[stack->top--];
}

Token* PeekToken(TokenStack* stack) {
    if (IsStackEmpty(stack)) {
        return NULL;
    }
    return stack->items[stack->top];
}

int GetOperatorPrecedence(const Token* token) {
    if (!token || token->type != TOKEN_EXPR_BINARY) {
        return -1;
    }

    if (strcmp(token->value, "*") == 0 ||
        strcmp(token->value, "/") == 0) {
        return 2;
    }
    if (strcmp(token->value, "+") == 0 ||
        strcmp(token->value, "-") == 0) {
        return 1;
    }
    return 0;
}

// Add tree visualization aids
static void PrintExpressionTree(Token* token, int depth) {
    if (!token) return;

    // Print indentation
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Print current token
    printf("├── ");
    Token_print(token, stdout);
    printf("\n");
}

// Update CreateBinaryOpResult to maintain tree structure
static Token* CreateBinaryOpResult(Token* left, Token* operator, Token* right) {
    if (!left || !operator || !right) return NULL;

    Token* result = Token_create(operator->type, operator->value);
    if (!result) return NULL;

    // Store the operation components for visualization
    result->prev = left;   // Use prev to store left operand
    result->next = right;  // Use next to store right operand

    return result;
}

// Update Token_print to include more information
void Token_print(const Token* token, FILE* stream) {
    if (!token) return;

    const char* type_str = TokenType_toString(token->type);
    fprintf(stream, "Token{type=%s, value='%s', line=%d, col=%d}",
            type_str,
            token->value ? token->value : "null",
            token->line_number,
            token->column_number);
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

    printf("\nBuilding expression tree...\n");

    for (int i = 0; i < count; i++) {
        Token* current = tokens[i];
        if (!current) continue;

        switch (current->type) {
            case TOKEN_LITERAL_IDENTIFIER:
            case TOKEN_LITERAL_INTEGER:
            case TOKEN_LITERAL_FLOAT:
                // Push operands directly to operand stack
                PushToken(operandStack, Token_copy(current));
                printf("Pushed operand: ");
                Token_print(current, stdout);
                printf("\n");
                break;

            case TOKEN_EXPR_BINARY: {
                // Process operators according to precedence
                while (!IsStackEmpty(operatorStack) &&
                       GetOperatorPrecedence(PeekToken(operatorStack)) >=
                       GetOperatorPrecedence(current)) {

                    Token* op = PopToken(operatorStack);
                    Token* right = PopToken(operandStack);
                    Token* left = PopToken(operandStack);

                    printf("Reducing: ");
                    Token_print(left, stdout);
                    printf(" %s ", op->value);
                    Token_print(right, stdout);
                    printf("\n");

                    Token* result = CreateBinaryOpResult(left, op, right);

                    if (!result || !PushToken(operandStack, result)) {
                        Token_destroy(result);
                        goto cleanup;
                    }
                }
                PushToken(operatorStack, Token_copy(current));
                printf("Pushed operator: %s\n", current->value);
                break;
            }

            default:
                printf("Invalid token type encountered\n");
                goto cleanup;
        }
    }

    // Process remaining operators
    while (!IsStackEmpty(operatorStack)) {
        Token* op = PopToken(operatorStack);
        Token* right = PopToken(operandStack);
        Token* left = PopToken(operandStack);

        printf("Final reduction: ");
        Token_print(left, stdout);
        printf(" %s ", op->value);
        Token_print(right, stdout);
        printf("\n");

        Token* result = CreateBinaryOpResult(left, op, right);

        if (!result || !PushToken(operandStack, result)) {
            Token_destroy(result);
            goto cleanup;
        }
    }

    Token* result = PopToken(operandStack);

    if (!IsStackEmpty(operandStack)) {
        printf("Error: Extra operands remain on stack\n");
        Token_destroy(result);
        result = NULL;
    }

    if (result) {
        printf("\nFinal expression tree:\n");
        PrintExpressionTree(result, 0);
    }

cleanup:
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
