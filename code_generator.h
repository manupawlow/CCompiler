#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include "parser.h"

void generate_code(FILE* outfile, ASTNode* root);

#endif // CODE_GENERATOR_H_
