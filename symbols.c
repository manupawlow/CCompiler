#include "symbols.h"

void updatesym(int slot, char* name, int type, int stype, int class, int endlabel, int size, int posn) {
    if (slot < 0 || slot >= NSYMBOLS) {
        fprintf(stderr, "Invalid symbol slot number in updatesym()");
        exit(1);
    }
    SymbolTable[slot].name = _strdup(name);
    SymbolTable[slot].type = type;
    SymbolTable[slot].stype = stype;
    SymbolTable[slot].class = class;
    SymbolTable[slot].endlabel = endlabel;
    SymbolTable[slot].size = size;
    SymbolTable[slot].posn = posn;
}

void freeloclsyms(void) {
    Locls = NSYMBOLS - 1;
}

int findGlobal(char* s) {
    for (int i = 0; i < Globs; i++) {
        if (SymbolTable[i].class != PARAM && *s == *SymbolTable[i].name && !strcmp(s, SymbolTable[i].name))
            return i;
    }
    return -1;
}

int findLocal(char* s) {
    int i;
    for (i = Locls + 1; i < NSYMBOLS; i++) {
        if (*s == *SymbolTable[i].name && !strcmp(s, SymbolTable[i].name))
            return (i);
    }
    return (-1);
}

int findSymbol(char* s) {
    int slot;
    slot = findLocal(s);
    if (slot == -1)
        slot = findGlobal(s);
    return slot;
}

int newGlobal() {
    int p;
    if ((p = Globs++) >= Locls)
        fprintf(stderr, "Too many global symbols");
    return p;
}

int newLocal() {
    int p;
    if ((p = Locls--) <= Globs)
        fprintf(stderr, "Too many local symbols");
    return p;
}

int addGlobal(char* name, PrimitiveType type, StructuralType stype, int endlabel, int size) {
    int slot;

    if ((slot = findGlobal(name)) != -1)
        return slot;

    slot = newGlobal();
    updatesym(slot, name, type, stype, GLOBAL, endlabel, size, 0);
    assembly_generate_global_symbol(slot);
    return slot;
}

int addLocal(char* name, PrimitiveType type, StructuralType stype, int isparam, int size) {
    int localslot, posn;

    if ((localslot = findLocal(name)) != -1)
        return localslot;

    localslot = newLocal();
    if (isparam) {
        updatesym(localslot, name, type, stype, PARAM, 0, size, 0);
        updatesym(newGlobal(), name, type, stype, PARAM, 0, size, 0);
    }
    else {
        updatesym(localslot, name, type, stype, LOCAL, 0, size, 0);
    }

    //posn = newlocaloffset(type, 0);
    return localslot;
}