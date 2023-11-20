#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

// AST: Abstract Syntaxt Tree
typedef enum {
	NODE_ADD = 1, NODE_SUBTRACT,
	NODE_MULTIPLY, NODE_DIVIDE,
	NODE_EQUALS, NODE_NOTEQUALS,
	NODE_LESS, NODE_GREATER, NODE_LESSOREQUALS, NODE_GREATEROREQUALS,
	
	NODE_INTLIT,
	NODE_IDENTIFIER,
	NODE_LVALUE_IDENTIFIER,
	NODE_ASSIGN,
	NODE_IF,
	NODE_WHILE,
	NODE_GLUE,
	NODE_PRINT,
	NODE_FUNCTION,
} NodeType;

typedef struct {
	NodeType type;
	int value;
	struct ASTNode* left;
	struct ASTNode* mid;
	struct ASTNode* right;
} ASTNode;

struct ASTNode* parse(Lexer* lexer);
struct ASTNode* binexpr(Lexer* lexer, int prev_precedence);
struct ASTNode* ast_new_node(NodeType type, ASTNode* left, ASTNode* mid, ASTNode* right, int value);
struct ASTNode* ast_new_leaf(NodeType type, int value);
struct ASTNode* ast_new_unary(NodeType type, ASTNode* left, int value);

#endif // PARSER_H_