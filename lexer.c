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

char* scan_token(Lexer* lexer, char* buffer) {
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

int keyword_token(char* s) {
	if (strcmp(s, "print") == 0) 
		return TOKEN_PRINT;
	if (strcmp(s, "int") == 0) return TOKEN_INT;
	if (strcmp(s, "if") == 0) return TOKEN_IF;
	if (strcmp(s, "else") == 0) return TOKEN_ELSE;
	return 0;
}

Token lexer_next_token(Lexer* lexer) {
	int c = next_non_space_char(lexer);
	printf("%c:", c);

	Token t = {0};

	switch (c)
	{
	case EOF:
		t.tokenType = TOKEN_EOF;
		break;
	case '+':
		t.tokenType = TOKEN_PLUS;
		break;
	case '-':
		t.tokenType = TOKEN_MINUS;
		break;
	case '*':
		t.tokenType = TOKEN_STAR;
		break;
	case '/':
		t.tokenType = TOKEN_SLASH;
		break;
	case ';':
		t.tokenType = TOKEN_SEMICOLON;
		break;
	case '(':
		t.tokenType = TOKEN_LPAREN;
		break;
	case ')':
		t.tokenType = TOKEN_RPAREN;
		break;
	case '{':
		t.tokenType = TOKEN_LBRACE;
		break;
	case '}':
		t.tokenType = TOKEN_RBRACE;
		break;
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
	default:
		if (isdigit(c)) {
			t.tokenType = TOKEN_INTLIT;
			t.value = scan_int(lexer);
			break;
		}
		else if (isalpha(c) || '_' == c) {
			scan_token(lexer, &Text);
			t.tokenType = keyword_token(Text);

			if (!t.tokenType) {
				t.tokenType = TOKEN_IDENTIFIER;
			}
			break;
		}
		lexer_error(c, lexer->curr_line);
	}
	t.line = lexer->curr_line;
	lexer->curr_token = t;

	printf(" %d %d\n", t.tokenType, t.value);

	return t;
}