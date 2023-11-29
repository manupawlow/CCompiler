#ifndef LEXER_H_
#define LEXER_H_

#include <ctype.h>
#include <string.h>
#include "definitions.h"
#include "types.h"

Lexer lexer_new(FILE* source);
int lexer_next_token(Lexer* lexer);
void lexer_reject_token(Token* t, Lexer* lexer);
int parse_type(Lexer* lexer);

#endif // LEXER_H_