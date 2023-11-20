#include "code_generator.h"

static int freereg[4];
static char* reglist[4] = { "r8", "r9", "r10", "r11" };
static char* breglist[4] = { "r8b", "r9b", "r10b", "r11b" };

void setOutputFile(FILE* output) {
    OutFile = output;
}

static int label_id(void) {
    static int id = 1;
    return (id++);
}

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
    //fputs("\tglobal\tmain\n"
    //    "\textern\tprintf\n"
    //    "\tsection\t.text\n"
    //    "LC0:\tdb\t\"%d\",10,0\n"
    //    "printint:\n"
    //    "\tpush\trbp\n"
    //    "\tmov\trbp, rsp\n"
    //    "\tsub\trsp, 16\n"
    //    "\tmov\t[rbp-4], edi\n"
    //    "\tmov\teax, [rbp-4]\n"
    //    "\tmov\tesi, eax\n"
    //    "\tlea	rdi, [rel LC0]\n"
    //    "\tmov	eax, 0\n"
    //    "\tcall	printf\n" "\tnop\n" "\tleave\n" "\tret\n" "\n"
    //    ";;;;;;;;;;;;;;;;;;;;;;\n"
    //    , OutFile);
    //fputs(
    //    "global main\n"
    //    "\n"
    //    "extern printf\n"
    //    "\n"
    //    "section .data\n"
    //    "\t_printint_format db '%d', 0xA\n"
    //    "\n"
    //    "section .text\n"
    //    "\n"
    //    "main:\n"
    //    ";;;;;;;;;;;;;;;;;;;;;;\n",
    //    OutFile);
    fputs(
        "extern printf\n"
        "\n"
        "section .data\n"
        "\t_printint_format db '%d', 0xA\n"
        "\n",
        OutFile);
}

void assembly_postamble()
{
    fputs(
        "\tmov  \teax, 0\n"
        "\tpop  \trbp\n" 
        "\tret\n"
        , OutFile);
    //fputs(
    //    "\n"
    //    "\tmov  \trax, 60\t\t; exit syscall\n"
    //    "\txor  \trdi, rdi\t\t; exit code 0\n"
    //    "\tsyscall\n",
    //    OutFile);
}

int assembly_load_int(int value) {
    int r = alloc_register();
    fprintf(OutFile, "\tmov  \t%s, %d\t\t; %s <- %d\n", reglist[r], value, reglist[r], value);
    return r;
}

int assembly_load_global(char* identifier) {
    int r = alloc_register();
    fprintf(OutFile, "\tmov  \t%s, [%s]", reglist[r], identifier);
    return r;
}

int assembly_add(int r1, int r2) {
    fprintf(OutFile, "\tadd  \t%s, %s\t\t; %s + %s\n", reglist[r2], reglist[r1], reglist[r2], reglist[r1]);
    free_register(r1);
    return r2;
}

int assembly_sub(int r1, int r2) {
    fprintf(OutFile, "\tsub  \t%s, %s\t\t; %s - %s\n", reglist[r1], reglist[r2], reglist[r1], reglist[r2]);
    free_register(r2);
    return r1;
}

int assembly_mul(int r1, int r2) {
    fprintf(OutFile, "\timul  \t%s, %s\t\t; %s * %s\n", reglist[r2], reglist[r1], reglist[r2], reglist[r1]);
    free_register(r1);
    return(r2);
}

int assembly_div(int r1, int r2) {
    fprintf(OutFile, "\tmov  \trax, %s\t\t; %s / %s\n", reglist[r1], reglist[r1], reglist[r2]);
    fprintf(OutFile, "\txor  \trdx, rdx\n");
    fprintf(OutFile, "\tidiv \t%s\n", reglist[r2]);
    fprintf(OutFile, "\tmov  \t%s, rax\n", reglist[r1]);
    free_register(r2);
    return(r1);
}

void assembly_printint(int r) {
    fprintf(OutFile, "\n");
    fprintf(OutFile, "\tmov  \trdi, _printint_format\n");
    fprintf(OutFile, "\tmov  \trsi, %s\n", reglist[r]);
    fprintf(OutFile, "\tmov  \tal, 0\n");
    fprintf(OutFile, "\tcall \tprintf\n");
    fprintf(OutFile, "\n");
    free_register(r);
}

int assembly_store_global(int r, char* identifier) {
    fprintf(OutFile, "\tmov  \t[%s], %s\n", identifier, reglist[r]);
    return r;
}

void assembly_generate_global_symbol(char* symbol) {
    fprintf(OutFile, "\tcommon\t%s 8:8\n", symbol);
}

int assembly_compare(int r1, int r2, char* compare_operator) {
    fprintf(OutFile, "\tcmp  \t%s, %s\n", reglist[r1], reglist[r2]);
    fprintf(OutFile, "\t%s \t%s\n", compare_operator, breglist[r2]);
    fprintf(OutFile, "\tand  \t%s, 255\n", reglist[r2]);
    free_register(r1);
    return r2;
}
int assembly_equals(int r1, int r2) { return assembly_compare(r1, r2, "sete"); }
int assembly_notequals(int r1, int r2) { return assembly_compare(r1, r2, "setne"); }
int assembly_lessthan(int r1, int r2) { return assembly_compare(r1, r2, "setl"); }
int assembly_greaterthan(int r1, int r2) { return assembly_compare(r1, r2, "setg"); }
int assembly_lessorequal(int r1, int r2) { return assembly_compare(r1, r2, "setle"); }
int assembly_greaterorequal(int r1, int r2) { return assembly_compare(r1, r2, "setge"); }

void assembly_label(int l) {
    fprintf(OutFile, "L%d:\n", l);
}

void assembly_jump(int l) {
    fprintf(OutFile, "\tjmp  \tL%d\n", l);
}

int assembly_if(ASTNode* n) {
    int Lfalse, Lend;

    Lfalse = label_id();

    if (n->right)
        Lend = label_id();

    assembly_ast_node(n->left, Lfalse, n->type);
    freeall_registers();

    assembly_ast_node(n->mid, -1, n->type);
    freeall_registers();

    if (n->right)
        assembly_jump(Lend);

    assembly_label(Lfalse);

    if (n->right) {
        assembly_ast_node(n->right, -1, n->type);
        freeall_registers();
        assembly_label(Lend);
    }

    return -1;
}

static char* cmplist[] = { "sete", "setne", "setl", "setg", "setle", "setge" };
int assembly_compare_and_set(NodeType parent_type, int r1, int r2) {
    if (parent_type < NODE_EQUALS || parent_type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "Bad ASTop in cgcompare_and_set()");
        exit(1);
    }
    fprintf(OutFile, "\tcmp\t%s, %s\n", reglist[r1], reglist[r2]);
    fprintf(OutFile, "\t%s\t%s\n", cmplist[parent_type - NODE_EQUALS], reglist[r2]);
    fprintf(OutFile, "\tmovzb\t%s, %s\n", reglist[r2], breglist[r2]);
    free_register(r1);
    return r2;
}


static char* invcmplist[] = { "jne", "je", "jge", "jle", "jg", "jl" };
int assembly_compare_and_jump(NodeType parent_type, int r1, int r2, int label) {
    if (parent_type < NODE_EQUALS || parent_type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "Bad ASTop in cgcompare_and_set()");
        exit(1);
    }
    fprintf(OutFile, "\tcmp\t%s, %s\n", reglist[r1], reglist[r2]);
    fprintf(OutFile, "\t%s\tL%d\n", invcmplist[parent_type - NODE_EQUALS], label);
    freeall_registers();
    return -1;
}

int assembly_while(ASTNode* n) {
    int Lstart, Lend;
    
    Lstart = label_id();
    Lend = label_id();

    assembly_label(Lstart);

    assembly_ast_node(n->left, Lend, n->type);
    freeall_registers();

    assembly_ast_node(n->right, -1, n->type);
    freeall_registers();

    assembly_jump(Lstart);
    assembly_label(Lend);

    return -1;
}

void assembly_funcpreamble(char* name) {
    fprintf(OutFile,
        "\tsection\t.text\n"
        "\tglobal\t%s\n"
        "%s:\n" 
        "\tpush \trbp\n"
        "\tmov  \trbp, rsp\n", name, name);
}

void assembly_funcpostamble() {
    fputs(
        "\tmov  \teax, 0\n"
        "\tpop  \trbp\n" 
        "\tret\n"
        , OutFile);
}

//genAST
int assembly_ast_node(ASTNode* node, int reg, NodeType parent_type) {
	
    switch (node->type) {
    case NODE_IF: return assembly_if(node);
    case NODE_WHILE: return assembly_while(node);
    case NODE_GLUE: 
        assembly_ast_node(node->left, -1, node->type);
        freeall_registers();
        assembly_ast_node(node->right, -1, node->type);
        freeall_registers();
        return -1;
    case NODE_FUNCTION:
        assembly_funcpreamble(GlobalSymbols[node->value].name);
        assembly_ast_node(node->left, -1, node->type);
        assembly_funcpostamble();
        return -1;
    }

    int leftRegister = -1, rightRegister = -1;

	if (node->left)
        leftRegister = assembly_ast_node(node->left, -1, 0);
	if (node->right)
        rightRegister = assembly_ast_node(node->right, leftRegister, 0);

    switch (node->type) {
    case NODE_ADD: return assembly_add(leftRegister, rightRegister);
    case NODE_SUBTRACT: return assembly_sub(leftRegister, rightRegister);
    case NODE_MULTIPLY: return assembly_mul(leftRegister, rightRegister);
    case NODE_DIVIDE: return assembly_div(leftRegister, rightRegister);
    
    case NODE_EQUALS: 
    case NODE_NOTEQUALS: 
    case NODE_LESS: 
    case NODE_GREATER: 
    case NODE_LESSOREQUALS: 
    case NODE_GREATEROREQUALS:
        if (parent_type == NODE_IF || parent_type == NODE_WHILE)
            return assembly_compare_and_jump(node->type, leftRegister, rightRegister, reg);
        else
            return assembly_compare_and_set(node->type, leftRegister, rightRegister);
    case NODE_INTLIT: return assembly_load_int(node->value);
    case NODE_IDENTIFIER: return assembly_load_global(GlobalSymbols[node->value].name);
    case NODE_LVALUE_IDENTIFIER: return assembly_store_global(reg, GlobalSymbols[node->value].name);
    case NODE_ASSIGN: return rightRegister;
    case NODE_PRINT:
        assembly_printint(leftRegister);
        freeall_registers();
        return -1;
    default:
        fprintf(stderr, "[CG] Unknown AST operator %d\n", node->type);
        exit(1);
    }
}