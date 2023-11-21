#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

#include <string.h>
#include "definitions.h"
#include "lexer.h"
#include "parser.h"
#include "symbols.h"

static FILE* OutFile;
void setOutputFile(FILE* output);

void freeall_registers();
void assembly_preamble();
void assembly_postamble();
int assembly_ast_node(struct ASTNode* node, int reg, OperationType parent_type);
int assembly_load_int(int value);
int assembly_load_global(char* identifier);
int assembly_add(int r1, int r2);
int assembly_sub(int r1, int r2);
int assembly_mul(int r1, int r2);
int assembly_div(int r1, int r2);
void assembly_printint(int r);
int assembly_store_global(int r, int id);
void assembly_generate_global_symbol(int id);
int assembly_if(struct ASTNode* n);

#endif // CODE_GENERATOR_H_
