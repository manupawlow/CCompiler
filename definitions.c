#include "definitions.h"

void match(TokenType type, Lexer* lexer) {
	if (lexer->curr_token.tokenType != type) {
		printf("Unexpected token on line %d\n", lexer->curr_line);
		exit(1);
	}
	lexer_next_token(lexer);
}

int label_id() {
	static int id = 1;
	return (id++);
}
