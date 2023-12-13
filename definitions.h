#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXT_LEN 512 
#define MAX_TOKEN_LEN 512 
#define NSYMBOLS 1024 

int Functionid;

char Text[MAX_TOKEN_LEN + 1];

static int O_dumpAST = 0;

typedef enum {
	GLOBAL = 1,
	LOCAL,
} STORAGECLASS;

typedef enum {
	TOKEN_EOF,

	//operators
	TOKEN_ASSING,
	TOKEN_LOGAND, TOKEN_LOGOR, 
	TOKEN_OR, TOKEN_XOR, TOKEN_AMPERSAND,
	TOKEN_EQUALS, TOKEN_NEGATE,
	TOKEN_LESS, TOKEN_GREATER, TOKEN_LESSOREQUALS, TOKEN_GREATEROREQUALS,
	TOKEN_LSHIFT, TOKEN_RSHIFT,
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,
	TOKEN_INCREMENT, TOKEN_DECREMENT, TOKEN_INVERT, TOKEN_LOGNOT,

	TOKEN_INTLIT, TOKEN_STRINGLIT,
	TOKEN_SEMICOLON, TOKEN_IDENTIFIER,
	TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_LBRACE, TOKEN_RBRACE,

	//keywords
	TOKEN_PRINT, TOKEN_PRINT2,
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
	NODE_LOGOR, NODE_LOGAND, NODE_OR, NODE_XOR, NODE_AND,
	NODE_EQUALS, NODE_NOTEQUALS, NODE_LESS, NODE_GREATER, NODE_LESSOREQUALS, NODE_GREATEROREQUALS,
	NODE_LSHIFT, NODE_RSHIFT,
	NODE_ADD, NODE_SUBTRACT,
	NODE_MULTIPLY, NODE_DIVIDE,

	NODE_PREINC, NODE_PREDEC, NODE_POSTINC, NODE_POSTDEC,
	NODE_NEGATE, NODE_INVERT, NODE_LOGNOT,

	NODE_INTLIT,
	NODE_STRINGLIT,
	NODE_IDENTIFIER,
	NODE_LVALUEIDENT,
	NODE_IF,
	NODE_WHILE,
	NODE_GLUE,
	NODE_PRINT, NODE_PRINT2,
	NODE_FUNCTION,
	NODE_WIDEN,
	NODE_SCALE,
	NODE_FUNCCALL,
	NODE_RETURN,
	NODE_ADDRESS,
	NODE_DEREFERENCE,
	NODE_TOBOOL,
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
	STORAGECLASS class;
	int posn; //local offset from stack
} Symbol;

Symbol SymbolTable[NSYMBOLS];
int Globs;
int Locls;

void match(TokenType type, Lexer* lexer);
int label_id();
char* token_to_string(TokenType t);
void dumpAST(struct ASTNode* n, int label, int level);

#endif // DEFINITIONS_H_
