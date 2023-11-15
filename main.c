#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "code_generator.h"

void print_ast_node(ASTNode* node) {
	char myString[20];
	switch (node->type)
	{
	case NODE_ADD:
		printf("+");
		break;
	case NODE_SUBTRACT:
		printf("-");
		break;
	case NODE_MULTIPLY :
		printf("*");
		break;
	case NODE_DIVIDE:
		printf("/");
		break;
	case NODE_INTLIT:
		printf("%d", node->value);
		break;
	default:
		printf("?");
		break;
	}
}

void printTree(const ASTNode* node, const char* prefix, int isRight)
{
	if (node != NULL)
	{
		char newPrefix[100];
		char* val;

		if (isRight) {
			printf("%s|--", prefix);
			print_ast_node(node);
			printf("\n");
			strcpy(newPrefix, prefix);
			strcat(newPrefix, "|   ");
		}
		else {
			printf("%s --", prefix);
			print_ast_node(node);
			printf("\n");
			strcpy(newPrefix, prefix);
			strcat(newPrefix, "    ");
		}

		printTree(node->right, newPrefix, 1);
		printTree(node->left, newPrefix, 0);
	}
}

int interpretAST(ASTNode* n) {
	int leftval = 0, rightval = 0;

	// Get the left and right sub-tree values
	if (n->left)
		leftval = interpretAST(n->left);
	if (n->right)
		rightval = interpretAST(n->right);

	switch (n->type) {
	case NODE_ADD:
		return (leftval + rightval);
	case NODE_SUBTRACT:
		return (leftval - rightval);
	case NODE_MULTIPLY:
		return (leftval * rightval);
	case NODE_DIVIDE:
		return (leftval / rightval);
	case NODE_INTLIT:
		return (n->value);
	default:
		fprintf(stderr, "Unknown AST operator %d\n", n->type);
		exit(1);
	}
}

int* line;

int main()
{
    FILE* infile;
    if ((infile = fopen("input.txt", "r")) == NULL) {
        fprintf(stderr, "Unable to open file: %s", strerror(errno));
        exit(1);
    }

    Lexer lexer = lexer_new(infile);
	
	ASTNode* root = parse(&lexer);

	FILE* outfile;
	if ((outfile = fopen("out.asm", "w")) == NULL) {
		fprintf(stderr, "Unable to create file: %s", strerror(errno));
		exit(1);
	}

	generate_code(outfile, root);

	printTree(root, "", 0);
	printf("\n\nResult: %d", interpretAST(root));

	fclose(outfile);
	return 0;
}