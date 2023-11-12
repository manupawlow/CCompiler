#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
	TOKEN_END,
	TOKEN_PLUS,
	TOKEN_MINUS, 
	TOKEN_STAR, 
	TOKEN_SLASH, 
	TOKEN_INTLIT,
} TokenType;

typedef struct {
	TokenType tokenType;
	int value;
	//FILE* source;
} Token;

typedef struct {
	FILE* source;
	size_t line;
	int curr_char;
} Lexer;

Lexer lexer_new(FILE* source);
Token lexer_next_token(Lexer* lexer);

#endif // LEXER_H_