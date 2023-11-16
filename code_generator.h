#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include <string.h>
#include "lexer.h"
#include "parser.h"

#define NSYMBOLS 1024 

struct Symbol {
	char* name;                   // Name of a symbol
};

struct Symbol SymTable[NSYMBOLS];
static int SymTablePosition = 0;

void generate_code(FILE* outfile, Lexer* lexer);
int findSymbol(char* symbol);

#endif // CODE_GENERATOR_H_
