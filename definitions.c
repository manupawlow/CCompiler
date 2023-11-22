#include "definitions.h"

void match(TokenType type, Lexer* lexer) {
	if (lexer->curr_token.tokenType != type) {
		printf("Unexpected token on line %d\n", lexer->curr_line);
		exit(1);
	}
	lexer_next_token(lexer);
}