#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

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

int main()
{
    FILE* infile;
    errno_t err = fopen_s(&infile, "input.txt", "r");
    if (err != 0) {
        fprintf(stderr, "Unable to open file\n");
        exit(1);
    }

    Lexer lexer = lexer_new(infile);
	
	Parser parser = parser_new(&lexer);

	ASTNode* root = parser_parse(&parser);
	
	printTree(root, "", 0);

	printf("\n\nResult: %d", interpretAST(root));

	//Token token = lexer_next_token(&lexer);
	//while (token.tokenType != TOKEN_END) {
	//	print_token(token);
	//	token = lexer_next_token(&lexer);
	//}
}