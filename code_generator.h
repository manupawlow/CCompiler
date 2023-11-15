#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include "lexer.h"
#include "parser.h"

void generate_code(FILE* outfile, Lexer* lexer);

#endif // CODE_GENERATOR_H_
