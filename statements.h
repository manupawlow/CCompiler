#ifndef STATEMENTS_H_
#define STATEMENTS_H_

#include "lexer.h"
#include "parser.h"
#include "code_generator.h"
#include "types.h"
#include "definitions.h"

struct ASTNode* compound_statement(Lexer* lexer);
struct ASTNode* if_statement(Lexer* lexer);
struct ASTNode* single_statement(Lexer* lexer);
struct ASTNode* function_declaration(Lexer* lexer);

#endif // STATEMENTS_H_