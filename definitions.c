#include "definitions.h"

void match(TokenType type, Lexer* lexer) {
	if (lexer->curr_token.tokenType != type) {
		printf("Unexpected token on line %d, expected a %s but found %s\n", lexer->curr_line, token_to_string(type), token_to_string(lexer->curr_token.tokenType));
		exit(1);
	}
	lexer_next_token(lexer);
}

int label_id() {
	static int id = 1;
	return (id++);
}

char* token_to_string(TokenType t) {
	if (t == TOKEN_EOF)              return "TOKEN_EOF            ";

	if (t == TOKEN_PLUS)             return "TOKEN_PLUS           ";
	if (t == TOKEN_MINUS)            return "TOKEN_MINUS          ";
	if (t == TOKEN_STAR)             return "TOKEN_STAR           ";
	if (t == TOKEN_SLASH)            return "TOKEN_SLASH          ";
	if (t == TOKEN_EQUALS)           return "TOKEN_EQUALS         ";
	if (t == TOKEN_NOTEQUALS)        return "TOKEN_NOTEQUALS      ";
	if (t == TOKEN_LESS)             return "TOKEN_LESS           ";
	if (t == TOKEN_GREATER)          return "TOKEN_GREATER        ";
	if (t == TOKEN_LESSOREQUALS)     return "TOKEN_LESSOREQUALS   ";
	if (t == TOKEN_GREATEROREQUALS)  return "TOKEN_GREATEROREQUALS";

	if (t == TOKEN_AMPERSAND)		 return "TOKEN_AMPERSAND      ";
	if (t == TOKEN_AND)				 return "TOKEN_AND            ";

	if (t == TOKEN_ASSING)           return "TOKEN_ASSING         ";
	if (t == TOKEN_SEMICOLON)        return "TOKEN_SEMICOLON      ";
	if (t == TOKEN_INTLIT)           return "TOKEN_INTLIT         ";
	if (t == TOKEN_IDENTIFIER)       return "TOKEN_IDENTIFIER     ";
	if (t == TOKEN_LPAREN)           return "TOKEN_LPAREN         ";
	if (t == TOKEN_RPAREN)           return "TOKEN_RPAREN         ";
	if (t == TOKEN_LBRACE)           return "TOKEN_LBRACE         ";
	if (t == TOKEN_RBRACE)           return "TOKEN_RBRACE         ";
	if (t == TOKEN_PRINT)            return "TOKEN_PRINT          ";
	if (t == TOKEN_IF)               return "TOKEN_IF             ";
	if (t == TOKEN_ELSE)             return "TOKEN_ELSE           ";
	if (t == TOKEN_WHILE)            return "TOKEN_WHILE          ";
	if (t == TOKEN_FOR)              return "TOKEN_FOR            ";
	if (t == TOKEN_INT)              return "TOKEN_INT            ";
	if (t == TOKEN_VOID)             return "TOKEN_VOID           ";
	if (t == TOKEN_CHAR)             return "TOKEN_CHAR           ";
	if (t == TOKEN_LONG)             return "TOKEN_LONG           ";
	if (t == TOKEN_RETURN)			 return "TOKEN_RETURN         ";
	return "????????             ";
}