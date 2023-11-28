#include "types.h"

PrimitiveType pointer_to(PrimitiveType type) {
	PrimitiveType newType;
	switch (type)
	{
	case PRIM_VOID: newType = PRIM_VOIDPTR; break;
	case PRIM_CHAR: newType = PRIM_CHARPTR; break;
	case PRIM_INT: newType = PRIM_INTPTR; break;
	case PRIM_LONG: newType = PRIM_LONGPTR; break;
	default:
		fprintf(stderr, "pointer_to");
		exit(1);
	}
	return newType;
}

PrimitiveType value_at(PrimitiveType type) {
	PrimitiveType newType;
	switch (type)
	{
	case PRIM_VOIDPTR: newType = PRIM_VOID; break;
	case PRIM_CHARPTR: newType = PRIM_CHAR; break;
	case PRIM_INTPTR: newType = PRIM_INT; break;
	case PRIM_LONGPTR: newType = PRIM_LONG; break;
	default:
		fprintf(stderr, "value_at");
		exit(1);
	}
	return newType;
}

int type_compatible(PrimitiveType* left, PrimitiveType* right, int onlyright) {
	int leftsize, rightsize;

	if (*left == *right) {
		*left = *right = 0; 
		return 1;
	}

	leftsize = get_type_size(*left);
	rightsize = get_type_size(*right);

	if (leftsize == 0 || rightsize == 0) return 0;

	if (leftsize < rightsize) {
		*left = NODE_WIDEN;
		*right = 0;
		return 1;
	}

	if (rightsize < leftsize) {
		if (onlyright) return 0;
		*left = 0;
		*right = NODE_WIDEN;
		return 1;
	}

	*left = *right = 0;
	return 1;
}