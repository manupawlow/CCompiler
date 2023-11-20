#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 512 

char Text[MAX_TOKEN_LEN + 1];

typedef enum {
	TOKEN_EOF,
	TOKEN_PLUS, TOKEN_MINUS, 
	TOKEN_STAR, TOKEN_SLASH, 
	TOKEN_EQUALS, TOKEN_NOTEQUALS,
	TOKEN_LESS, TOKEN_GREATER, TOKEN_LESSOREQUALS, TOKEN_GREATEROREQUALS,
	TOKEN_ASSING, TOKEN_SEMICOLON, TOKEN_INTLIT, TOKEN_IDENTIFIER,
	TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE,

	//keywords
	TOKEN_PRINT,
	TOKEN_INT,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_WHILE,
	TOKEN_FOR,
	TOKEN_VOID,
} TokenType;

typedef struct {
	TokenType tokenType;
	int value;
	int line;
} Token;

typedef struct {
	FILE* source;
	int curr_line;
	int curr_char;
	Token curr_token;
} Lexer;

Lexer lexer_new(FILE* source);
Token lexer_next_token(Lexer* lexer);

#endif // LEXER_H_