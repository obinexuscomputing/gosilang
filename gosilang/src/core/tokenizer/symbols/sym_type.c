#include "sym_type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
