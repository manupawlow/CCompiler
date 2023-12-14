#include "definitions.h"

Globs = 0;
Locls = NSYMBOLS - 1;

void match(_TokenType type, Lexer* lexer) {
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

char* token_to_string(_TokenType t) {
	if (t == TOKEN_EOF)              return "TOKEN_EOF            ";

	if (t == TOKEN_PLUS)             return "TOKEN_PLUS           ";
	if (t == TOKEN_MINUS)            return "TOKEN_MINUS          ";
	if (t == TOKEN_STAR)             return "TOKEN_STAR           ";
	if (t == TOKEN_SLASH)            return "TOKEN_SLASH          ";
	if (t == TOKEN_EQUALS)           return "TOKEN_EQUALS         ";
	if (t == TOKEN_NEGATE)        return "TOKEN_NEGATE      ";
	if (t == TOKEN_LESS)             return "TOKEN_LESS           ";
	if (t == TOKEN_GREATER)          return "TOKEN_GREATER        ";
	if (t == TOKEN_LESSOREQUALS)     return "TOKEN_LESSOREQUALS   ";
	if (t == TOKEN_GREATEROREQUALS)  return "TOKEN_GREATEROREQUALS";

	if (t == TOKEN_AMPERSAND)		 return "TOKEN_AMPERSAND      ";
	if (t == TOKEN_LOGAND)				 return "TOKEN_LOGAND            ";

	if (t == TOKEN_ASSING)           return "TOKEN_ASSING         ";
	if (t == TOKEN_SEMICOLON)        return "TOKEN_SEMICOLON      ";
	if (t == TOKEN_INTLIT)           return "TOKEN_INTLIT         ";
	if (t == TOKEN_IDENTIFIER)       return "TOKEN_IDENTIFIER     ";
	if (t == TOKEN_LPAREN)           return "TOKEN_LPAREN         ";
	if (t == TOKEN_RPAREN)           return "TOKEN_RPAREN         ";
	if (t == TOKEN_LBRACE)           return "TOKEN_LBRACE         ";
	if (t == TOKEN_RBRACE)           return "TOKEN_RBRACE         ";
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

static int gendumplabel(void) {
    static int id = 1;
    return (id++);
}

void dumpAST(struct ASTNode* n, int label, int level) {
    int Lfalse, Lstart, Lend;

    switch (n->operation) {
    case NODE_IF:
        Lfalse = gendumplabel();
        for (int i = 0; i < level; i++) fprintf(stdout, " ");
        fprintf(stdout, "NODE_IF");
        if (n->right) {
            Lend = gendumplabel();
            fprintf(stdout, ", end L%d", Lend);
        }
        fprintf(stdout, "\n");
        dumpAST(n->left, Lfalse, level + 2);
        dumpAST(n->mid, 0, level + 2);
        if (n->right) dumpAST(n->right, 0, level + 2);
        return;
    case NODE_WHILE:
        Lstart = gendumplabel();
        for (int i = 0; i < level; i++) fprintf(stdout, " ");
        fprintf(stdout, "NODE_WHILE, start L%d\n", Lstart);
        Lend = gendumplabel();
        dumpAST(n->left, Lend, level + 2);
        dumpAST(n->right, 0, level + 2);
        return;
    }

    // Reset level to -2 for NODE_GLUE
    if (n->operation == NODE_GLUE) level = -2;

    // General AST node handling
    if (n->left) dumpAST(n->left, 0, level + 2);
    if (n->right) dumpAST(n->right, 0, level + 2);


    for (int i = 0; i < level; i++) fprintf(stdout, " ");
    switch (n->operation) {
    case NODE_GLUE:
        fprintf(stdout, "\n\n"); return;
    case NODE_FUNCTION:
        fprintf(stdout, "NODE_FUNCTION %s\n", SymbolTable[n->value].name); return;
    case NODE_ADD:
        fprintf(stdout, "NODE_ADD\n"); return;
    case NODE_SUBTRACT:
        fprintf(stdout, "NODE_SUBTRACT\n"); return;
    case NODE_MULTIPLY:
        fprintf(stdout, "NODE_MULTIPLY\n"); return;
    case NODE_DIVIDE:
        fprintf(stdout, "NODE_DIVIDE\n"); return;
    case NODE_EQUALS:
        fprintf(stdout, "NODE_EQ\n"); return;
    case NODE_NOTEQUALS:
        fprintf(stdout, "NODE_NE\n"); return;
    case NODE_LESS:
        fprintf(stdout, "NODE_LE\n"); return;
    case NODE_GREATER:
        fprintf(stdout, "NODE_GT\n"); return;
    case NODE_LESSOREQUALS:
        fprintf(stdout, "NODE_LE\n"); return;
    case NODE_GREATEROREQUALS:
        fprintf(stdout, "NODE_GE\n"); return;
    case NODE_INTLIT:
        fprintf(stdout, "NODE_INTLIT %d\n", n->value); return;
    case NODE_IDENTIFIER:
        if (n->isRvalue)
            fprintf(stdout, "NODE_IDENT rval %s\n", SymbolTable[n->value].name);
        else
            fprintf(stdout, "NODE_IDENT %s\n", SymbolTable[n->value].name);
        return;
    case NODE_ASSIGN:
        fprintf(stdout, "NODE_ASSIGN\n"); return;
    case NODE_WIDEN:
        fprintf(stdout, "NODE_WIDEN\n"); return;
    case NODE_RETURN:
        fprintf(stdout, "NODE_RETURN\n"); return;
    case NODE_FUNCCALL:
        fprintf(stdout, "NODE_FUNCCALL %s\n", SymbolTable[n->value].name); return;
    case NODE_ADDRESS:
        fprintf(stdout, "NODE_ADDR %s\n", SymbolTable[n->value].name); return;
    case NODE_DEREFERENCE:
        if (n->isRvalue)
            fprintf(stdout, "NODE_DEREF rval\n");
        else
            fprintf(stdout, "NODE_DEREF\n");
        return;
    case NODE_SCALE:
        fprintf(stdout, "NODE_SCALE %d\n", n->value); return;
    case NODE_STRINGLIT:
        fprintf(stdout, "NODE_STRINGLIT rval label L%d\n", n->value); return;
    default:
        printf("asdasdsa");
        //exit(1);
    }
}