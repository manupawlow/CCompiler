#ifndef TYPES_H_
#define TYPES_H_

#include "definitions.h"
#include "code_generator.h"

int type_compatible(PrimitiveType* left, PrimitiveType* right, int onlyright);
PrimitiveType pointer_to(PrimitiveType type);
PrimitiveType value_at(PrimitiveType type);
struct ASTnode* modify_type(struct ASTNode* tree, PrimitiveType rtype, OperationType op);
int inttype(int type);

#endif // TYPES_H_
