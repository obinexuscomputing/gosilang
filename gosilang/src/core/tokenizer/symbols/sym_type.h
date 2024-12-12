#ifndef SYM_TYPE_H
#define SYM_TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For Compile-Type safety,I have separated the
// the token type and token value in two modules
// sym_type and sym_value.

// To correctly identify token we use a bottom-up
// parser in both token and type definition.
enum TokenType {
    TOKEN_LITERAL_VALUE ,
    TOKEN_LITERAL_KEYWORD,
    TOKEN_LITERAL_DELIMETER,
    TOKEN_LITERAL_OPERATOR,
    TOKEN_LITERAL_IDENTIFER
};

#endif // SYM_TYPE_H
