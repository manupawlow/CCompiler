#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN_LEN 512 

int Functionid;

char Text[MAX_TOKEN_LEN + 1];

static int O_dumpAST = 0;

typedef enum {
	TOKEN_EOF,

	//operators
	TOKEN_ASSING,
	TOKEN_PLUS, TOKEN_MINUS,
	TOKEN_STAR, TOKEN_SLASH,
	TOKEN_EQUALS, TOKEN_NOTEQUALS,
	TOKEN_LESS, TOKEN_GREATER, TOKEN_LESSOREQUALS, TOKEN_GREATEROREQUALS,
	
	TOKEN_SEMICOLON, TOKEN_INTLIT, TOKEN_IDENTIFIER,
	TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_LBRACE, TOKEN_RBRACE,
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
	NODE_ASSIGN = 1,
	NODE_ADD, NODE_SUBTRACT,
	NODE_MULTIPLY, NODE_DIVIDE,
	NODE_EQUALS, NODE_NOTEQUALS,
	NODE_LESS, NODE_GREATER, NODE_LESSOREQUALS, NODE_GREATEROREQUALS,

	NODE_INTLIT,
	NODE_IDENTIFIER,
	NODE_LVALUEIDENT,
	NODE_IF,
	NODE_WHILE,
	NODE_GLUE,
	NODE_PRINT,
	NODE_FUNCTION,
	NODE_WIDEN,
	NODE_SCALE,
	NODE_FUNCCALL,
	NODE_RETURN,
	NODE_ADDRESS,
	NODE_DEREFERENCE,
} OperationType;

struct ASTNode {
	OperationType operation;
	PrimitiveType type;
	int value;
	int isRvalue;
	struct ASTNode* left;
	struct ASTNode* mid;
	struct ASTNode* right;
};

#define NSYMBOLS 1024 

typedef enum {
	STRU_VARIABLE,
	STRU_FUNCTION,
	STRU_ARRAY,
} StructuralType;

typedef struct {
	char* name;
	PrimitiveType type;
	StructuralType stype;
	int endlabel;
	int size;
} Symbol;

Symbol GlobalSymbols[NSYMBOLS];

void match(TokenType type, Lexer* lexer);
int label_id();
char* token_to_string(TokenType t);
void dumpAST(struct ASTNode* n, int label, int level);

#endif // DEFINITIONS_H_
