#include "v-asm.h"
#include <stdlib.h>
#include <string.h>

void vasm_bytearray_push(VASM_ByteArray *arr, uint8_t byte)
{
    if(arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity + 8;
        if(new_capacity == 0) new_capacity = 32;
        arr->data = realloc(arr->data, new_capacity);
        arr->capacity = new_capacity;
    }
    arr->data[arr->count++] = byte;
}

void vasm_bytearray_push_many(VASM_ByteArray *arr, const uint8_t *bytes_data, size_t nbytes)
{
    if(arr->count + nbytes >= arr->capacity) {
        size_t new_capacity = arr->capacity + nbytes + 8;
        arr->data = realloc(arr->data, new_capacity);
        arr->capacity = new_capacity;
    }

    memcpy(arr->data + arr->count, bytes_data, nbytes * sizeof(*arr->data));
    arr->count += nbytes;
}

bool vasm_bytearray_save(const VASM_ByteArray *arr, const char *file_path)
{
    assert(arr && "Expecting `arr` not null in vasm_bytearray_save()");
    assert(file_path && "Expecting `file_path` not null in vasm_bytearray_save()");

    FILE *f = fopen(file_path, "wb");
    if(!f) return false;

    fwrite(arr->data, 1, arr->count, f);
    fclose(f);

    return true;
}

void vasm_module_push(VASM_Module *module, VASM_Inst inst)
{
    if(module->program.count >= module->program.capacity) {
        size_t new_capacity = module->program.capacity + 8;
        if(new_capacity == 0) new_capacity = 32;
        module->program.data = realloc(module->program.data, (new_capacity * sizeof(*module->program.data)));
        module->program.capacity = new_capacity;
    }
    module->program.data[module->program.count++] = inst;
}

void vasm_module_emplace(VASM_Module *module, VASM_Inst_Type type, bool imm, uint16_t op1, uint64_t op2)
{
    if(module->program.count >= module->program.capacity) {
        size_t new_capacity = module->program.capacity + 8;
        if(new_capacity == 0) new_capacity = 32;
        module->program.data = realloc(module->program.data, (new_capacity * sizeof(*module->program.data)));
        module->program.capacity = new_capacity;
    }
    module->program.data[module->program.count].type = type;
    module->program.data[module->program.count].imm = imm;
    module->program.data[module->program.count].op1 = op1;
    module->program.data[module->program.count].op2 = op2;
    module->program.count += 1;
}

static VASM_Register_Info registers_info[VASM_COUNT_REGISTERS] = {
    [VASM_R0] = { .code = VASM_R0, .name = "VASM_R0", .size = 8, }, 
    [VASM_R1] = { .code = VASM_R1, .name = "VASM_R1", .size = 8, }, 
    [VASM_R2] = { .code = VASM_R2, .name = "VASM_R2", .size = 8, }, 
    [VASM_R3] = { .code = VASM_R3, .name = "VASM_R3", .size = 8, }, 
    [VASM_R4] = { .code = VASM_R4, .name = "VASM_R4", .size = 8, }, 
    [VASM_R5] = { .code = VASM_R5, .name = "VASM_R5", .size = 8, }, 
    [VASM_R6] = { .code = VASM_R6, .name = "VASM_R6", .size = 8, }, 
    [VASM_R7] = { .code = VASM_R7, .name = "VASM_R7", .size = 8, },

    [VASM_R8] = { .code = VASM_R8, .name = "VASM_R8", .size = 8, }, 
    [VASM_R9] = { .code = VASM_R9, .name = "VASM_R9", .size = 8, }, 
    [VASM_RA] = { .code = VASM_RA, .name = "VASM_RA", .size = 8, }, 
    [VASM_RB] = { .code = VASM_RB, .name = "VASM_RB", .size = 8, }, 
    [VASM_RC] = { .code = VASM_RC, .name = "VASM_RC", .size = 8, }, 
    [VASM_RD] = { .code = VASM_RD, .name = "VASM_RD", .size = 8, }, 
    [VASM_RE] = { .code = VASM_RE, .name = "VASM_RE", .size = 8, }, 
    [VASM_RF] = { .code = VASM_RF, .name = "VASM_RF", .size = 8, },

    [VASM_D0] = { .code = VASM_D0, .name = "VASM_D0", .size = 4, }, 
    [VASM_D1] = { .code = VASM_D1, .name = "VASM_D1", .size = 4, }, 
    [VASM_D2] = { .code = VASM_D2, .name = "VASM_D2", .size = 4, }, 
    [VASM_D3] = { .code = VASM_D3, .name = "VASM_D3", .size = 4, }, 
    [VASM_D4] = { .code = VASM_D4, .name = "VASM_D4", .size = 4, }, 
    [VASM_D5] = { .code = VASM_D5, .name = "VASM_D5", .size = 4, }, 
    [VASM_D6] = { .code = VASM_D6, .name = "VASM_D6", .size = 4, }, 
    [VASM_D7] = { .code = VASM_D7, .name = "VASM_D7", .size = 4, },

    [VASM_W0] = { .code = VASM_W0, .name = "VASM_W0", .size = 2, }, 
    [VASM_W1] = { .code = VASM_W1, .name = "VASM_W1", .size = 2, }, 
    [VASM_W2] = { .code = VASM_W2, .name = "VASM_W2", .size = 2, }, 
    [VASM_W3] = { .code = VASM_W3, .name = "VASM_W3", .size = 2, }, 
    [VASM_W4] = { .code = VASM_W4, .name = "VASM_W4", .size = 2, }, 
    [VASM_W5] = { .code = VASM_W5, .name = "VASM_W5", .size = 2, }, 
    [VASM_W6] = { .code = VASM_W6, .name = "VASM_W6", .size = 2, }, 
    [VASM_W7] = { .code = VASM_W7, .name = "VASM_W7", .size = 2, },

    [VASM_L0] = { .code = VASM_L0, .name = "VASM_L0", .size = 1, }, 
    [VASM_L1] = { .code = VASM_L1, .name = "VASM_L1", .size = 1, }, 
    [VASM_L2] = { .code = VASM_L2, .name = "VASM_L2", .size = 1, }, 
    [VASM_L3] = { .code = VASM_L3, .name = "VASM_L3", .size = 1, }, 
    [VASM_L4] = { .code = VASM_L4, .name = "VASM_L4", .size = 1, }, 
    [VASM_L5] = { .code = VASM_L5, .name = "VASM_L5", .size = 1, }, 
    [VASM_L6] = { .code = VASM_L6, .name = "VASM_L6", .size = 1, }, 
    [VASM_L7] = { .code = VASM_L7, .name = "VASM_L7", .size = 1, },

    [VASM_H0] = { .code = VASM_H0, .name = "VASM_H0", .size = 1, }, 
    [VASM_H1] = { .code = VASM_H1, .name = "VASM_H1", .size = 1, }, 
    [VASM_H2] = { .code = VASM_H2, .name = "VASM_H2", .size = 1, }, 
    [VASM_H3] = { .code = VASM_H3, .name = "VASM_H3", .size = 1, }, 
    [VASM_H4] = { .code = VASM_H4, .name = "VASM_H4", .size = 1, }, 
    [VASM_H5] = { .code = VASM_H5, .name = "VASM_H5", .size = 1, }, 
    [VASM_H6] = { .code = VASM_H6, .name = "VASM_H6", .size = 1, }, 
    [VASM_H7] = { .code = VASM_H7, .name = "VASM_H7", .size = 1, },
};

static VASM_Inst_Info instructions_info[VASM_COUNT_INSTS] = {
    [VASM_INST_MOV] = { .type = VASM_INST_MOV, .name = "VASM_INST_MOV", .has_immediate_mode = true }, 
    [VASM_INST_PUSH] = { .type = VASM_INST_PUSH, .name = "VASM_INST_PUSH", .has_immediate_mode = true }, 
    [VASM_INST_POP] = { .type = VASM_INST_POP, .name = "VASM_INST_POP", .has_immediate_mode = true }, 
    [VASM_INST_LDB] = { .type = VASM_INST_LDB, .name = "VASM_INST_LDB", .has_immediate_mode = true }, 
    [VASM_INST_LDW] = { .type = VASM_INST_LDW, .name = "VASM_INST_LDW", .has_immediate_mode = true }, 
    [VASM_INST_LDD] = { .type = VASM_INST_LDD, .name = "VASM_INST_LDD", .has_immediate_mode = true }, 
    [VASM_INST_LDQ] = { .type = VASM_INST_LDQ, .name = "VASM_INST_LDQ", .has_immediate_mode = true }, 

    [VASM_INST_STB] = { .type = VASM_INST_STB, .name = "VASM_INST_STB", .has_immediate_mode = true }, 
    [VASM_INST_STW] = { .type = VASM_INST_STW, .name = "VASM_INST_STW", .has_immediate_mode = true }, 
    [VASM_INST_STD] = { .type = VASM_INST_STD, .name = "VASM_INST_STD", .has_immediate_mode = true }, 
    [VASM_INST_STQ] = { .type = VASM_INST_STQ, .name = "VASM_INST_STQ", .has_immediate_mode = true }, 

    [VASM_INST_ADD] = { .type = VASM_INST_ADD, .name = "VASM_INST_ADD", .has_immediate_mode = true }, 
    [VASM_INST_SUB] = { .type = VASM_INST_SUB, .name = "VASM_INST_SUB", .has_immediate_mode = true }, 
    [VASM_INST_MUL] = { .type = VASM_INST_MUL, .name = "VASM_INST_MUL", .has_immediate_mode = true }, 
    [VASM_INST_DIV] = { .type = VASM_INST_DIV, .name = "VASM_INST_DIV", .has_immediate_mode = true }, 
    [VASM_INST_IMUL] = { .type = VASM_INST_IMUL, .name = "VASM_INST_IMUL", .has_immediate_mode = true }, 
    [VASM_INST_IDIV] = { .type = VASM_INST_IDIV, .name = "VASM_INST_IDIV", .has_immediate_mode = true }, 

    [VASM_INST_OR] = { .type = VASM_INST_OR, .name = "VASM_INST_OR", .has_immediate_mode = true }, 
    [VASM_INST_AND] = { .type = VASM_INST_AND, .name = "VASM_INST_AND", .has_immediate_mode = true }, 
    [VASM_INST_NOT] = { .type = VASM_INST_NOT, .name = "VASM_INST_NOT", .has_immediate_mode = true }, 
    [VASM_INST_SHL] = { .type = VASM_INST_SHL, .name = "VASM_INST_SHL", .has_immediate_mode = true }, 
    [VASM_INST_SHR] = { .type = VASM_INST_SHR, .name = "VASM_INST_SHR", .has_immediate_mode = true },

    [VASM_INST_CMP] = { .type = VASM_INST_CMP, .name = "VASM_INST_CMP", .has_immediate_mode = true }, 
    [VASM_INST_JE] = { .type = VASM_INST_JE, .name = "VASM_INST_JE", .has_immediate_mode = true }, 
    [VASM_INST_JN] = { .type = VASM_INST_JN, .name = "VASM_INST_JN", .has_immediate_mode = true }, 
    [VASM_INST_JL] = { .type = VASM_INST_JL, .name = "VASM_INST_JL", .has_immediate_mode = true }, 
    [VASM_INST_JLE] = { .type = VASM_INST_JLE, .name = "VASM_INST_JLE", .has_immediate_mode = true }, 
    [VASM_INST_JG] = { .type = VASM_INST_JG, .name = "VASM_INST_JG", .has_immediate_mode = true }, 
    [VASM_INST_JGE] = { .type = VASM_INST_JGE, .name = "VASM_INST_JGE", .has_immediate_mode = true },

    [VASM_INST_CALL] = { .type = VASM_INST_CALL, .name = "VASM_INST_CALL", .has_immediate_mode = true }, 
    [VASM_INST_RET] = { .type = VASM_INST_RET, .name = "VASM_INST_RET", .has_immediate_mode = true }, 
    [VASM_INST_SYSCALL] = { .type = VASM_INST_SYSCALL, .name = "VASM_INST_SYSCALL", .has_immediate_mode = true },
};


VASM_Inst_Info vasm_get_inst_info(VASM_Inst_Type type)
{
    return instructions_info[type];
}

VASM_Register_Info vasm_get_register_info(VASM_Register_Code code)
{
    return registers_info[code];
}

void vasm_dump_inst(const VASM_Inst *inst)
{
    VASM_Inst_Info iinfo = vasm_get_inst_info(inst->type);
    if(inst->op1 != 0) {
        VASM_Register_Info rinfo = vasm_get_register_info(inst->op1);
        printf("%s %s", iinfo.name, rinfo.name);
        if(inst->imm) {
            printf(", %zu\n", inst->op2);
        } else if(inst->op2 != 0) {
            rinfo = vasm_get_register_info(inst->op2);
            printf(", %s\n", rinfo.name);
        }
    } else {
        printf("%s\n", iinfo.name);
    }
}
