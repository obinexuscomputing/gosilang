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
    TOKEN_VALUE,
    TOKEN_KEYWORD,
    TOKEN_DELIMETER,
    TOKEN_OPERATOR
};

#endif // SYM_TYPE_H
