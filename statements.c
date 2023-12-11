#include "statements.h"

//declarations
void global_declarations(Lexer* lexer) {
    struct ASTNode* tree;
    PrimitiveType type;

    while (1) {
        type = parse_type(lexer);
        match(TOKEN_IDENTIFIER, lexer);

        if (lexer->curr_token.tokenType == TOKEN_LPAREN) {
            tree = function_declaration(lexer, type);
            if (O_dumpAST || 1) {
                fprintf(stdout, "\n\n");
                dumpAST(tree, 0, 0);
                fprintf(stdout, "\n\n");
            }
            assembly_ast_node(tree, -1, 0);
        } else {
            variable_declaration(lexer, type);
        }
        if (lexer->curr_token.tokenType == TOKEN_EOF)
            break;
    }
}

void variable_declaration(Lexer* lexer, PrimitiveType type) {
    int id;

    if (lexer->curr_token.tokenType == TOKEN_LBRACKET) {
        lexer_next_token(lexer);

        if (lexer->curr_token.tokenType == TOKEN_INTLIT) {
            id = addGlobal(Text, pointer_to(type), STRU_ARRAY, 0, lexer->curr_token.value);
            assembly_generate_global_symbol(id);
        }
        lexer_next_token(lexer);
        match(TOKEN_RBRACKET, lexer);
    }
    else {
        while (1) {
            id = addGlobal(Text, type, STRU_VARIABLE, 0, 1);
            assembly_generate_global_symbol(id);

            if (lexer->curr_token.tokenType == TOKEN_SEMICOLON) {
                lexer_next_token(lexer);
                return;
            }

            if (lexer->curr_token.tokenType == TOKEN_COMMA) {
                lexer_next_token(lexer);
                match(TOKEN_IDENTIFIER, lexer);
                continue;
            }

            fprintf(stderr, "Missing , or ; after identifier");
            exit(1);
        }
    }
    match(TOKEN_SEMICOLON, lexer);
}

struct ASTNode* function_declaration(Lexer* lexer, PrimitiveType type) {
    struct ASTNode* tree, * finalstmt;
    int nameslot, endlabel;

    //type = parse_type(lexer->curr_token.tokenType, lexer);
    //match(TOKEN_IDENTIFIER, lexer);

    endlabel = label_id();
    nameslot = addGlobal(Text, type, STRU_FUNCTION, endlabel, 1);
    Functionid = nameslot;

    match(TOKEN_LPAREN, lexer);
    match(TOKEN_RPAREN, lexer);

    tree = compound_statement(lexer);

    if (type != PRIM_VOID) {
        finalstmt = tree->operation == NODE_GLUE ? tree->right : tree;
        if (finalstmt == NULL || finalstmt->operation != NODE_RETURN) {
            fprintf(stderr, "No return for function with non-void type");
            exit(1);
        }
    }

    return ast_new_unary(NODE_FUNCTION, PRIM_VOID, tree, nameslot);
}

//statements
struct ASTNode* return_statement(Lexer* lexer) {
    struct ASTNode* tree;

    if (GlobalSymbols[Functionid].type == PRIM_VOID) {
        fprintf(stderr, "Can't return from a void function");
        exit(1);
    }

    match(TOKEN_RETURN, lexer);
    match(TOKEN_LPAREN, lexer);

    tree = binexpr(lexer, 0);
    tree = modify_type(tree, GlobalSymbols[Functionid].type, 0);
    if (tree == NULL) {
        fprintf(stderr, "Incompatible return type");
        exit(1);
    }
    
    tree = ast_new_unary(NODE_RETURN, PRIM_NONE, tree, 0);

    match(TOKEN_RPAREN, lexer);

    return tree;
}

struct ASTNode* print_statement(Lexer* lexer) {
    struct ASTNode* tree;
    PrimitiveType leftType, rightType;
    int reg;

    match(TOKEN_PRINT, lexer);
    match(TOKEN_LPAREN, lexer);

    tree = binexpr(lexer, 0);
    tree = modify_type(tree, PRIM_LONG, 0);
    if (tree == NULL) {
        fprintf(stderr, "Incompatible expression in assignment");
        exit(1);
    }

    tree = ast_new_unary(NODE_PRINT, PRIM_NONE, tree, 0);

    match(TOKEN_RPAREN, lexer);

    return tree;
}

struct ASTNode* print2_statement(Lexer* lexer) {
    struct ASTNode* tree;
    PrimitiveType leftType, rightType;
    int reg;

    match(TOKEN_PRINT2, lexer);
    match(TOKEN_LPAREN, lexer);

    tree = binexpr(lexer, 0);
    tree = modify_type(tree, PRIM_CHAR, 0);
    if (tree == NULL) {
        fprintf(stderr, "Incompatible expression in assignment");
        exit(1);
    }

    tree = ast_new_unary(NODE_PRINT2, PRIM_NONE, tree, 0);

    match(TOKEN_RPAREN, lexer);

    return tree;
}

//unused
struct ASTNode* assignment_statement(Lexer* lexer) {
    struct ASTNode* left, * right, * tree;
    int id;

    match(TOKEN_IDENTIFIER, lexer);

    if (lexer->curr_token.tokenType == TOKEN_LPAREN) {
        return parse_funccall(lexer);
    }

    if ((id = findGlobal(Text)) == -1) {
        fprintf(stderr, "Undeclared variable %s", Text);
        exit(1);
    }

    right = ast_new_leaf(NODE_LVALUEIDENT, GlobalSymbols[id].type, id);

    match(TOKEN_ASSING, lexer);

    left = binexpr(lexer, 0);
    left = modify_type(left, right->type, 0);
    if (left == NULL) {
        fprintf(stderr, "Incompatible expression in assignment");
        exit(1);
    }

    tree = ast_new_node(NODE_ASSIGN, PRIM_INT, left, NULL, right, 0);

    return tree;
}

struct ASTNode* if_statement(Lexer* lexer) {
    struct ASTNode* condAST, * trueAST, * falseAST = NULL;

    match(TOKEN_IF, lexer);
    match(TOKEN_LPAREN, lexer);

    condAST = binexpr(lexer, 0);

    if (condAST->operation < NODE_EQUALS || condAST->operation > NODE_GREATEROREQUALS) {
        condAST = ast_new_unary(NODE_TOBOOL, condAST->type, condAST, 0);
    }

    match(TOKEN_RPAREN, lexer);

    trueAST = compound_statement(lexer);

    if (lexer->curr_token.tokenType == TOKEN_ELSE) {
        lexer_next_token(lexer);
        falseAST = compound_statement(lexer);
    }

    return ast_new_node(NODE_IF, PRIM_NONE, condAST, trueAST, falseAST, 0);
}

struct ASTNode* while_statement(Lexer* lexer) {
    struct ASTNode* condAST, * bodyAST;

    match(TOKEN_WHILE, lexer);
    match(TOKEN_LPAREN, lexer);

    condAST = binexpr(lexer, 0);

    if (condAST->operation < NODE_EQUALS || condAST->operation > NODE_GREATEROREQUALS) {
        condAST = ast_new_unary(NODE_TOBOOL, condAST->type, condAST, 0);
    }

    match(TOKEN_RPAREN, lexer);

    bodyAST = compound_statement(lexer);

    return ast_new_node(NODE_WHILE, PRIM_NONE, condAST, NULL, bodyAST, 0);
}

struct ASTNode* for_statement(Lexer* lexer) {
    struct ASTNode* condAST, * bodyAST;
    struct ASTNode* preopAST, * postopAST;
    struct ASTNode* tree;

    match(TOKEN_FOR, lexer);
    match(TOKEN_LPAREN, lexer);

    preopAST = single_statement(lexer);
    match(TOKEN_SEMICOLON, lexer);

    condAST = binexpr(lexer, 0);
    if (condAST->operation < NODE_EQUALS || condAST->operation > NODE_GREATEROREQUALS) {
        condAST = ast_new_unary(NODE_TOBOOL, condAST->type, condAST, 0);
    }
    match(TOKEN_SEMICOLON, lexer);

    postopAST = single_statement(lexer);
    match(TOKEN_RPAREN, lexer);

    bodyAST = compound_statement(lexer);

    tree = ast_new_node(NODE_GLUE, PRIM_NONE, bodyAST, NULL, postopAST, 0);
    tree = ast_new_node(NODE_WHILE, PRIM_NONE, condAST, NULL, tree, 0);

    return ast_new_node(NODE_GLUE, PRIM_NONE, preopAST, NULL, tree, 0);
}

struct ASTNode* single_statement(Lexer* lexer) {
    PrimitiveType type;

    switch (lexer->curr_token.tokenType)
    {
    case TOKEN_PRINT: return print_statement(lexer);
    case TOKEN_PRINT2: return print2_statement(lexer);
    case TOKEN_CHAR:
    case TOKEN_INT:
    case TOKEN_LONG:
        type = parse_type(lexer);
        match(TOKEN_IDENTIFIER, lexer);
        variable_declaration(lexer, type);
        return NULL;
    //case TOKEN_IDENTIFIER: return assignment_statement(lexer);
    case TOKEN_IF: return if_statement(lexer);
    case TOKEN_WHILE: return while_statement(lexer);
    case TOKEN_FOR: return for_statement(lexer);
    case TOKEN_RETURN: return return_statement(lexer);
    default: return binexpr(lexer, 0);
    }
}

struct ASTNode* compound_statement(Lexer* lexer) {
    struct ASTNode* left = NULL;
    struct ASTNode *tree;

    match(TOKEN_LBRACE, lexer);

    while (1) {

        tree = single_statement(lexer);

        if (tree != NULL && (tree->operation == NODE_PRINT || tree->operation == NODE_PRINT2 || tree->operation == NODE_ASSIGN || tree->operation == NODE_RETURN || tree->operation == NODE_FUNCCALL))
            match(TOKEN_SEMICOLON, lexer);

        if (tree != NULL) {
            if (left == NULL) left = tree;
            else left = ast_new_node(NODE_GLUE, PRIM_NONE, left, NULL, tree, 0);
        }

        if (lexer->curr_token.tokenType == TOKEN_RBRACE) {
            match(TOKEN_RBRACE, lexer);
            return left;
        }
    }
}