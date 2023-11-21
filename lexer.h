#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "definitions.h"

Lexer lexer_new(FILE* source);
Token lexer_next_token(Lexer* lexer);
int parse_type(TokenType t);

#endif // LEXER_H_