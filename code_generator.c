#include "code_generator.h"

static int localOffset;
static int stackOffset;

static int freereg[4];
static char* reglist[4] = { "r8", "r9", "r10", "r11" };
static char* breglist[4] = { "r8b", "r9b", "r10b", "r11b" };
static char* dreglist[4] = { "r8d", "r9d", "r10d", "r11d" };

void cgresetlocals(void) {
    localOffset = 0;
}

int gengetlocaloffset(int type, int isparam) {
    localOffset += (get_type_size(type) > 4) ? get_type_size(type) : 4;
    return (-localOffset);
}

void setOutputFile(FILE* output) {
    OutFile = output;
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
    fputs(
        "extern printf\n"
        "\n"
        "section .data\n"
        "\t_format db '%d', 0xA, 0\n"
        "\t_format2 db '%c', 0\n"
        "\n",
        OutFile);
}

void assembly_postamble() {
    //Nothing to do
}

int assembly_load_int(int value, PrimitiveType type) {
    int r = alloc_register();
    fprintf(OutFile, "\tmov  \t%s, %d\t\t; %s = %d\n", reglist[r], value, reglist[r], value);
    return r;
}

int assembly_add(int r1, int r2) {
    fprintf(OutFile, "\tadd  \t%s, %s\t\t; %s = %s + %s\n", reglist[r2], reglist[r1], reglist[r2], reglist[r2], reglist[r1]);
    free_register(r1);
    return r2;
}

int assembly_sub(int r1, int r2) {
    fprintf(OutFile, "\tsub  \t%s, %s\t\t; %s = %s - %s\n", reglist[r1], reglist[r2], reglist[r1], reglist[r1], reglist[r2]);
    free_register(r2);
    return r1;
}

int assembly_mul(int r1, int r2) {
    fprintf(OutFile, "\timul  \t%s, %s\t\t; %s = %s * %s\n", reglist[r2], reglist[r1], reglist[r2], reglist[r2], reglist[r1]);
    free_register(r1);
    return(r2);
}

int assembly_div(int r1, int r2) {
    fprintf(OutFile, "\tmov  \trax, %s\t\t; %s = %s / %s\n", reglist[r1], reglist[r1], reglist[r1], reglist[r2]);
    fprintf(OutFile, "\txor  \trdx, rdx\n");
    fprintf(OutFile, "\tidiv \t%s\n", reglist[r2]);
    fprintf(OutFile, "\tmov  \t%s, rax\n", reglist[r1]);
    free_register(r2);
    return(r1);
}

void assembly_printchar(int r) {
    fprintf(OutFile, "\n");
    fprintf(OutFile, "\tmov  \trdi, _format2\t; printf2 %s\n", reglist[r]);
    fprintf(OutFile, "\tmov  \trsi, %s\n", reglist[r]);
    fprintf(OutFile, "\tmov  \tal, 0\n");
    fprintf(OutFile, "\tcall \tprintf\n");
    fprintf(OutFile, "\n");
    free_register(r);
}

void assembly_printint(int r) {
    fprintf(OutFile, "\n");
    fprintf(OutFile, "\tmov  \trdi, _format\t; printf %s\n", reglist[r]);
    fprintf(OutFile, "\tmov  \trsi, %s\n", reglist[r]);
    fprintf(OutFile, "\tmov  \tal, 0\n");
    fprintf(OutFile, "\tcall \tprintf\n");
    fprintf(OutFile, "\n");
    free_register(r);
}

int assembly_load_global(int id, OperationType op) {
    int r = alloc_register();
    Symbol sym = SymbolTable[id];
    switch (sym.type)
    {
    case PRIM_CHAR: 
        if (op == NODE_PREINC) fprintf(OutFile, "\tinc  \tbyte [%s]\n", sym.name);
        if (op == NODE_PREDEC) fprintf(OutFile, "\tdec  \tbyte [%s]\n", sym.name);
        fprintf(OutFile, "\tmovzx\t%s, byte [%s]\n", reglist[r], sym.name); 
        if (op == NODE_POSTINC) fprintf(OutFile, "\tinc  \tbyte [%s]\n", sym.name);
        if (op == NODE_POSTDEC) fprintf(OutFile, "\tdec  \tbyte [%s]\n", sym.name);

        break;
    case PRIM_INT:  
        if (op == NODE_PREINC) fprintf(OutFile, "\tinc  \tdword [%s]\n", sym.name);
        if (op == NODE_PREDEC) fprintf(OutFile, "\tdec  \tdword [%s]\n", sym.name);
        fprintf(OutFile, "\tmovsx \t%s, word [%s]\n", reglist[r], sym.name);
        fprintf(OutFile, "\tmovsxd\t%s, %s\n", reglist[r], dreglist[r]);
        if (op == NODE_POSTINC) fprintf(OutFile, "\tinc  \tdword [%s]\n", sym.name);
        if (op == NODE_POSTDEC) fprintf(OutFile, "\tdec  \tdword [%s]\n", sym.name);
        break;
    case PRIM_LONG:
    case PRIM_CHARPTR:
    case PRIM_INTPTR:
    case PRIM_LONGPTR:
        if (op == NODE_PREINC) fprintf(OutFile, "\tinc  \tqword [%s]\n", sym.name);
        if (op == NODE_PREDEC) fprintf(OutFile, "\tdec  \tqword [%s]\n", sym.name);
        fprintf(OutFile, "\tmov  \t%s, [%s]\n", reglist[r], sym.name);
        if (op == NODE_POSTINC) fprintf(OutFile, "\tinc  \tqword [%s]\n", sym.name);
        if (op == NODE_POSTDEC) fprintf(OutFile, "\tdec  \tqword [%s]\n", sym.name);
        break;
    default:
        fprintf(stderr, "Bad type in assembly_load_global()");
        exit(1);
        break;
    }

    return r;
}

int assembly_load_local(int id, OperationType op) {
    int r = alloc_register();
    Symbol sym = SymbolTable[id];

    switch (sym.type)
    {
    case PRIM_CHAR:
        if (op == NODE_PREINC) fprintf(OutFile, "\tinc  \tbyte [rbp + %d]\n", sym.posn);
        if (op == NODE_PREDEC) fprintf(OutFile, "\tdec  \tbyte [rbp + %d]\n", sym.posn);
        fprintf(OutFile, "\tmovzx\t%s, byte [rbp + %d]\n", reglist[r], sym.posn);
        if (op == NODE_POSTINC) fprintf(OutFile, "\tinc  \tbyte [rbp + %d]\n", sym.posn);
        if (op == NODE_POSTDEC) fprintf(OutFile, "\tdec  \tbyte [rbp + %d]\n", sym.posn);

        break;
    case PRIM_INT:
        if (op == NODE_PREINC) fprintf(OutFile, "\tinc  \tdword [rbp + %d]\n", sym.posn);
        if (op == NODE_PREDEC) fprintf(OutFile, "\tdec  \tdword [rbp + %d]\n", sym.posn);
        fprintf(OutFile, "\tmovsx \t%s, word [rbp + %d]\n", reglist[r], sym.posn);
        fprintf(OutFile, "\tmovsxd\t%s, %s\n", reglist[r], dreglist[r]);
        if (op == NODE_POSTINC) fprintf(OutFile, "\tinc  \tdword [rbp + %d]\n", sym.posn);
        if (op == NODE_POSTDEC) fprintf(OutFile, "\tdec  \tdword [rbp + %d]\n", sym.posn);
        break;
    case PRIM_LONG:
    case PRIM_CHARPTR:
    case PRIM_INTPTR:
    case PRIM_LONGPTR:
        if (op == NODE_PREINC) fprintf(OutFile, "\tinc  \tqword [rbp + %d]\n", sym.posn);
        if (op == NODE_PREDEC) fprintf(OutFile, "\tdec  \tqword [rbp + %d]\n", sym.posn);
        fprintf(OutFile, "\tmov  \t%s, [rbp + %d]\n", reglist[r], sym.posn);
        if (op == NODE_POSTINC) fprintf(OutFile, "\tinc  \tqword [rbp + %d]\n", sym.posn);
        if (op == NODE_POSTDEC) fprintf(OutFile, "\tdec  \tqword [rbp + %d]\n", sym.posn);
        break;
    default:
        fprintf(stderr, "Bad type in assembly_load_global()");
        exit(1);
        break;
    }

    return r;
}

//saves register value into variable
int assembly_store_global(int r, int id) {
    Symbol sym = SymbolTable[id];

    switch (sym.type)
    {
    case PRIM_CHAR: fprintf(OutFile, "\tmov  \t[%s], %s\n", sym.name, breglist[r]); break;
    case PRIM_INT:  fprintf(OutFile, "\tmov  \t[%s], %s\n", sym.name, dreglist[r]); break;
    case PRIM_LONG:
    case PRIM_CHARPTR:
    case PRIM_INTPTR:
    case PRIM_LONGPTR:
        fprintf(OutFile, "\tmov  \t[%s], %s\n", sym.name, reglist[r]); break;
    default:
        fprintf(stderr, "Bad type in assembly_store_global()");
        exit(1);
        break;
    }
    return r;
}

int assembly_store_local(int r, int id) {
    Symbol sym = SymbolTable[id];

    switch (sym.type)
    {
    case PRIM_CHAR: fprintf(OutFile, "\tmov  \tbyte [rbp + %d], %s\n", sym.posn, breglist[r]); break;
    case PRIM_INT:  fprintf(OutFile, "\tmov  \tdword [rbp + %d], %s\n", sym.posn, dreglist[r]); break;
    case PRIM_LONG:
    case PRIM_CHARPTR:
    case PRIM_INTPTR:
    case PRIM_LONGPTR:
        fprintf(OutFile, "\tmov  \tqword [rbp + %d], %s\n", sym.posn, reglist[r]); break;
    default:
        fprintf(stderr, "Bad type in assembly_store_local()");
        exit(1);
        break;
    }
    return r;
}

int get_type_size(PrimitiveType type) {
    switch (type)
    {
    case PRIM_NONE: return 0;
    case PRIM_VOID: return 0;
    case PRIM_CHAR: return 1;
    case PRIM_INT:  return 4;
    case PRIM_LONG: return 8;
    case PRIM_VOIDPTR:
    case PRIM_CHARPTR:
    case PRIM_INTPTR:
    case PRIM_LONGPTR:
        return 8;
    default:
        fprintf(stderr, "Bad type in cgprimsize()");
        exit(1);
    }
}

void assembly_generate_global_symbol(int id) {
    Symbol sym = SymbolTable[id];
    if (sym.stype == STRU_FUNCTION)
        return;
    int size = get_type_size(sym.type);
    fprintf(OutFile, "section .bss\n");
    fprintf(OutFile, "\t%s: resb %d\n\n", sym.name, get_type_size(sym.type));
}

//int assembly_compare(int r1, int r2, char* compare_operator, char* symbol) {
//    fprintf(OutFile, "\tcmp  \t%s, %s\t\t; %s %s %s\n", reglist[r1], reglist[r2], reglist[r1], symbol, reglist[r2]);
//    fprintf(OutFile, "\t%s \t%s\n", compare_operator, breglist[r2]);
//    fprintf(OutFile, "\tand  \t%s, 255\n", reglist[r2]);
//    free_register(r1);
//    return r2;
//}
//int assembly_equals(int r1, int r2) { return assembly_compare(r1, r2, "sete", "=="); }
//int assembly_notequals(int r1, int r2) { return assembly_compare(r1, r2, "setne", "!="); }
//int assembly_lessthan(int r1, int r2) { return assembly_compare(r1, r2, "setl", "<"); }
//int assembly_greaterthan(int r1, int r2) { return assembly_compare(r1, r2, "setg", ">"); }
//int assembly_lessorequal(int r1, int r2) { return assembly_compare(r1, r2, "setle", "<="); }
//int assembly_greaterorequal(int r1, int r2) { return assembly_compare(r1, r2, "setge", ">="); }

void assembly_label(int l) {
    fprintf(OutFile, "L%d:\n", l);
}

void assembly_jump(int l) {
    fprintf(OutFile, "\tjmp  \tL%d\t\t; goto L%d\n", l, l);
}

int assembly_if(struct ASTNode* n) {
    int Lfalse, Lend;

    Lfalse = label_id();

    if (n->right)
        Lend = label_id();

    assembly_ast_node(n->left, Lfalse, n->operation);
    freeall_registers();

    assembly_ast_node(n->mid, -1, n->operation);
    freeall_registers();

    if (n->right)
        assembly_jump(Lend);

    assembly_label(Lfalse);

    if (n->right) {
        assembly_ast_node(n->right, -1, n->operation);
        freeall_registers();
        assembly_label(Lend);
    }

    return -1;
}

static char* cmpsymbols[] = { "==", "!=", "<", ">", "<=", ">=" };
static char* cmplist[] = { "sete", "setne", "setl", "setg", "setle", "setge" };
int assembly_compare_and_set(OperationType parent_type, int r1, int r2) {
    if (parent_type < NODE_EQUALS || parent_type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "Bad ASTop in cgcompare_and_set()");
        exit(1);
    }
    int index = parent_type - NODE_EQUALS;
    fprintf(OutFile, "\tcmp\t%s, %s\t\t; %s %s %s\n", reglist[r1], reglist[r2], reglist[r1], cmpsymbols[index], reglist[r2]);
    fprintf(OutFile, "\t%s\t%s\n", cmplist[index], breglist[r2]);
    fprintf(OutFile, "\tmovzx\t%s, %s\n", reglist[r2], breglist[r2]);
    free_register(r1);
    return r2;
}

static char* invcmpsymbols[] = { "!=", "==", ">=", "<=", ">", "<" };
static char* invcmplist[] = { "jne", "je", "jge", "jle", "jg", "jl" };
int assembly_compare_and_jump(OperationType parent_type, int r1, int r2, int label) {
    if (parent_type < NODE_EQUALS || parent_type > NODE_GREATEROREQUALS) {
        fprintf(stderr, "Bad ASTop in cgcompare_and_set()");
        exit(1);
    }
    int index = parent_type - NODE_EQUALS;
    fprintf(OutFile, "\tcmp\t%s, %s\t\t; if %s %s %s\n", reglist[r1], reglist[r2], reglist[r1], invcmpsymbols[index], reglist[r2]);
    fprintf(OutFile, "\t%s\tL%d\t\t; goto L%d\n", invcmplist[index], label, label);
    freeall_registers();
    return -1;
}

int assembly_while(struct ASTNode* n) {
    int Lstart, Lend;

    Lstart = label_id();
    Lend = label_id();

    assembly_label(Lstart);

    assembly_ast_node(n->left, Lend, n->operation);
    freeall_registers();

    assembly_ast_node(n->right, -1, n->operation);
    freeall_registers();

    assembly_jump(Lstart);
    assembly_label(Lend);

    return -1;
}

void assembly_funcpreamble(int id) {
    char* name = SymbolTable[id].name;

    // Align the stack pointer to be a multiple of 16 less than its previous value
    stackOffset = (localOffset + 15) & ~15;

    fprintf(OutFile,
        "section\t.text\n"
        "global\t%s\n"
        "%s:\n" 
        "\tpush \trbp\n"
        "\tmov  \trbp, rsp\n"
        "\tadd  \trsp, %d\n\n"
        , name, name, -stackOffset);
}

void assembly_funcpostamble(int id) {
    fputs("\n", OutFile);
    assembly_label(SymbolTable[id].endlabel);
    fprintf(OutFile,
        "\tadd  \trsp, %d\n"
        "\tpop  \trbp\n"
        "\tret\n"
        , stackOffset);
}

int assembly_widen(int r, PrimitiveType oldType, PrimitiveType newType) {
    //Nothing to do
    return r;
}

void assembly_return(int r, int id) {
    switch (SymbolTable[id].type)
    {
    case PRIM_CHAR:
        fprintf(OutFile, "\tmovzx\teax, %s\n", breglist[r]);
        break;
    case PRIM_INT:
        fprintf(OutFile, "\tmov  \teax, %s\n", dreglist[r]);
        break;
    case PRIM_LONG:
        fprintf(OutFile, "\tmov  \trax, %s\n", reglist[r]);
        break;
    default:
        fprintf(stderr, "Bad ASTop in cgcompare_and_set()");
        exit(1);
    }
    assembly_jump(SymbolTable[id].endlabel);
}

int assembly_function_call(int r, int id) {
    int outr = alloc_register();
    fprintf(OutFile, "\tmov  \trdi, %s\n", reglist[r]);
    fprintf(OutFile, "\tcall \t%s\n", SymbolTable[id].name);
    fprintf(OutFile, "\tmov  \t%s, rax\n", reglist[outr]);
    free_register(r);
    return outr;
}

int assembly_address(int id) {
    int r = alloc_register();
    //fprintf(OutFile, "\tmov  \t%s, %s\n", reglist[r], SymbolTable[id].name);
    fprintf(OutFile, "\tlea  \t%s, [%s]\n", reglist[r], SymbolTable[id].name);
    return r;
}

int assembly_dereference(int r, PrimitiveType type) {
    switch (type)
    {
    case PRIM_CHARPTR:  
        fprintf(OutFile, "\tmovzx\t%s, byte [%s]\n", reglist[r], reglist[r]); break;
    case PRIM_INTPTR:   
        fprintf(OutFile, "\tmovzx\t%s, word [%s]\n", reglist[r], reglist[r]); break;
    case PRIM_LONGPTR:  fprintf(OutFile, "\tmov  \t%s, [%s]\n", reglist[r], reglist[r]); break;
    }
    return r;
}

int assembly_shift_left(int r, int val) {
    fprintf(OutFile, "\tsal  \t%s, %d\n", reglist[r], val);
    return r;
}

//stores a value through a dereferenced pointer
int assembly_store_dereference(int r1, int r2, PrimitiveType type) {
    switch (type)
    {
    case PRIM_CHAR:
        fprintf(OutFile, "\tmov  \t[%s], byte %s\n", reglist[r2], breglist[r1]);
        break;
    case PRIM_INT:
        fprintf(OutFile, "\tmov  \t[%s], %s\n", reglist[r2], reglist[r1]);
        break;
    case PRIM_LONG:
        fprintf(OutFile, "\tmov  \t[%s], %s\n", reglist[r2], reglist[r1]);
        break;
    default:
        fprintf(stderr, "Can't cgstoderef on type");
        exit(1);
    }
    return r1;
}

void assembly_global_string(int l, char* text) {
    char* cptr;
    assembly_label(l);
    for (int i = 0; text[i] != '\0'; ++i) {
        fprintf(OutFile, "\tdb   \t%d\n", text[i]);
    }
    fprintf(OutFile, "\tdb    \t0\n");
}

int assembly_generate_global_string(char* text) {
    //int l = label_id();
    //assembly_global_string(l, text);
    int l = label_id();
    fprintf(OutFile, "section .data\n");
    //fprintf(OutFile, "\tLC%d db '%s', 0\n\n", l, text);
    fprintf(OutFile, "\tLC%d db ", l);
    char* cptr;
    for (cptr = text; *cptr; cptr++) {
        if (*cptr == '\n')
            fprintf(OutFile, "0xA, ");
        else 
            fprintf(OutFile, "'%c', ", *cptr);
    }
    fprintf(OutFile, "0\n");

    return l;
}

int assembly_load_global_string(int id) {
    int r = alloc_register();
    //fprintf(OutFile, "\tmov  \t%s, L%d\n", reglist[r], id);
    fprintf(OutFile, "\tmov  \t%s, LC%d\n", reglist[r], id);
    return r;
}

int assembly_and(int r1, int r2) {
    fprintf(OutFile, "\tand  \t%s, %s\n", reglist[r2], reglist[r1]);
    free_register(r1);
    return r2;
}

int assembly_or(int r1, int r2) {
    fprintf(OutFile, "\tor   \t%s, %s\n", reglist[r2], reglist[r1]);
    free_register(r1);
    return r2;
}

int assembly_xor(int r1, int r2) {
    fprintf(OutFile, "\txor  \t%s, %s\n", reglist[r2], reglist[r1]);
    free_register(r1);
    return r2;
}

int assembly_lshift(int r1, int r2) {
    fprintf(OutFile, "\tmov  \tcl, %s\n", breglist[r2]);
    fprintf(OutFile, "\tshl  \t%s, cl\n", reglist[r1]);
    free_register(r2);
    return r1;
}


int assembly_rshift(int r1, int r2) {
    fprintf(OutFile, "\tmov  \tcl, %s\n", breglist[r2]);
    fprintf(OutFile, "\tshr  \t%s, cl\n", reglist[r1]);
    free_register(r2);
    return r1;
}

int assembly_negate(int r) {
    fprintf(OutFile, "\tneg  \t%s\n", reglist[r]);
    return r;
}

int assembly_invert(int r) {
    fprintf(OutFile, "\tnot  \t%s\n", reglist[r]);
    return r;
}

int assembly_lognot(int r) {
    fprintf(OutFile, "\ttest \t%s, %s\n", reglist[r], reglist[r]);
    fprintf(OutFile, "\tsete \t%s\n", breglist[r]);
    fprintf(OutFile, "\tmovzx\t%s, %s\n", reglist[r], breglist[r]);
    return (r);
}

int assembly_boolean(int r, int op, int label) {
    fprintf(OutFile, "\ttest \t%s, %s\n", reglist[r], reglist[r]);
    if (op == NODE_IF || op == NODE_WHILE)
        fprintf(OutFile, "\tje   \tL%d\n", label);
    else {
        fprintf(OutFile, "\tsetnz\t%s\n", breglist[r]);
        fprintf(OutFile, "\tmovzx\t%s, byte %s\n", reglist[r], breglist[r]);
    }
    return (r);
}

//genAST
int assembly_ast_node(struct ASTNode* node, int label, OperationType parent_type) {
	
    switch (node->operation) {
    case NODE_IF: return assembly_if(node);
    case NODE_WHILE: return assembly_while(node);
    case NODE_GLUE:
        assembly_ast_node(node->left, -1, node->operation);
        freeall_registers();
        assembly_ast_node(node->right, -1, node->operation);
        freeall_registers();
        return -1;
    case NODE_FUNCTION:
        assembly_funcpreamble(node->value);
        assembly_ast_node(node->left, -1, node->operation);
        assembly_funcpostamble(node->value);
        return -1;
    }

    int leftRegister = -1, rightRegister = -1;

    if (node->left)
        leftRegister = assembly_ast_node(node->left, -1, node->operation);
    if (node->right)
        rightRegister = assembly_ast_node(node->right, leftRegister, node->operation);

    switch (node->operation) {
    case NODE_ADD: return assembly_add(leftRegister, rightRegister);
    case NODE_SUBTRACT: return assembly_sub(leftRegister, rightRegister);
    case NODE_MULTIPLY: return assembly_mul(leftRegister, rightRegister);
    case NODE_DIVIDE: return assembly_div(leftRegister, rightRegister);

    case NODE_AND: return assembly_and(leftRegister, rightRegister);
    case NODE_OR: return assembly_or(leftRegister, rightRegister);
    case NODE_XOR: return assembly_xor(leftRegister, rightRegister);
    case NODE_LSHIFT: return assembly_lshift(leftRegister, rightRegister);
    case NODE_RSHIFT: return assembly_rshift(leftRegister, rightRegister);
    case NODE_POSTINC: return assembly_load_global(node->value, node->operation);
    case NODE_POSTDEC: return assembly_load_global(node->value, node->operation);
    case NODE_PREINC: return assembly_load_global(node->left->value, node->operation);
    case NODE_PREDEC: return assembly_load_global(node->left->value, node->operation);
    case NODE_NEGATE: return assembly_negate(leftRegister, rightRegister);
    case NODE_INVERT: return assembly_invert(leftRegister, rightRegister);
    case NODE_LOGNOT: return assembly_lognot(leftRegister, rightRegister);
    case NODE_TOBOOL: return assembly_boolean(leftRegister, parent_type, label);

    case NODE_EQUALS:
    case NODE_NOTEQUALS:
    case NODE_LESS:
    case NODE_GREATER:
    case NODE_LESSOREQUALS:
    case NODE_GREATEROREQUALS:
        if (parent_type == NODE_IF || parent_type == NODE_WHILE)
            return assembly_compare_and_jump(node->operation, leftRegister, rightRegister, label);
        else
            return assembly_compare_and_set(node->operation, leftRegister, rightRegister);
    
    case NODE_INTLIT: return assembly_load_int(node->value, node->type);
    case NODE_STRINGLIT: return assembly_load_global_string(node->value);
    case NODE_IDENTIFIER: 
        if (node->isRvalue || parent_type == NODE_DEREFERENCE) {
            if (SymbolTable[node->value].class == LOCAL)
                return assembly_load_local(node->value, node->operation);
            else 
                return assembly_load_global(node->value, node->operation);
        }
        return -1;
    case NODE_ASSIGN: 
        switch (node->right->operation)
        {
        case NODE_IDENTIFIER: 
            if (SymbolTable[node->right->value].class == LOCAL)
                return assembly_store_local(leftRegister, node->right->value);
            else
                return assembly_store_global(leftRegister, node->right->value);

        case NODE_DEREFERENCE:  return assembly_store_dereference(leftRegister, rightRegister, node->right->type);
        default:
            fprintf(stderr, "Can't A_ASSIGN in genAST()");
            exit(1);
        }
        return rightRegister;
    case NODE_WIDEN: return assembly_widen(leftRegister, node->left->type, node->type);
    case NODE_RETURN: assembly_return(leftRegister, Functionid); return -1;
    case NODE_FUNCCALL: return assembly_function_call(leftRegister, node->value);
    case NODE_ADDRESS: return assembly_address(node->value);
    case NODE_DEREFERENCE: 
        if (node->isRvalue)
            return assembly_dereference(leftRegister, node->left->type);
        else
            return leftRegister;
    case NODE_SCALE:
        switch (node->value)
        {
        case 2: return assembly_shift_left(leftRegister, 1);
        case 4: return assembly_shift_left(leftRegister, 2);
        case 8: return assembly_shift_left(leftRegister, 3);
        default:
            rightRegister = assembly_load_int(node->value, PRIM_INT);
            return assembly_mul(leftRegister, rightRegister);
        }
    case NODE_PRINT:
        assembly_printint(leftRegister);
        freeall_registers();
    return -1; 
    case NODE_PRINT2:
        assembly_printchar(leftRegister);
        freeall_registers();
        return -1;
    default:
        fprintf(stderr, "[CG] Unknown AST operator %d\n", node->operation);
        exit(1);
    }
}