#ifndef PARSER_H_
#define PARSER_H_

#include "definitions.h"
#include "lexer.h"

struct ASTNode* parse(Lexer* lexer);
struct ASTNode* binexpr(Lexer* lexer, int prev_precedence);
struct ASTNode* ast_new_node(OperationType operation, PrimitiveType type, struct ASTNode* left, struct ASTNode* mid, struct ASTNode* right, int value);
struct ASTNode* ast_new_leaf(OperationType operation, PrimitiveType type, int value);
struct ASTNode* ast_new_unary(OperationType operation, PrimitiveType type, struct ASTNode* left, int value);
struct ASTNode* parse_funccall(Lexer* lexer);
struct ASTNode* parse_primary_factor(Lexer* lexer);

#endif // PARSER_H_