#pragma warning(disable : 4996)
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "code_generator.h"
#include "statements.h"

int interpretAST(struct ASTNode* n) {
	int leftval = 0, rightval = 0;

	// Get the left and right sub-tree values
	if (n->left)
		leftval = interpretAST(n->left);
	if (n->right)
		rightval = interpretAST(n->right);

	switch (n->operation) {
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
		fprintf(stderr, "Unknown AST operator %d\n", n->operation);
		exit(1);
	}
}

void read_output() {
	FILE* archivo;
	char caracter;

	// Abrir el archivo en modo lectura ("r")
	archivo = fopen("out.asm", "r");

	// Verificar si el archivo se abrió correctamente
	if (archivo == NULL) {
		printf("No se pudo abrir el archivo.\n");
		exit(1);
	}
	printf("\n");

	// Leer y mostrar el contenido del archivo
	while ((caracter = fgetc(archivo)) != EOF) {
		printf("%c", caracter);
	}

	// Cerrar el archivo
	fclose(archivo);
}

int main()
{
    FILE* infile;
    if ((infile = fopen("input.txt", "r")) == NULL) {
        fprintf(stderr, "Unable to open file: %s", strerror(errno));
        exit(1);
    }

	FILE* outfile;
	if ((outfile = fopen("out.asm", "w")) == NULL) {
		fprintf(stderr, "Unable to create file: %s", strerror(errno));
		exit(1);
	}

	setOutputFile(outfile);

	//Start compilation
	O_dumpAST = 1;
	struct ASTNode* tree;
	Lexer lexer = lexer_new(infile);
	lexer_next_token(&lexer);
	assembly_preamble();
	global_declarations(&lexer);
	assembly_postamble();
	//End compilation
	
	fclose(outfile);
	
	if (1) system("cls");
	read_output();

	return 0;
}