
#include "sym_value.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ====== Value Management Functions ======

LiteralValue* CreateLiteralValue(ValueType type) {
    LiteralValue* value = (LiteralValue*)malloc(sizeof(LiteralValue));
    if (!value) return NULL;

    value->type = type;
    value->is_unsigned = false;
    value->bit_width = 32; // default

    // Initialize union based on type
    switch (type) {
        case VAL_INTEGER:
            value->data.int_val = 0;
            break;
        case VAL_FLOAT:
            value->data.float_val = 0.0;
            break;
        case VAL_CHAR:
            value->data.char_val = '\0';
            break;
        case VAL_STRING:
            value->data.string_val = NULL;
            break;
        case VAL_BOOL:
            value->data.bool_val = false;
            break;
        case VAL_NULL:
        case VAL_COMPOUND:
            value->data.compound_val = NULL;
            break;
    }

    return value;
}

void DestroyLiteralValue(LiteralValue* value) {
    if (!value) return;

    if (value->type == VAL_STRING && value->data.string_val) {
        free(value->data.string_val);
    }
    free(value);
}

// ====== Symbol Table Management ======

SymbolTableEntry* CreateSymbol(const char* name, TokenType type) {
    SymbolTableEntry* symbol = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
    if (!symbol) return NULL;

    symbol->name = strdup(name);
    symbol->token_type = type;
    TokenAttributes_init(&symbol->attributes);
    symbol->value.type = VAL_NULL;
    symbol->next = NULL;

    return symbol;
}

void DestroySymbol(SymbolTableEntry* symbol) {
    if (!symbol) return;

    free(symbol->name);
    DestroyLiteralValue(&symbol->value);
    free(symbol);
}

ScopeLevel* CreateScope(ScopeLevel* parent) {
    ScopeLevel* scope = (ScopeLevel*)malloc(sizeof(ScopeLevel));
    if (!scope) return NULL;

    scope->symbols = NULL;
    scope->parent = parent;
    scope->level = parent ? parent->level + 1 : 0;

    return scope;
}

void DestroyScope(ScopeLevel* scope) {
    if (!scope) return;

    // Free all symbols in this scope
    SymbolTableEntry* current = scope->symbols;
    while (current) {
        SymbolTableEntry* next = current->next;
        DestroySymbol(current);
        current = next;
    }

    free(scope);
}

bool AddSymbol(ScopeLevel* scope, SymbolTableEntry* symbol) {
    if (!scope || !symbol) return false;

    // Check for duplicate symbols in current scope
    if (FindSymbol(scope, symbol->name)) {
        return false;
    }

    // Add to front of list
    symbol->next = scope->symbols;
    scope->symbols = symbol;
    return true;
}

SymbolTableEntry* FindSymbol(ScopeLevel* scope, const char* name) {
    if (!scope || !name) return NULL;

    // Look in current scope
    SymbolTableEntry* current = scope->symbols;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    // Look in parent scope if not found
    return scope->parent ? FindSymbol(scope->parent, name) : NULL;
}

// ====== Bottom-up Parser Support Functions ======

// Stack for operator precedence parsing
#define MAX_STACK_SIZE 1000

typedef struct {
    Token* tokens[MAX_STACK_SIZE];
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
    free(stack);
}

static bool PushToken(TokenStack* stack, Token* token) {
    if (stack->top >= MAX_STACK_SIZE - 1) return false;
    stack->tokens[++stack->top] = token;
    return true;
}

static Token* PopToken(TokenStack* stack) {
    if (stack->top < 0) return NULL;
    return stack->tokens[stack->top--];
}

static Token* PeekToken(TokenStack* stack) {
    if (stack->top < 0) return NULL;
    return stack->tokens[stack->top];
}

// Bottom-up parsing helper functions
static bool HasHigherPrecedence(Token* op1, Token* op2) {
    return TokenType_getOperatorPrecedence(op1->type) >
           TokenType_getOperatorPrecedence(op2->type);
}

// Function to reduce tokens based on operator precedence
static Token* ReduceTokens(TokenStack* operandStack, TokenStack* operatorStack) {
    Token* operator = PopToken(operatorStack);
    if (!operator) return NULL;

    // Handle different operator types
    switch (operator->type) {
        case TOKEN_EXPR_BINARY: {
            Token* right = PopToken(operandStack);
            Token* left = PopToken(operandStack);
            if (!right || !left) return NULL;

            // Create new token representing the operation
            Token* result = Token_create(TOKEN_EXPR_BINARY, "");
            if (!result) return NULL;

            // Store operation result
            result->value = right->value; // Simplified - should compute actual value

            // Clean up
            Token_destroy(right);
            Token_destroy(left);

            return result;
        }

        case TOKEN_EXPR_UNARY: {
            Token* operand = PopToken(operandStack);
            if (!operand) return NULL;

            Token* result = Token_create(TOKEN_EXPR_UNARY, "");
            if (!result) return NULL;

            result->value = operand->value; // Simplified
            Token_destroy(operand);

            return result;
        }

        default:
            return NULL;
    }
}

// Main parsing function for expressions
Token* ParseExpression(Token* tokens[], int count) {
    TokenStack* operandStack = CreateStack();
    TokenStack* operatorStack = CreateStack();

    for (int i = 0; i < count; i++) {
        Token* current = tokens[i];

        switch (TokenType_getCategory(current->type)) {
            case TOKEN_CATEGORY_LITERAL: {
                PushToken(operandStack, Token_copy(current));
                break;
            }

            case TOKEN_CATEGORY_EXPRESSION: {
                while (PeekToken(operatorStack) &&
                       HasHigherPrecedence(PeekToken(operatorStack), current)) {
                    Token* result = ReduceTokens(operandStack, operatorStack);
                    if (!result) goto cleanup;
                    PushToken(operandStack, result);
                }
                PushToken(operatorStack, Token_copy(current));
                break;
            }

            default:
                // Handle error or other token types
                break;
        }
    }

    // Process remaining operators
    while (PeekToken(operatorStack)) {
        Token* result = ReduceTokens(operandStack, operatorStack);
        if (!result) goto cleanup;
        PushToken(operandStack, result);
    }

    Token* finalResult = PopToken(operandStack);

cleanup:
    DestroyStack(operandStack);
    DestroyStack(operatorStack);
    return finalResult;
}

// String conversion functions
const char* GetOperatorString(OperatorType op) {
    static const char* operator_strings[] = {
        "+", "-", "*", "/", "%",
        "=", "+=", "-=", "*=", "/=", "%=",
        "&=", "|=", "^=", "<<=", ">>=",
        "&", "|", "^", "~", "<<", ">>",
        "&&", "||", "!",
        "==", "!=", "<", ">", "<=", ">=",
        "++", "--",
        ".", "->",
        "&", "*", "sizeof", ",", "?"
    };

    if (op >= 0 && op < sizeof(operator_strings)/sizeof(operator_strings[0])) {
        return operator_strings[op];
    }
    return "unknown";
}

const char* GetKeywordString(KeywordType kw) {
    static const char* keyword_strings[] = {
        "if", "else", "while", "for", "do",
        "switch", "case", "default", "break",
        "continue", "return", "goto",
        "void", "char", "short", "int", "long",
        "float", "double", "signed", "unsigned",
        "const", "volatile", "restrict",
        "auto", "register", "static", "extern",
        "typedef", "struct", "union", "enum",
        "sizeof", "_Alignof", "inline",
        "static_assert"
    };

    if (kw >= 0 && kw < sizeof(keyword_strings)/sizeof(keyword_strings[0])) {
        return keyword_strings[kw];
    }
    return "unknown";
}

// Value conversion and validation
bool IsValidValue(const LiteralValue* value) {
    if (!value) return false;

    switch (value->type) {
        case VAL_INTEGER:
            return true;  // All integer values are valid
        case VAL_FLOAT:
            return value->data.float_val == value->data.float_val; // Check for NaN
        case VAL_STRING:
            return value->data.string_val != NULL;
        case VAL_CHAR:
            return true;  // All char values are valid
        case VAL_BOOL:
            return true;  // All bool values are valid
        case VAL_NULL:
            return true;  // NULL is always valid
        case VAL_COMPOUND:
            return value->data.compound_val != NULL;
        default:
            return false;
    }
}

char* ValueToString(const LiteralValue* value) {
    if (!value) return NULL;

    char buffer[64];
    switch (value->type) {
        case VAL_INTEGER:
            snprintf(buffer, sizeof(buffer), "%ld", value->data.int_val);
            break;
        case VAL_FLOAT:
            snprintf(buffer, sizeof(buffer), "%f", value->data.float_val);
            break;
        case VAL_CHAR:
            snprintf(buffer, sizeof(buffer), "'%c'", value->data.char_val);
            break;
        case VAL_STRING:
            return value->data.string_val ? strdup(value->data.string_val) : strdup("");
        case VAL_BOOL:
            return strdup(value->data.bool_val ? "true" : "false");
        case VAL_NULL:
            return strdup("null");
        default:
            return strdup("unknown");
    }

    return strdup(buffer);
}
