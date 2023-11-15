#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

// AST: Abstract Syntaxt Tree
typedef enum {
	NODE_ADD,
	NODE_SUBTRACT,
	NODE_MULTIPLY,
	NODE_DIVIDE,
	NODE_INTLIT,
} NodeType;

typedef struct {
	NodeType type;
	int value;
	struct ASTNode* left;
	struct ASTNode* right;
} ASTNode;

struct ASTNode* parse(Lexer* lexer);
struct ASTNode* parser_expresion(Lexer* lexer, int prev_precedence);

#endif // PARSER_H_