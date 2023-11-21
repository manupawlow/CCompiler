#include "parser.h"
#include "symbols.h"

#define ERROR "Unknown variable %s"

int* _line;
void syntax_error() {
    fprintf(stderr, "[PARSER] syntax error on line %d", *_line);
    exit(1);
}

struct ASTNode* ast_new_node(OperationType operation, PrimitiveType type, struct ASTNode* left, struct ASTNode* mid, struct ASTNode* right, int value)
{
    struct ASTNode* node;
    node = (struct ASTNode*)malloc(sizeof(struct ASTNode));
    if (node == NULL) {
        fprintf(stderr, "[PARSER] Unable to create a new ASTNode\n");
        exit(1);
    }

    node->operation = operation;
    node->type = type;
    node->left = left;
    node->mid = mid;
    node->right = right;
    node->value = value;
    return node;
}

struct ASTNode* ast_new_leaf(OperationType operation, PrimitiveType type, int value) { return ast_new_node(operation, type, NULL, NULL, NULL, value); }
struct ASTNode* ast_new_unary(OperationType operation, PrimitiveType type, struct ASTNode* left, int value) { return ast_new_node(operation, type, left, NULL, NULL, value); }

OperationType arithmetic_operation(TokenType tokenType) {
    if (tokenType <= TOKEN_EOF || tokenType >= TOKEN_INTLIT) {
        syntax_error();
    }
    return tokenType;
}

struct ASTNode* parse_primary_factor(Lexer* lexer) {
    struct ASTNode* n;
    int id;

    Token t = lexer->curr_token;
    switch (t.tokenType)
    {
    case TOKEN_INTLIT:
        if (t.value >= 0 && t.value < 256)
            n = ast_new_leaf(NODE_INTLIT, PRIM_CHAR, lexer->curr_token.value);
        else 
            n = ast_new_leaf(NODE_INTLIT, PRIM_INT, lexer->curr_token.value);
        break;
    case TOKEN_IDENTIFIER:
        id = findGlobal(Text);
        if (id == -1) {
            fprintf(stderr, ERROR, Text);
            exit(1);
        }
        n = ast_new_leaf(NODE_IDENTIFIER, GlobalSymbols[id].type, id);
        break;
    default:
        fprintf(stderr, "Syntax error, token %s", Text);
        exit(1);
        break;
    }
    lexer_next_token(lexer);
    return n;
}

static int _op_prec[] = { 
    0,              // TOKEN_EOF
    10, 10,         // TOKEN_PLUS, TOKEN_MINUS
    20, 20,         // TOKEN_STAR, TOKEN_SLASH
    30, 30,         // TOKEN_EQUALS, TOKEN_NOTE
    40, 40, 40, 40, // TOKEN_LESS, TOKEN_GREATER, TOKEN_LESSOREQUALS, TOKEN_GREATEROREQUALS
};
int operator_precedence(TokenType tokenType) {
    int prec = _op_prec[tokenType];
    if (prec == 0) {
        syntax_error();
    }
    return prec;
}

//parser_expresion
struct ASTNode* binexpr(Lexer* lexer, int prev_precedence) {
    _line = &lexer->curr_line;
    struct ASTNode *left, *right;

    left = parse_primary_factor(lexer);
    
    TokenType tokenType = lexer->curr_token.tokenType;
    if (tokenType == TOKEN_SEMICOLON || tokenType == TOKEN_RPAREN) {
        return left;
    }
    
    while (operator_precedence(tokenType) > prev_precedence) {
        
        lexer_next_token(lexer);

        right = binexpr(lexer, operator_precedence(tokenType));

        left = ast_new_node(arithmetic_operation(tokenType), left->type, left, NULL, right, 0);

        tokenType = lexer->curr_token.tokenType;

        if (tokenType == TOKEN_SEMICOLON || tokenType == TOKEN_RPAREN) {
            return left;
        }
    }

    return left;
}

struct ASTNode* parse(Lexer* lexer) {
    lexer_next_token(lexer);
    _line = &lexer->curr_line;
    return binexpr(lexer, 0);
}
