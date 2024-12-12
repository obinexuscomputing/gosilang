#include "core/tokenizer/symbols/sym_type.h"
#include "core/tokenizer/symbols/sym_value.h"
#include <stdio.h>

// Forward declarations of all demonstration functions
void demonstrate_tokens(void);
void demonstrate_symbol_table(void);
void demonstrate_expression_parsing(void);
void demonstrate_value_handling(void);

// Function implementations
void demonstrate_tokens(void) {
    printf("=== Token Demonstration ===\n");

    // Create a few tokens
    Token* id_token = Token_create(TOKEN_LITERAL_IDENTIFIER, "variable_name");
    Token* num_token = Token_create(TOKEN_LITERAL_INTEGER, "42");
    Token* op_token = Token_create(TOKEN_EXPR_BINARY, "+");

    printf("Created tokens:\n");
    Token_print(id_token, stdout);
    printf("\n");
    Token_print(num_token, stdout);
    printf("\n");
    Token_print(op_token, stdout);
    printf("\n");

    // Clean up
    Token_destroy(id_token);
    Token_destroy(num_token);
    Token_destroy(op_token);
}

void demonstrate_symbol_table(void) {
    printf("\n=== Symbol Table Demonstration ===\n");

    // Create a scope hierarchy
    ScopeLevel* global_scope = CreateScope(NULL);
    ScopeLevel* local_scope = CreateScope(global_scope);

    // Create and add symbols
    SymbolTableEntry* var1 = CreateSymbol("x", TOKEN_LITERAL_INTEGER);
    SymbolTableEntry* var2 = CreateSymbol("y", TOKEN_LITERAL_FLOAT);

    printf("Adding symbols to scopes...\n");
    AddSymbol(global_scope, var1);
    AddSymbol(local_scope, var2);

    // Demonstrate symbol lookup
    const char* symbols_to_find[] = {"x", "y", "z"};
    for (int i = 0; i < 3; i++) {
        SymbolTableEntry* found = FindSymbol(local_scope, symbols_to_find[i]);
        if (found) {
            printf("Found symbol '%s' of type %s\n",
                   found->name,
                   TokenType_toString(found->token_type));
        } else {
            printf("Symbol '%s' not found\n", symbols_to_find[i]);
        }
    }

    // Clean up
    DestroyScope(local_scope);
    DestroyScope(global_scope);
}

void demonstrate_expression_parsing(void) {
    printf("\n=== Expression Parsing Demonstration ===\n");

    // Create tokens for expression: "a + b * c"
    Token* tokens[5];
    tokens[0] = Token_create(TOKEN_LITERAL_IDENTIFIER, "a");
    tokens[1] = Token_create(TOKEN_EXPR_BINARY, "+");
    tokens[2] = Token_create(TOKEN_LITERAL_IDENTIFIER, "b");
    tokens[3] = Token_create(TOKEN_EXPR_BINARY, "*");
    tokens[4] = Token_create(TOKEN_LITERAL_IDENTIFIER, "c");

    printf("Original expression tokens:\n");
    for (int i = 0; i < 5; i++) {
        Token_print(tokens[i], stdout);
        printf("\n");
    }

    printf("\nParsing expression: a + b * c\n");
    Token* result = ParseExpression(tokens, 5);

    if (result) {
        printf("Expression parsed successfully:\n");
        Token_print(result, stdout);
        printf("\n");
        Token_destroy(result);
    } else {
        printf("Failed to parse expression\n");
    }

    // Clean up input tokens
    for (int i = 0; i < 5; i++) {
        if (tokens[i]) {
            Token_destroy(tokens[i]);
        }
    }
}

void demonstrate_value_handling(void) {
    printf("\n=== Value Handling Demonstration ===\n");

    // Create different types of values
    LiteralValue* int_val = CreateLiteralValue(VAL_INTEGER);
    int_val->data.int_val = 42;

    LiteralValue* float_val = CreateLiteralValue(VAL_FLOAT);
    float_val->data.float_val = 3.14;

    LiteralValue* str_val = CreateLiteralValue(VAL_STRING);
    str_val->data.string_val = strdup("Hello");

    // Convert values to strings and print
    char* int_str = ValueToString(int_val);
    char* float_str = ValueToString(float_val);
    char* str_str = ValueToString(str_val);

    printf("Integer value: %s\n", int_str);
    printf("Float value: %s\n", float_str);
    printf("String value: %s\n", str_str);

    // Clean up
    free(int_str);
    free(float_str);
    free(str_str);
    DestroyLiteralValue(int_val);
    DestroyLiteralValue(float_val);
    DestroyLiteralValue(str_val);
}

int main(void) {
    printf("Gosilang Symbol System Demonstration\n");
    printf("===================================\n\n");

    demonstrate_tokens();
    demonstrate_symbol_table();
    demonstrate_expression_parsing();
    demonstrate_value_handling();

    printf("\nDemonstration complete.\n");
    return 0;
}
