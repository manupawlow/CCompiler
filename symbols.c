#include "symbols.h"

static int Globals_Len;

int findGlobal(char* s) {
    for (int i = 0; i < Globals_Len; i++) {
        if (*s == *GlobalSymbols[i].name && !strcmp(s, GlobalSymbols[i].name))
            return i;
    }
    return -1;
}

int nextGlobalSlot() {
    int p;
    if ((p = Globals_Len++) >= NSYMBOLS)
        fprintf(stderr, "Too many global symbols");
    return p;
}

int addGlobal(char* name, PrimitiveType type, StructuralType stype) {
    int y;

    if ((y = findGlobal(name)) != -1)
        return y;

    y = nextGlobalSlot();
    GlobalSymbols[y].name = _strdup(name);
    GlobalSymbols[y].type = type;
    GlobalSymbols[y].stype = stype;
    return y;
}