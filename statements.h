#ifndef STATEMENTS_H_
#define STATEMENTS_H_

#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "code_generator.h"

ASTNode* compound_statement(Lexer* lexer);
ASTNode* if_statement(Lexer* lexer);
ASTNode* single_statement(Lexer* lexer);
ASTNode* function_declaration(Lexer* lexer);

#endif // STATEMENTS_H_