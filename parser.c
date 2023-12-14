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
    node->isRvalue = 0;
    return node;
}

struct ASTNode* ast_new_leaf(OperationType operation, PrimitiveType type, int value) { return ast_new_node(operation, type, NULL, NULL, NULL, value); }
struct ASTNode* ast_new_unary(OperationType operation, PrimitiveType type, struct ASTNode* left, int value) { return ast_new_node(operation, type, left, NULL, NULL, value); }

OperationType arithmetic_operation(_TokenType tokenType) {
    if (tokenType <= TOKEN_EOF || tokenType >= TOKEN_INTLIT) {
        syntax_error();
    }
    return tokenType;
}

struct ASTNode* parse_funccall(Lexer* lexer) {
    struct ASTNode* tree;
    int id;

    if ((id = findSymbol(Text)) == -1) {
        fprintf(stderr, "Undeclared function %s", Text);
        exit(1);
    }

    if (SymbolTable[id].stype != STRU_FUNCTION) {
        fprintf(stderr, "%s is not a function", SymbolTable[id].name);
        exit(1);
    }

    match(TOKEN_LPAREN, lexer);

    tree = binexpr(lexer, 0);

    tree = ast_new_unary(NODE_FUNCCALL, SymbolTable[id].type, tree, id);
    
    match(TOKEN_RPAREN, lexer);

    return tree;
}

struct ASTNode* array_access(Lexer* lexer) {
    struct ASTNode* left, *right;
    int id;

    if ((id = findSymbol(Text)) == -1) {
        fprintf(stderr, "Undeclared array %s", Text);
        exit(1);
    }

    if (SymbolTable[id].stype != STRU_ARRAY) {
        fprintf(stderr, "Identifier %s is not an array", Text);
        exit(1);
    }
    left = ast_new_leaf(NODE_ADDRESS, SymbolTable[id].type, id);
    
    lexer_next_token(lexer);

    right = binexpr(lexer, 0);

    match(TOKEN_RBRACKET, lexer);

    if (!inttype(right->type)) {
        fprintf(stderr, "Array index is not of integer type");
        exit(1);
    }

    right = modify_type(right, left->type, NODE_ADD);

    left = ast_new_node(NODE_ADD, SymbolTable[id].type, left, NULL, right, 0);
    left = ast_new_unary(NODE_DEREFERENCE, value_at(left->type), left, 0);
 
    return left;
}

struct ASTNode* parse_prefix(Lexer* lexer) {
    struct ASTNode* tree;
    switch (lexer->curr_token.tokenType)
    {
    case TOKEN_AMPERSAND:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);

        if (tree->operation != NODE_IDENTIFIER) {
            fprintf(stderr, "& operator must be followed by an identifier");
            exit(1);
        }

        tree->operation = NODE_ADDRESS; 
        tree->type = pointer_to(tree->type);
        break;
    case TOKEN_STAR:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);

        if (tree->operation != NODE_IDENTIFIER && tree->operation != NODE_DEREFERENCE) {
            fprintf(stderr, "& operator must be followed by an identifier");
            exit(1);
        }

        tree = ast_new_unary(NODE_DEREFERENCE, value_at(tree->type), tree, 0);
        break;
    case TOKEN_MINUS:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);
        tree->isRvalue = 1;
        tree = modify_type(tree, PRIM_INT, 0);
        tree = ast_new_unary(NODE_NEGATE, tree->type, tree, 0);
        break;
    case TOKEN_INVERT:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);
        tree->isRvalue = 1;
        tree = ast_new_unary(NODE_INVERT, tree->type, tree, 0);
        break;
    case TOKEN_LOGNOT:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);
        tree->isRvalue = 1;
        tree = ast_new_unary(NODE_LOGNOT, tree->type, tree, 0);
        break;
    case TOKEN_INCREMENT:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);
        if (tree->operation != NODE_IDENTIFIER) {
            fprintf(stderr, "++ operator must be followed by an identifier");
            exit(1);
        }
        tree = ast_new_unary(NODE_PREINC, tree->type, tree, 0);
        break;
    case TOKEN_DECREMENT:
        lexer_next_token(lexer);
        tree = parse_prefix(lexer);
        if (tree->operation != NODE_IDENTIFIER) {
            fprintf(stderr, "-- operator must be followed by an identifier");
            exit(1);
        }
        tree = ast_new_unary(NODE_PREDEC, tree->type, tree, 0);
        break;
    default:
        tree = parse_primary_factor(lexer);
        break;
    }
    return tree;
}

struct ASTNode* parse_postfix(Lexer* lexer) {
    struct ASTNode* n;
    int id;
    
    lexer_next_token(lexer);

    if (lexer->curr_token.tokenType == TOKEN_LPAREN) {
        return parse_funccall(lexer);
    }

    if (lexer->curr_token.tokenType == TOKEN_LBRACKET) {
        return array_access(lexer);
    }

    id = findSymbol(Text);
    if (id == -1 || SymbolTable[id].stype != STRU_VARIABLE) {
        fprintf(stderr, ERROR, Text);
        exit(1);
    }

    switch (lexer->curr_token.tokenType)
    {
    case TOKEN_INCREMENT:
        lexer_next_token(lexer);
        n = ast_new_leaf(NODE_POSTINC, SymbolTable[id].type, id);
        break;
    case TOKEN_DECREMENT:
        lexer_next_token(lexer);
        n = ast_new_leaf(NODE_POSTDEC, SymbolTable[id].type, id);
        break;
    default:
        n = ast_new_leaf(NODE_IDENTIFIER, SymbolTable[id].type, id);
        break;
    }
    return n;
}

//primary expresion
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
    case TOKEN_STRINGLIT:
        id = assembly_generate_global_string(Text);
        n = ast_new_leaf(NODE_STRINGLIT, PRIM_CHARPTR, id);
        break;
    case TOKEN_IDENTIFIER:
        return parse_postfix(lexer);
    default:
        fprintf(stderr, "Syntax error, token %s", Text);
        exit(1);
        break;
    }
    lexer_next_token(lexer);
    return n;
}

static int _op_prec[] = {
    0, 10, 20, 30,                // T_EOF, T_ASSIGN, T_LOGOR, T_LOGAND
    40, 50, 60,                   // T_OR, T_XOR, T_AMPER 
    70, 70,                       // T_EQ, T_NE
    80, 80, 80, 80,               // T_LT, T_GT, T_LE, T_GE
    90, 90,                       // T_LSHIFT, T_RSHIFT
    100, 100,                     // T_PLUS, T_MINUS
    110, 110                      // T_STAR, T_SLASH
};
int operator_precedence(_TokenType tokenType) {
    if (tokenType >= TOKEN_VOID) {
        fprintf(stderr, "Token with no precedence in op_precedence: %d", (int)tokenType);
        exit(1);
    }

    int prec = _op_prec[(int)tokenType];

    if (prec == 0) {
        syntax_error();
    }
    return prec;
}

int rightassoc(_TokenType tokenType) {
    return tokenType == TOKEN_ASSING;
}

//parser_expresion
struct ASTNode* binexpr(Lexer* lexer, int prev_precedence) {
    _line = &lexer->curr_line;
    struct ASTNode* left, * right;
    struct ASTNode *ltemp, *rtemp;
    OperationType operation;
    _TokenType tokenType;

    left = parse_prefix(lexer);
    
    tokenType = lexer->curr_token.tokenType;
    if (tokenType == TOKEN_SEMICOLON || tokenType == TOKEN_RPAREN || tokenType == TOKEN_RBRACKET) {
        left->isRvalue = 1;
        return left;
    }
    
    while (operator_precedence(tokenType) > prev_precedence
        || (rightassoc(tokenType) && operator_precedence(tokenType) == prev_precedence)) {
        
        lexer_next_token(lexer);

        right = binexpr(lexer, operator_precedence(tokenType));

        operation = arithmetic_operation(tokenType);

        if (operation == NODE_ASSIGN) {
            right->isRvalue = 1;
            right = modify_type(right, left->type, 0);
            if (left == NULL || right == NULL) {
                fprintf(stderr, "Incompatible expression in assignment");
                exit(1);
            }
            ltemp = left;
            left = right;
            right = ltemp;
        }
        else {
            left->isRvalue = 1;
            right->isRvalue = 1;

            ltemp = modify_type(left, right->type, operation);
            rtemp = modify_type(right, left->type, operation);

            if (ltemp == NULL && rtemp == NULL) {
                fprintf(stderr, "Incompatible types in binary expression");
                exit(1);
            }

            if (ltemp != NULL) left = ltemp;
            if (rtemp != NULL) right = rtemp;
        }

        left = ast_new_node(arithmetic_operation(tokenType), left->type, left, NULL, right, 0);

        tokenType = lexer->curr_token.tokenType;

        if (tokenType == TOKEN_SEMICOLON || tokenType == TOKEN_RPAREN) {
            left->isRvalue = 1;
            return left;
        }
    }

    left->isRvalue = 1;
    return left;
}

struct ASTNode* parse(Lexer* lexer) {
    lexer_next_token(lexer);
    _line = &lexer->curr_line;
    return binexpr(lexer, 0);
}
