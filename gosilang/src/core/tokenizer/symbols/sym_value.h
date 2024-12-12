#ifndef SYM_VALUE_H
#define SYM_VALUE_H

#include "sym_type.h"
#include <stdint.h>

// Operator types
typedef enum {
    // Arithmetic operators
    OP_ADD,          // +
    OP_SUBTRACT,     // -
    OP_MULTIPLY,     // *
    OP_DIVIDE,       // /
    OP_MODULO,       // %

    // Assignment operators
    OP_ASSIGN,       // =
    OP_ADD_ASSIGN,   // +=
    OP_SUB_ASSIGN,   // -=
    OP_MUL_ASSIGN,   // *=
    OP_DIV_ASSIGN,   // /=
    OP_MOD_ASSIGN,   // %=
    OP_AND_ASSIGN,   // &=
    OP_OR_ASSIGN,    // |=
    OP_XOR_ASSIGN,   // ^=
    OP_SHL_ASSIGN,   // <<=
    OP_SHR_ASSIGN,   // >>=

    // Bitwise operators
    OP_BITWISE_AND,  // &
    OP_BITWISE_OR,   // |
    OP_BITWISE_XOR,  // ^
    OP_BITWISE_NOT,  // ~
    OP_SHIFT_LEFT,   // <<
    OP_SHIFT_RIGHT,  // >>

    // Logical operators
    OP_LOGICAL_AND,  // &&
    OP_LOGICAL_OR,   // ||
    OP_LOGICAL_NOT,  // !

    // Comparison operators
    OP_EQUAL,        // ==
    OP_NOT_EQUAL,    // !=
    OP_LESS,         // <
    OP_GREATER,      // >
    OP_LESS_EQUAL,   // <=
    OP_GREATER_EQUAL,// >=

    // Increment/Decrement
    OP_INCREMENT,    // ++
    OP_DECREMENT,    // --

    // Member access
    OP_MEMBER_DOT,   // .
    OP_MEMBER_ARROW, // ->

    // Other operators
    OP_ADDRESS_OF,   // &
    OP_DEREFERENCE,  // *
    OP_SIZEOF,       // sizeof
    OP_COMMA,        // ,
    OP_CONDITIONAL   // ? :
} OperatorType;

// Keyword types
typedef enum {
    // Control flow
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_FOR,
    KW_DO,
    KW_SWITCH,
    KW_CASE,
    KW_DEFAULT,
    KW_BREAK,
    KW_CONTINUE,
    KW_RETURN,
    KW_GOTO,

    // Type keywords
    KW_VOID,
    KW_CHAR,
    KW_SHORT,
    KW_INT,
    KW_LONG,
    KW_FLOAT,
    KW_DOUBLE,
    KW_SIGNED,
    KW_UNSIGNED,

    // Type qualifiers
    KW_CONST,
    KW_VOLATILE,
    KW_RESTRICT,

    // Storage classes
    KW_AUTO,
    KW_REGISTER,
    KW_STATIC,
    KW_EXTERN,
    KW_TYPEDEF,

    // Structure/Union
    KW_STRUCT,
    KW_UNION,
    KW_ENUM,

    // Other
    KW_SIZEOF,
    KW_ALIGNOF,
    KW_INLINE,
    KW_STATIC_ASSERT
} KeywordType;

// Literal value types that can be stored
typedef enum {
    VAL_INTEGER,
    VAL_FLOAT,
    VAL_CHAR,
    VAL_STRING,
    VAL_BOOL,
    VAL_NULL,
    VAL_COMPOUND  // For array/struct initializers
} ValueType;

// Union to store different types of values
typedef union {
    int64_t int_val;
    double float_val;
    char char_val;
    char* string_val;
    bool bool_val;
    void* compound_val;  // For complex types
} ValueData;

// Structure to represent a literal value
typedef struct {
    ValueType type;
    ValueData data;
    bool is_unsigned;    // For integer types
    int bit_width;       // For integer types (8, 16, 32, 64)
} LiteralValue;

// Structure for symbol table entries
typedef struct SymbolTableEntry {
    char* name;
    TokenType token_type;
    TokenAttributes attributes;
    LiteralValue value;
    struct SymbolTableEntry* next;
} SymbolTableEntry;

// Structure for tracking scope
typedef struct ScopeLevel {
    SymbolTableEntry* symbols;
    struct ScopeLevel* parent;
    int level;
} ScopeLevel;

// Structure for tracking array dimensions
typedef struct ArrayDimension {
    int size;
    struct ArrayDimension* next;
} ArrayDimension;

// Structure for function parameters
typedef struct FunctionParameter {
    char* name;
    TokenType param_type;
    TokenAttributes attributes;
    struct FunctionParameter* next;
} FunctionParameter;

// Structure for function signatures
typedef struct {
    char* name;
    TokenType return_type;
    TokenAttributes return_attributes;
    FunctionParameter* parameters;
    bool is_variadic;
    ScopeLevel* scope;
} FunctionSignature;

// Structure for struct/union members
typedef struct StructMember {
    char* name;
    TokenType member_type;
    TokenAttributes attributes;
    int offset;
    struct StructMember* next;
} StructMember;

// Structure for struct/union definitions
typedef struct {
    char* name;
    bool is_union;
    StructMember* members;
    int total_size;
    int alignment;
} StructDefinition;

// Structure for enum values
typedef struct EnumValue {
    char* name;
    int value;
    struct EnumValue* next;
} EnumValue;

// Structure for enum definitions
typedef struct {
    char* name;
    EnumValue* values;
    int last_value;
} EnumDefinition;

// Function declarations for value handling
LiteralValue* CreateLiteralValue(ValueType type);
void DestroyLiteralValue(LiteralValue* value);
const char* GetOperatorString(OperatorType op);
const char* GetKeywordString(KeywordType kw);
bool IsValidValue(const LiteralValue* value);
char* ValueToString(const LiteralValue* value);
bool ConvertValue(LiteralValue* value, ValueType target_type);

// Symbol table functions
SymbolTableEntry* CreateSymbol(const char* name, TokenType type);
void DestroySymbol(SymbolTableEntry* symbol);
ScopeLevel* CreateScope(ScopeLevel* parent);
void DestroyScope(ScopeLevel* scope);
bool AddSymbol(ScopeLevel* scope, SymbolTableEntry* symbol);
SymbolTableEntry* FindSymbol(ScopeLevel* scope, const char* name);

// Type-specific functions
FunctionSignature* CreateFunction(const char* name, TokenType return_type);
void DestroyFunction(FunctionSignature* func);
StructDefinition* CreateStruct(const char* name, bool is_union);
void DestroyStruct(StructDefinition* struct_def);
EnumDefinition* CreateEnum(const char* name);
void DestroyEnum(EnumDefinition* enum_def);

#endif // SYM_VALUE_H
