#ifndef SYMBOLS_H_
#define SYMBOLS_H_

#include <stdio.h>
#include <string.h>

#define NSYMBOLS 1024 

struct Symbol {
	char* name;
};

struct Symbol GlobalSymbols[NSYMBOLS];

int findGlobal(char* s);
int nextGlobalSlot();
int addGlobal(char* name);

#endif // SYMBOLS_H_
