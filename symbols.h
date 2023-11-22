#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include <string.h>
#include "definitions.h"

int findGlobal(char* s);
int nextGlobalSlot();
int addGlobal(char* name, PrimitiveType type, StructuralType stype);

#endif // SYMBOLS_H_
