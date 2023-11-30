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

int inttype(int type) {
	return type == PRIM_CHAR || type == PRIM_INT || type == PRIM_LONG;
}

int ptrtype(int type) {
	return type == PRIM_VOIDPTR || type == PRIM_CHARPTR || type == PRIM_INTPTR || type == PRIM_LONGPTR;
}

struct ASTnode* modify_type(struct ASTNode* tree, PrimitiveType rtype, OperationType op) {
	PrimitiveType ltype;
	int lsize, rsize;

	ltype = tree->type;

	if (inttype(ltype) && inttype(rtype)) {

		if (ltype == rtype) return tree;

		lsize = get_type_size(ltype);
		rsize = get_type_size(rtype);

		if (lsize > rsize) return NULL;

		if (rsize > lsize) return (ast_new_unary(NODE_WIDEN, rtype, tree, 0));
	}

	if (ptrtype(ltype)) {
		if (op == 0 && ltype == rtype) return (tree);
	}

	if (op != NODE_ADD && op != NODE_SUBTRACT) {
		fprintf(stderr, "Only add and subtract operation on pointer");
		exit(1);
	}

	if (op == NODE_ADD || op == NODE_SUBTRACT) {

		if (inttype(ltype) && ptrtype(rtype)) {
			rsize = get_type_size(value_at(rtype));
			if (rsize > 1)
				return (ast_new_unary(NODE_SCALE, rtype, tree, rsize));
		}
	}

	return (NULL);
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