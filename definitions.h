#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN_LEN 512 

int Functionid;

char Text[MAX_TOKEN_LEN + 1];

typedef enum {
	TOKEN_EOF,

	//operators
	TOKEN_PLUS, TOKEN_MINUS,
	TOKEN_STAR, TOKEN_SLASH,
	TOKEN_EQUALS, TOKEN_NOTEQUALS,
	TOKEN_LESS, TOKEN_GREATER, TOKEN_LESSOREQUALS, TOKEN_GREATEROREQUALS,
	TOKEN_ASSING, TOKEN_SEMICOLON, TOKEN_INTLIT, TOKEN_IDENTIFIER,
	TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE,

	TOKEN_AMPERSAND,
	TOKEN_AND,

	//keywords
	TOKEN_PRINT,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_WHILE,
	TOKEN_FOR,
	TOKEN_RETURN,
	TOKEN_VOID, TOKEN_CHAR, TOKEN_INT, TOKEN_LONG,
	TOKEN_COMMA,
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
	Token* rejected_token;
} Lexer;

typedef enum {
	PRIM_NONE,
	PRIM_VOID, PRIM_VOIDPTR,
	PRIM_CHAR, PRIM_CHARPTR,
	PRIM_INT, PRIM_INTPTR,
	PRIM_LONG, PRIM_LONGPTR,
} PrimitiveType;

// AST: Abstract Syntaxt Tree
typedef enum {
	NODE_ADD = 1, NODE_SUBTRACT,
	NODE_MULTIPLY, NODE_DIVIDE,
	NODE_EQUALS, NODE_NOTEQUALS,
	NODE_LESS, NODE_GREATER, NODE_LESSOREQUALS, NODE_GREATEROREQUALS,

	NODE_INTLIT,
	NODE_IDENTIFIER,
	NODE_LVALUEIDENT,
	NODE_ASSIGN,
	NODE_IF,
	NODE_WHILE,
	NODE_GLUE,
	NODE_PRINT,
	NODE_FUNCTION,
	NODE_WIDEN,
	NODE_FUNCCALL,
	NODE_RETURN,
	NODE_ADDRESS,
	NODE_DEREFERENCE,
} OperationType;

struct ASTNode {
	OperationType operation;
	PrimitiveType type;
	int value;
	struct ASTNode* left;
	struct ASTNode* mid;
	struct ASTNode* right;
};

#define NSYMBOLS 1024 

typedef enum {
	STRU_VARIABLE,
	STRU_FUNCTION,
} StructuralType;

typedef struct {
	char* name;
	PrimitiveType type;
	StructuralType stype;
	int endlabel;
} Symbol;

Symbol GlobalSymbols[NSYMBOLS];

void match(TokenType type, Lexer* lexer);
int label_id();
char* token_to_string(TokenType t);

#endif // DEFINITIONS_H_
