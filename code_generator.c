#include "code_generator.h"

static FILE* OutFile;

static int freereg[4];
static char* reglist[4] = { "r8", "r9", "r10", "r11" };

void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

static int alloc_register(void)
{
    for (int i = 0; i < 4; i++) {
        if (freereg[i]) {
            freereg[i] = 0;
            return(i);
        }
    }
    fprintf(stderr, "[CG] Out of registers");
    exit(1);
}

static void free_register(int reg)
{
    if (freereg[reg] != 0) {
        fprintf(stderr, "Error trying to free register %d\n", reg);
        exit(1);
    }
    freereg[reg] = 1;
}

void assembly_preamble()
{
    freeall_registers();
    fputs(
        "global main\n"
        "\n"
        "extern printf\n"
        "\n"
        "section .data\n"
        "\tformatStr\tdb '%d', 0xA\n"
        "\n"
        "section .text\n"
        "\n"
        "main:\n"
        "",
        OutFile);
}

void assembly_postamble()
{

    //fprintf(OutFile, 
    //    "\n"
    //    "\t; exit syscall\n"
    //    "\tmov rax, 60\t\t; número de syscall para exit\n"
    //    "\tmov rdi, %s\t\t; código de salida 0\n"
    //    "\tsyscall\n"
    //, reglist[r]);

    fputs(
        "\n"
        "\t; exit syscall\n"
        "\tmov\t\trax, 60\t\t; número de syscall para exit\n"
        "\txor\t\trdi, rdi\t\t; código de salida 0\n"
        "\tsyscall\n",
        OutFile);
}

int assembly_load_int(int value) {
    int r = alloc_register();
    fprintf(OutFile, "\tmov\t\t%s, %d\t\t; load %d into %s\n", reglist[r], value, value, reglist[r]);
    return r;
}

int assembly_add(int r1, int r2) {
    fprintf(OutFile, "\tadd\t\t%s, %s\t\t; add %s, %s\n", reglist[r2], reglist[r1], reglist[r2], reglist[r1]);
    free_register(r1);
    return r2;
}

int assembly_sub(int r1, int r2) {
    fprintf(OutFile, "\tsub\t\t%s, %s\t\t; sub %s, %s\n", reglist[r1], reglist[r2], reglist[r1], reglist[r2]);
    free_register(r2);
    return r1;
}

int assembly_mul(int r1, int r2) {
    fprintf(OutFile, "\timul\t\t%s, %s\t\t; mul %s, %s\n", reglist[r2], reglist[r1], reglist[r2], reglist[r1]);
    free_register(r1);
    return(r2);
}

int assembly_div(int r1, int r2) {
    fprintf(OutFile, "\tmov\t\trax, %s\t\t; div %s, %s\n", reglist[r1], reglist[r1], reglist[r2]);
    fprintf(OutFile, "\txor\t\trdx, rdx\n");
    fprintf(OutFile, "\tidiv\t%s\n", reglist[r2]);
    fprintf(OutFile, "\tmov\t\t%s, rax\t\t; result in rax, rest in rdx\n", reglist[r1]);
    free_register(r2);
    return(r1);
}

void assembly_printint(int r) {
    fprintf(OutFile, "\tpush\trbp\t\t; printint\n");
    fprintf(OutFile, "\tmov\t\trdi, formatStr\n");
    fprintf(OutFile, "\tmov\t\trsi, %s\n", reglist[r]);
    fprintf(OutFile, "\tmov\t\tal, 0\n");
    fprintf(OutFile, "\tcall\tprintf\n");
    fprintf(OutFile, "\tpop\t\trbp\n");
    free_register(r);
}

int assembly_load_identifier(char* symbol) {
    int r = alloc_register();
    fprintf(OutFile, "\tmov\t%s, [%s]\n", symbol, reglist[r]);
    return r;
}

int assembly_store_variable(int r, char* identifier) {
    fprintf(OutFile, "\t.comm\t%s, 8, 8%s\n", identifier, reglist[r]);
    return r;
}

//genAST
int assembly_ast_node(ASTNode* node, int reg) {
	int leftRegister = -1, rightRegister = -1;

	if (node->left)
        leftRegister = assembly_ast_node(node->left, -1);
	if (node->right)
        rightRegister = assembly_ast_node(node->right, leftRegister);

    switch (node->type) {
    case NODE_ADD: return assembly_add(leftRegister, rightRegister);
    case NODE_SUBTRACT: return assembly_sub(leftRegister, rightRegister);
    case NODE_MULTIPLY: return assembly_mul(leftRegister, rightRegister);
    case NODE_DIVIDE: return assembly_div(leftRegister, rightRegister);
    case NODE_INTLIT: return assembly_load_int(node->value);
    case NODE_IDENTIFIER: return assembly_load_identifier(SymTable[node->value].name);
    case NODE_LVALUE_IDENTIFIER: return assembly_store_variable(reg, node->value);
    case NODE_ASSIGN: return rightRegister;
    default:
        fprintf(stderr, "[CG] Unknown AST operator %d\n", node->type);
        exit(1);
    }
}

//statements

int findSymbol(char* symbol) {
    int i;
    for (i = 0; i < SymTablePosition; i++) {
        if (*symbol == *SymTable[i].name && !strcmp(symbol, SymTable[i].name))
            return (i);
    }
    return (-1);
}

int newSymbolSlot(void) {
    int p;
    if ((p = SymTablePosition++) >= NSYMBOLS){
        fprintf(stderr, "Too many global symbols");
        exit(1);
    }
    return (p);
}

int addSymbol(char* name) {
    int y;

    if ((y = findSymbol(name)) != -1)
        return (y);

    y = newSymbolSlot();
    SymTable[y].name = _strdup(name);
    return (y);
}

void match(TokenType type, Lexer* lexer) {
    if (lexer->curr_token.tokenType != type) {
        printf("Unexpected token on line %d\n", lexer->curr_line);
        exit(1);
    }
    lexer_next_token(lexer);
}

void print_statement(Lexer* lexer) {
    ASTNode* tree;
    int reg;
    match(TOKEN_PRINT, lexer);
    tree = parser_expresion(lexer, 0);
    reg = assembly_ast_node(tree, -1);
    assembly_printint(reg);
    freeall_registers();
    match(TOKEN_SEMICOLON, lexer);
}

void int_statement(Lexer* lexer) {
    match(TOKEN_INT, lexer);
    match(TOKEN_IDENTIFIER, lexer);
    addSymbol(Text);
    //genglobsym(Text);
    match(TOKEN_SEMICOLON, lexer);
}

void assignment_statement(Lexer* lexer) {
    
    match(TOKEN_IDENTIFIER, lexer);

    ASTNode *left, *right, *tree;
    int id;

    if ((id = findSymbol(Text)) == -1) {
        fprintf(stderr, "Undeclared variable %s", Text);
        exit(1);
    }

    right = ast_new_node(NODE_INTLIT, id, NULL, NULL);

    match(TOKEN_EQUALS, lexer);

    left = parser_expresion(lexer, 0);

    tree = ast_new_node(NODE_ASSIGN, 0, left, right);

    assembly_ast_node(tree, -1);
    freeall_registers();

    match(TOKEN_SEMICOLON, lexer);
}

int statements(Lexer* lexer) {
    while (1) {

        switch (lexer->curr_token.tokenType)
        {
        case TOKEN_PRINT:
            print_statement(lexer);
            break;
        case TOKEN_INT:
            int_statement(lexer);
            break;
        case TOKEN_IDENTIFIER:
            assignment_statement(lexer);
            break;
        case TOKEN_END:
            return;
        default:
            printf("Syntax error");
            exit(1);
        }
    }
}

void generate_code(FILE* outfile, Lexer* lexer) {
	OutFile = outfile;

    lexer_next_token(lexer);

    assembly_preamble();
    statements(lexer);
    assembly_postamble();
}