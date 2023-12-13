#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include <string.h>
#include "definitions.h"
#include "code_generator.h"

int findSymbol(char* s);
int newGlobal();
int addGlobal(char* name, PrimitiveType type, StructuralType stype, int endlabel, int size);
int addLocal(char* name, PrimitiveType type, StructuralType stype, int endlabel, int size);

#endif // SYMBOLS_H_
