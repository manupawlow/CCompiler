#include "parser.h"
#include "symbols.h"

#define ERROR "Unknown variable %s"

int* _line;
void syntax_error() {
    fprintf(stderr, "[PARSER] syntax error on line %d", *_line);
    exit(1);
}

struct ASTNode* ast_new_node(NodeType type, ASTNode* left, ASTNode* mid, ASTNode* right, int value)
{
    ASTNode* node;
    node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "[PARSER] Unable to create a new ASTNode\n");
        exit(1);
    }

    node->type = type;
    node->left = left;
    node->mid = mid;
    node->right = right;
    node->value = value;
    return node;
}

struct ASTNode* ast_new_leaf(NodeType type, int value) { return ast_new_node(type, NULL, NULL, NULL, value); }
struct ASTNode* ast_new_unary(NodeType type, ASTNode* left, int value) { return ast_new_node(type, left, NULL, NULL, value); }

NodeType arithmetic_operation(TokenType tokenType) {
    if (tokenType <= TOKEN_EOF || tokenType >= TOKEN_INTLIT) {
        syntax_error();
    }
    return tokenType;
}

struct ASTNode* parse_primary_factor(Lexer* lexer) {
    ASTNode* n;
    int id;

    switch (lexer->curr_token.tokenType)
    {
    case TOKEN_INTLIT:
        n = ast_new_leaf(NODE_INTLIT, lexer->curr_token.value);
        //n = ast_new_node(NODE_INTLIT, lexer->curr_token.value, NULL, NULL);
        break;
    case TOKEN_IDENTIFIER:
        id = findGlobal(Text);
        if (id == -1) {
            fprintf(stderr, ERROR, Text);
            exit(1);
        }
        n = ast_new_leaf(NODE_IDENTIFIER, lexer->curr_token.value);
        //n = ast_new_node(NODE_IDENTIFIER, id, NULL, NULL);
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

        left = ast_new_node(arithmetic_operation(tokenType), left, NULL, right, 0);

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
