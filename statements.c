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
    return tree;
}

void variable_declaration(Lexer* lexer) {
    match(TOKEN_INT, lexer);
    match(TOKEN_IDENTIFIER, lexer);
    addGlobal(Text);
    assembly_generate_global_symbol(Text);
    match(TOKEN_SEMICOLON, lexer);
}

ASTNode* function_declaration(Lexer* lexer) {
    ASTNode* tree;
    int nameslot;
    
    match(TOKEN_VOID, lexer);
    match(TOKEN_IDENTIFIER, lexer);
    nameslot = addGlobal(Text);
    match(TOKEN_LPAREN, lexer);
    match(TOKEN_RPAREN, lexer);

    tree = compound_statement(lexer);

    return ast_new_unary(NODE_FUNCTION, tree, nameslot);
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

ASTNode* while_statement(Lexer* lexer) {
    ASTNode* condAST, *bodyAST;

    match(TOKEN_WHILE, lexer);
    match(TOKEN_LPAREN, lexer);

    condAST = binexpr(lexer, 0);

    if (condAST->type < NODE_EQUALS || condAST->type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "[PARSER] Bad comparison operator on line %d", lexer->curr_line);
        exit(1);
    }

    match(TOKEN_RPAREN, lexer);

    bodyAST = compound_statement(lexer);

    return ast_new_node(NODE_WHILE, condAST, NULL, bodyAST, 0);
}

ASTNode* for_statement(Lexer* lexer) {
    ASTNode* condAST, * bodyAST;
    ASTNode* preopAST, * postopAST;
    ASTNode* tree;

    match(TOKEN_FOR, lexer);
    match(TOKEN_LPAREN, lexer);

    preopAST = single_statement(lexer);
    match(TOKEN_SEMICOLON, lexer);

    condAST = binexpr(lexer, 0);
    if (condAST->type < NODE_EQUALS || condAST->type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "[PARSER] Bad comparison operator on line %d", lexer->curr_line);
        exit(1);
    }
    match(TOKEN_SEMICOLON, lexer);

    postopAST = single_statement(lexer);
    match(TOKEN_RPAREN, lexer);

    bodyAST = compound_statement(lexer);

    tree = ast_new_node(NODE_GLUE, bodyAST, NULL, postopAST, 0);
    tree = ast_new_node(NODE_WHILE, condAST, NULL, tree, 0);

    return ast_new_node(NODE_GLUE, preopAST, NULL, tree, 0);
}

ASTNode* single_statement(Lexer* lexer) {
    switch (lexer->curr_token.tokenType)
    {
    case TOKEN_PRINT: return print_statement(lexer);
    case TOKEN_INT: variable_declaration(lexer); return NULL;
    case TOKEN_IDENTIFIER: return assignment_statement(lexer);
    case TOKEN_IF: return if_statement(lexer);
    case TOKEN_WHILE: return while_statement(lexer);
    case TOKEN_FOR: return for_statement(lexer);
    default:
        printf("Syntax error");
        exit(1);
    }
}

ASTNode* compound_statement(Lexer* lexer) {
    ASTNode *left = NULL;
    ASTNode *tree;

    match(TOKEN_LBRACE, lexer);

    while (1) {

        tree = single_statement(lexer);

        if (tree != NULL && (tree->type == NODE_PRINT || tree->type == NODE_ASSIGN))
            match(TOKEN_SEMICOLON, lexer);

        if (tree != NULL) {
            if (left == NULL) left = tree;
            else left = ast_new_node(NODE_GLUE, left, NULL, tree, 0);
        }

        if (lexer->curr_token.tokenType == TOKEN_RBRACE) {
            match(TOKEN_RBRACE, lexer);
            return left;
        }
    }
}