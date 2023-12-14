#pragma warning(disable : 4996)
#include <string.h>
#include "symbols.h"
#include "parser.h"
#include "parser.h"
#include "code_generator.h"
#include "statements.h"
#include <Windows.h>
#include <stdio.h>

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
	printf("\n\n\n");

	// Leer y mostrar el contenido del archivo
	while ((caracter = fgetc(archivo)) != EOF) {
		printf("%c", caracter);
	}

	// Cerrar el archivo
	fclose(archivo);
}

void copy_output() {
	FILE* archivo = fopen("out.asm", "r");

	if (archivo == NULL) {
		perror("Error al abrir el archivo");
		return 1;
	}

	// Obtener el tamaño del archivo
	fseek(archivo, 0, SEEK_END);
	long tamaño = ftell(archivo);
	rewind(archivo);

	// Leer el contenido del archivo
	char* contenido = (char*)malloc(tamaño + 1);
	if (contenido == NULL) {
		perror("Error al asignar memoria para el contenido");
		fclose(archivo);
		return 1;
	}

	fread(contenido, 1, tamaño, archivo);
	contenido[tamaño] = '\0';

	// Cerrar el archivo
	fclose(archivo);

	// Abrir un proceso para ejecutar el comando xclip y pasarle el contenido del archivo
	FILE* pipe = _popen("clip", "w");
	if (pipe == NULL) {
		perror("Error al abrir el proceso xclip");
		free(contenido);
		return 1;
	}

	// Escribir el contenido en el proceso xclip
	fprintf(pipe, "%s", contenido);

	// Cerrar el proceso xclip
	_pclose(pipe);

	// Liberar la memoria asignada
	free(contenido);
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
	addGlobal("printint", PRIM_INT, STRU_FUNCTION, 0, 0);
	addGlobal("printchar", PRIM_VOID, STRU_FUNCTION, 0, 0);
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
	copy_output();

	return 0;
}