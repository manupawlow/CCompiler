#include <stdio.h>
#include "lexer.h"

print_token(Token t) {
	switch (t.tokenType)
	{
	case TOKEN_END:
		printf("END\n");
		break;
	case TOKEN_PLUS:
		printf("+\n");
		break;
	case TOKEN_MINUS:
		printf("-\n");
		break;
	case TOKEN_STAR:
		printf("*\n");
		break;
	case TOKEN_SLASH:
		printf("/\n");
		break;
	case TOKEN_INTLIT:
		printf("%d\n", t.value);
		break;
	default:
		printf("??\n");
		break;
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
    Token token = lexer_next_token(&lexer);
	
	while (token.tokenType != TOKEN_END) {
		print_token(token);
		token = lexer_next_token(&lexer);
	}
}