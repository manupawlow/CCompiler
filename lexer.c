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

int scanch(Lexer* lexer) {
	int c;
	c = next_char(lexer);
	if (c == '\\') {
		switch (c = next_char(lexer)) {
		case 'a':  return '\a';
		case 'b':  return '\b';
		case 'f':  return '\f';
		case 'n':  return '\n';
		case 'r':  return '\r';
		case 't':  return '\t';
		case 'v':  return '\v';
		case '\\': return '\\';
		case '"':  return '"';
		case '\'': return '\'';
		default:
			fprintf(stderr, "Unknown escape sequence");
			exit(1);
		}
	}
	return (c);
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

void scan_string(Lexer* lexer, char* buffer) {
	int i, c;
	for (i = 0; i < MAX_TEXT_LEN - 1; i++) {
		if ((c = scanch(lexer)) == '"') {
			buffer[i] = '\0';
			return 0;
		}
		buffer[i] = c;
	}

	fprintf(stderr, "String literal too long");
	exit(1);
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
	if (strcmp(s, "printchar") == 0) return TOKEN_PRINT2;
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
	case '+': 
		if ((c = next_char(lexer)) == '+') {
			t.tokenType = TOKEN_INCREMENT;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_PLUS;
		}
		break;
	case '-': 
		if ((c = next_char(lexer)) == '-') {
			t.tokenType = TOKEN_DECREMENT;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_MINUS;
		}
		break;
	case '*': t.tokenType = TOKEN_STAR; break;
	case '/': t.tokenType = TOKEN_SLASH; break;
	case ';': t.tokenType = TOKEN_SEMICOLON; break;
	case '(': t.tokenType = TOKEN_LPAREN; break;
	case ')': t.tokenType = TOKEN_RPAREN; break;
	case '[': t.tokenType = TOKEN_LBRACKET; break;
	case ']': t.tokenType = TOKEN_RBRACKET; break;
	case '{': t.tokenType = TOKEN_LBRACE; break;
	case '}': t.tokenType = TOKEN_RBRACE; break;
	case ',': t.tokenType = TOKEN_COMMA; break;
	case '~': t.tokenType = TOKEN_INVERT; break;
	case '^': t.tokenType = TOKEN_XOR; break;
	case '\'': 
		t.value = scanch(lexer);
		t.tokenType = TOKEN_INTLIT;
		if ((c = next_char(lexer)) != '\'') {
			fprintf(stderr, "Expected '\\'' at end of char literal");
			exit(1);
		}
		break;
	case '\"':
		scan_string(lexer, &Text);
		t.tokenType = TOKEN_STRINGLIT;
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
			t.tokenType = TOKEN_NEGATE;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_LOGNOT;
		}
		break;
	case '<':
		if ((c = next_char(lexer)) == '=') {
			t.tokenType = TOKEN_LESSOREQUALS;
		}
		else if (c == '<') {
			t.tokenType = TOKEN_LSHIFT;
		} else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_LESS;
		}
		break;
	case '>':
		if ((c = next_char(lexer)) == '=') {
			t.tokenType = TOKEN_GREATEROREQUALS;
		}
		else if (c == '>') {
			t.tokenType = TOKEN_RSHIFT;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_GREATER;
		}
		break;
	case '&':
		if ((c = next_char(lexer)) == '&') {
			t.tokenType = TOKEN_LOGAND;
		} else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_AMPERSAND;
		}
		break;
	case '|':
		if ((c = next_char(lexer)) == '|') {
			t.tokenType = TOKEN_LOGOR;
		}
		else {
			ungetc(c, lexer->source);
			t.tokenType = TOKEN_OR;
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