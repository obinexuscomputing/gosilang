#include "sym_value.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Value Management Functions
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

// Symbol Table Management
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
    if (symbol->value.type == VAL_STRING && symbol->value.data.string_val) {
        free(symbol->value.data.string_val);
    }
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

    if (FindSymbol(scope, symbol->name)) {
        return false;
    }

    symbol->next = scope->symbols;
    scope->symbols = symbol;
    return true;
}

SymbolTableEntry* FindSymbol(ScopeLevel* scope, const char* name) {
    if (!scope || !name) return NULL;

    SymbolTableEntry* current = scope->symbols;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return scope->parent ? FindSymbol(scope->parent, name) : NULL;
}

bool IsValidValue(const LiteralValue* value) {
    if (!value) return false;

    switch (value->type) {
        case VAL_INTEGER:
            return true;
        case VAL_FLOAT:
            return value->data.float_val == value->data.float_val; // NaN check
        case VAL_STRING:
            return value->data.string_val != NULL;
        case VAL_CHAR:
        case VAL_BOOL:
        case VAL_NULL:
            return true;
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
