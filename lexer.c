#include "lexer.h"

void lexer_error(char c, int line) {
	fprintf(stderr, "[LEXER] Unrecognised character %c on line %d\n", c, line);
	exit(1);
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

Lexer lexer_new(FILE* source) {
	Lexer l = { source, 1 };
	return l;
}

int is_white_space(char c) {
	return ' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c;
}

int next_char(Lexer* lexer) {
	int c = fgetc(lexer->source);
	if ('\n' == c) {
		lexer->curr_line++;
	}
	lexer->curr_char = c;
	return c;
}

int next_non_space_char(Lexer* lexer) {
	int c;
	do {
		c = next_char(lexer);
	} while (is_white_space(c));
	return c;
}

int char_pos(char* s, int c) {
	char* p;

	p = strchr(s, c);
	return (p ? p - s : -1);
}

int scan_int(Lexer* lexer) {
	int k, int_value = 0, c = lexer->curr_char;

	while ((k = char_pos("0123456789", c)) >= 0) {
		int_value = int_value * 10 + k;
		c = next_char(lexer);
	}
	ungetc(c, lexer->source);
	return int_value;
}

char* scan_identifier(Lexer* lexer, char* buffer) {
	int k, int_value = 0, c = lexer->curr_char;

	int i = 0;
	while (isalpha(c) || isdigit(c) || '_' == c) {

		if (i >= MAX_TOKEN_LEN - 1) {
			fprintf(stderr, "[LEXER] Unrecognised character on line %d\n", lexer->curr_line);
			exit(1);
		}

		buffer[i++] = c;
		c = next_char(lexer);
	}

	ungetc(c, lexer->source);
	buffer[i] = '\0';
}

PrimitiveType parse_type(TokenType t, Lexer* lexer) {
	PrimitiveType type;
	switch (t) {
	case TOKEN_VOID: return PRIM_VOID;
	case TOKEN_CHAR: return PRIM_CHAR;
	case TOKEN_INT: return PRIM_INT;
	case TOKEN_LONG: return PRIM_LONG;
	default:
		fprintf(stderr, "Illegal type");
		exit(1);
	}

	while (1) {
		lexer_next_token(lexer);
		if (lexer->curr_token.tokenType != TOKEN_STAR) break;
		type = pointer_to(type);
	}

	return type;
}

int keyword_token(char* s) {
	if (strcmp(s, "print") == 0) return TOKEN_PRINT;
	if (strcmp(s, "if") == 0) return TOKEN_IF;
	if (strcmp(s, "else") == 0) return TOKEN_ELSE;
	if (strcmp(s, "while") == 0) return TOKEN_WHILE;
	if (strcmp(s, "for") == 0) return TOKEN_FOR;
	if (strcmp(s, "return") == 0) return TOKEN_RETURN;

	if (strcmp(s, "void") == 0) return TOKEN_VOID;
	if (strcmp(s, "char") == 0) return TOKEN_CHAR;
	if (strcmp(s, "int") == 0) return TOKEN_INT;
	if (strcmp(s, "long") == 0) return TOKEN_LONG;
	return 0;
}

void lexer_reject_token(Token* t, Lexer* lexer) {
	if (lexer->rejected_token != NULL) {
		fprintf(stderr, "Can't reject token twice");
		exit(1);
	}
	lexer->rejected_token = t;
}

int lexer_next_token(Lexer* lexer) {

	if (lexer->rejected_token != NULL) {
		lexer->curr_token = *lexer->rejected_token;
		lexer->rejected_token = NULL;
		return 1;
	}

	int c = next_non_space_char(lexer);

	Token t = {0};

	int c2 = c;
	int aa = 1;
	switch (c)
	{
	case EOF: t.tokenType = TOKEN_EOF; break;
	case '+': t.tokenType = TOKEN_PLUS; break;
	case '-': t.tokenType = TOKEN_MINUS; break;
	case '*': t.tokenType = TOKEN_STAR; break;
	case '/': t.tokenType = TOKEN_SLASH; break;
	case ';': t.tokenType = TOKEN_SEMICOLON; break;
	case '(': t.tokenType = TOKEN_LPAREN; break;
	case ')': t.tokenType = TOKEN_RPAREN; break;
	case '{': t.tokenType = TOKEN_LBRACE; break;
	case '}': t.tokenType = TOKEN_RBRACE; break;
	case '=':
		if ((c = next_char(lexer)) == '=') {
			t.tokenType = TOKEN_EQUALS;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_ASSING;
		}
		break;
	case '!':
		if ((c = next_char(lexer)) == '=') {
			t.tokenType = TOKEN_NOTEQUALS;
		}
		else {
			fprintf(stderr, "[LEXER] Unrecognised character on line %d\n", lexer->curr_line);
			exit(1);
		}
		break;
	case '<':
		if ((c = next_char(lexer)) == '=') {
			t.tokenType = TOKEN_LESSOREQUALS;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_LESS;
		}
		break;
	case '>':
		if ((c = next_char(lexer)) == '=') {
			t.tokenType = TOKEN_GREATEROREQUALS;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_GREATER;
		}
		break;
	case '&':
		if ((c = next_char(lexer)) == '&') {
			t.tokenType = TOKEN_AND;
		} else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_AMPERSAND;
		}
		break;
	default:
		if (isdigit(c)) {
			t.tokenType = TOKEN_INTLIT;
			t.value = scan_int(lexer);
			break;
		}
		else if (isalpha(c) || '_' == c) {
			scan_identifier(lexer, &Text);
			t.tokenType = keyword_token(Text);

			if (!t.tokenType) {
				t.tokenType = TOKEN_IDENTIFIER;
			}
			aa = 0;
			printf("%s\t%s\t%d\n", token_to_string(t.tokenType), Text, t.value);

			break;
		}
		lexer_error(c, lexer->curr_line);
	}
	t.line = lexer->curr_line;
	lexer->curr_token = t;

	if (aa)
		printf("%s\t%c\t%d\n", token_to_string(t.tokenType), c2, t.value);


	return 1;
}