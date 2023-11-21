#include "types.h"

int type_compatible(PrimitiveType* left, PrimitiveType* right, int onlyright) {
	if (*left == PRIM_VOID || *right == PRIM_VOID) 
		return 0;
	
	if (*left == *right) {
		*left = *right = 0; 
		return 1;
	}

	if (*left == PRIM_CHAR && *right == PRIM_INT) {
		*left = NODE_WIDEN;
		*right = 0;
		return 1;
	}

	if (*left == PRIM_INT && *right == PRIM_CHAR) {
		if (onlyright) return 0;
		*left = 0;
		*right = NODE_WIDEN;
		return 1;
	}

	*left = *right = 0;
	return 1;
}