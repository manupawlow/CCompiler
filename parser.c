#include "parser.h"
#include "code_generator.h"

#define ERROR "Unknown variable %s"

int* _line;
void syntax_error() {
    fprintf(stderr, "[PARSER] syntax error on line %d", *_line);
    exit(1);
}

struct ASTNode* ast_new_node(NodeType type, int value, ASTNode* left, ASTNode* right)
{
    ASTNode* node;
    node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "[PARSER] Unable to create a new ASTNode\n");
        exit(1);
    }

    node->type = type;
    node->left = left;
    node->right = right;
    node->value = value;
    return node;
}

NodeType arithmetic_operation(TokenType tokenType) {
    switch (tokenType) {
    case TOKEN_PLUS:    return (NODE_ADD);
    case TOKEN_MINUS:   return (NODE_SUBTRACT);
    case TOKEN_STAR:    return (NODE_MULTIPLY);
    case TOKEN_SLASH:   return (NODE_DIVIDE);
    default: syntax_error();
    }
}

static struct ASTNode* parse_leaf(Lexer* lexer) {
    ASTNode* n;
    int id;

    switch (lexer->curr_token.tokenType)
    {
    case TOKEN_INTLIT:
        n = ast_new_node(NODE_INTLIT, lexer->curr_token.value, NULL, NULL);
        break;
    case TOKEN_IDENTIFIER:
        id = findSymbol(Text);
        if (id == -1) {
            fprintf(stderr, ERROR, Text);
            exit(1);
        }
        n = ast_new_node(NODE_IDENTIFIER, id, NULL, NULL);
        break;
    default:
        fprintf(stderr, "Syntax error, token %s", Text);
        exit(1);
        break;
    }
    lexer_next_token(lexer);
    return n;
}

//                       EOF +   -   *   /  int
static int _op_prec[] = { 0, 10, 10, 20, 20, 0 };
int operator_precedence(TokenType tokenType) {
    int prec = _op_prec[tokenType];
    if (prec == 0) {
        syntax_error();
    }
    return prec;
}

//binexpr
struct ASTNode* parser_expresion(Lexer* lexer, int prev_precedence) {
    _line = &lexer->curr_line;
    struct ASTNode *left, *right;

    left = parse_leaf(lexer);
    
    TokenType tokenType = lexer->curr_token.tokenType;
    if (tokenType == TOKEN_SEMICOLON) {
        return left;
    }
    
    while (operator_precedence(tokenType) > prev_precedence) {
        
        lexer_next_token(lexer);

        right = parser_expresion(lexer, operator_precedence(tokenType));

        left = ast_new_node(arithmetic_operation(tokenType), 0, left, right);

        tokenType = lexer->curr_token.tokenType;

        if (tokenType == TOKEN_SEMICOLON) {
            return left;
        }
    }

    return left;
}

struct ASTNode* parse(Lexer* lexer) {
    lexer_next_token(lexer);
    _line = &lexer->curr_line;
    return parser_expresion(lexer, 0);
}
