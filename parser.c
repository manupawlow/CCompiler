#include "parser.h"

Parser parser_new(Lexer* lexer) {
    Parser p = { lexer };
    return p;
}

static struct ASTNode* ast_new_node(NodeType type, int value, ASTNode* left, ASTNode* right)
{
    ASTNode* node;
    node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Unable to create a new ASTNode\n");
        exit(1);
    }
    node->type = type;
    node->left = left;
    node->right = right;
    node->value = value;
    return node;
}

NodeType tokenType_to_NodeType(TokenType token_type) {
    switch (token_type) {
    case TOKEN_PLUS:    return (NODE_ADD);
    case TOKEN_MINUS:   return (NODE_SUBTRACT);
    case TOKEN_STAR:    return (NODE_MULTIPLY);
    case TOKEN_SLASH:   return (NODE_DIVIDE);
    default:
        fprintf(stderr, "syntax error on line");
        exit(1);
    }
}

static struct ASTNode* parse_leaf(Lexer* lexer) {
    Token t = lexer_next_token(lexer);
    if (t.tokenType != TOKEN_INTLIT) {
        fprintf(stderr, "syntax error on line");
        exit(1);
    }
    return ast_new_node(NODE_INTLIT, t.value, NULL, NULL);
}

static struct ASTNode* parser_expresion(Lexer* lexer) {
    struct ASTNode* left;
    left = parse_leaf(lexer);

    Token t = lexer_next_token(lexer);
    if (t.tokenType == TOKEN_END) {
        return left;
    }
    NodeType operator_type = tokenType_to_NodeType(t.tokenType);

    ASTNode* rigth;
    rigth = parser_expresion(lexer);

    ASTNode* node;
    node = ast_new_node(operator_type, 0, left, rigth);

    return node;
}

struct ASTNode* parser_parse(Parser* parser) {
    return parser_expresion(parser->lexer);
}
