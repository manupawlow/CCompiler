#include "statements.h"

void match(TokenType type, Lexer* lexer) {
    if (lexer->curr_token.tokenType != type) {
        printf("Unexpected token on line %d\n", lexer->curr_line);
        exit(1);
    }
    lexer_next_token(lexer);
}

ASTNode* print_statement(Lexer* lexer) {
    ASTNode* tree;
    int reg;
    match(TOKEN_PRINT, lexer);
    tree = binexpr(lexer, 0);
    tree = ast_new_unary(NODE_PRINT, tree, 0);
    match(TOKEN_SEMICOLON, lexer);
    return tree;
}

void int_statement(Lexer* lexer) {
    match(TOKEN_INT, lexer);
    match(TOKEN_IDENTIFIER, lexer);
    addGlobal(Text);
    assembly_generate_global_symbol(Text);
    match(TOKEN_SEMICOLON, lexer);
}

ASTNode* assignment_statement(Lexer* lexer) {

    match(TOKEN_IDENTIFIER, lexer);

    ASTNode* left, * right, * tree;
    int id;

    if ((id = findGlobal(Text)) == -1) {
        fprintf(stderr, "Undeclared variable %s", Text);
        exit(1);
    }

    right = ast_new_leaf(NODE_LVALUE_IDENTIFIER, id);

    match(TOKEN_ASSING, lexer);

    left = binexpr(lexer, 0);

    tree = ast_new_node(NODE_ASSIGN, left, NULL, right, 0);

    match(TOKEN_SEMICOLON, lexer);

    return tree;
}

ASTNode* if_statement(Lexer* lexer) {
    ASTNode *condAST, *trueAST, *falseAST = NULL;
    
    match(TOKEN_IF, lexer);
    match(TOKEN_LPAREN, lexer);

    condAST = binexpr(lexer, 0);

    if (condAST->type < NODE_EQUALS || condAST->type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "[PARSER] Bad comparison operator on line %d", lexer->curr_line);
        exit(1);
    }

    match(TOKEN_RPAREN, lexer);

    trueAST = compound_statement(lexer);

    if (lexer->curr_token.tokenType == TOKEN_ELSE) {
        lexer_next_token(lexer);
        falseAST = compound_statement(lexer);
    }

    return ast_new_node(NODE_IF, condAST, trueAST, falseAST, 0);
}

ASTNode* compound_statement(Lexer* lexer) {
    ASTNode *left = NULL;
    ASTNode *tree;

    match(TOKEN_LBRACE, lexer);

    while (1) {
        switch (lexer->curr_token.tokenType)
        {
        case TOKEN_PRINT:
            tree = print_statement(lexer);
            break;
        case TOKEN_INT:
            int_statement(lexer);
            tree = NULL;
            break;
        case TOKEN_IDENTIFIER:
            tree = assignment_statement(lexer);
            break;
        case TOKEN_IF:
            tree = if_statement(lexer);
            break;
        case TOKEN_RBRACE:
            match(TOKEN_RBRACE, lexer);
            return left;
        default:
            printf("Syntax error");
            exit(1);
        }

        if (tree) {
            if (left == NULL) {
                left = tree;
            } else {
                left = ast_new_node(NODE_GLUE, 0, left, NULL, tree);
            }
        }
    }
}