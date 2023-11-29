#include "lexer.h"

void lexer_error(char c, int line) {
	fprintf(stderr, "[LEXER] Unrecognised character %c on line %d\n", c, line);
	exit(1);
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

PrimitiveType parse_type(Lexer* lexer) {
	PrimitiveType type;
	switch (lexer->curr_token.tokenType) {
	case TOKEN_VOID: type = PRIM_VOID; break;
	case TOKEN_CHAR: type = PRIM_CHAR; break;
	case TOKEN_INT: type = PRIM_INT; break;
	case TOKEN_LONG: type = PRIM_LONG; break;
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
	if (strcmp(s, "printint") == 0) return TOKEN_PRINT;
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

	char c = next_non_space_char(lexer);

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
	case ',': t.tokenType = TOKEN_COMMA; break;
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