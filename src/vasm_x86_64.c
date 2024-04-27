#include "v-asm.h"
#include <stdio.h>

struct x86_64_register_info {
    bool only_64bit;
    bool available;
    uint8_t bytesize;
    VASM_Register_Code code;
    uint8_t native;
};

static const struct x86_64_register_info register_map_x86_64[VASM_COUNT_REGISTERS] = {
    [VASM_R0] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R0, .native = 0, },
    [VASM_R1] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R1, .native = 1, },
    [VASM_R2] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R2, .native = 2, },
    [VASM_R3] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R3, .native = 3, },
    [VASM_R4] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R4, .native = 4, },
    [VASM_R5] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R5, .native = 5, },
    [VASM_R6] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R6, .native = 6, },
    [VASM_R7] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R7, .native = 7, },
    [VASM_R8] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R8, .native = 0, },
    [VASM_R9] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_R9, .native = 1, },
    [VASM_RA] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_RA, .native = 2, },
    [VASM_RB] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_RB, .native = 3, },
    [VASM_RC] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_RC, .native = 4, },
    [VASM_RD] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_RD, .native = 5, },
    [VASM_RE] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_RE, .native = 6, },
    [VASM_RF] = { .only_64bit = true, .available = true, .bytesize = 8, .code = VASM_RF, .native = 7, },

    [VASM_D0] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D0, .native = 0, },
    [VASM_D1] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D1, .native = 1, },
    [VASM_D2] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D2, .native = 2, },
    [VASM_D3] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D3, .native = 3, },
    [VASM_D4] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D4, .native = 4, },
    [VASM_D5] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D5, .native = 5, },
    [VASM_D6] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D6, .native = 6, },
    [VASM_D7] = { .only_64bit = false, .available = true, .bytesize = 4, .code = VASM_D7, .native = 7, },

    [VASM_W0] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W0, .native = 0, },
    [VASM_W1] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W1, .native = 1, },
    [VASM_W2] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W2, .native = 2, },
    [VASM_W3] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W3, .native = 3, },
    [VASM_W4] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W4, .native = 4, },
    [VASM_W5] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W5, .native = 5, },
    [VASM_W6] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W6, .native = 6, },
    [VASM_W7] = { .only_64bit = false, .available = true, .bytesize = 2, .code = VASM_W7, .native = 7, },

    [VASM_H0] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H0, .native = 0, },
    [VASM_H1] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H1, .native = 1, },
    [VASM_H2] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H2, .native = 2, },
    [VASM_H3] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H3, .native = 3, },
    [VASM_H4] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H4, .native = 4, },
    [VASM_H5] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H5, .native = 5, },
    [VASM_H6] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H6, .native = 6, },
    [VASM_H7] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_H7, .native = 7, },

    [VASM_L0] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L0, .native = 0, },
    [VASM_L1] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L1, .native = 1, },
    [VASM_L2] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L2, .native = 2, },
    [VASM_L3] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L3, .native = 3, },
    [VASM_L4] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L4, .native = 4, },
    [VASM_L5] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L5, .native = 5, },
    [VASM_L6] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L6, .native = 6, },
    [VASM_L7] = { .only_64bit = false, .available = true, .bytesize = 1, .code = VASM_L7, .native = 7, },
};

static VASM_Error vasm_translate_inst_mov(VASM_ByteArray *result, const VASM_Inst *inst)
{
    assert(inst->type == VASM_INST_MOV && "Invalid instruction type passed into vasm_translate_inst_mov()");
    if(!(VASM_R0 <= inst->op1 && inst->op1 <= VASM_RF)) {
        return VASM_INVALID_REGISTER_ERROR;
    }

    uint8_t opcode = 0xB8 + inst->op1;
    vasm_bytearray_push(result, opcode);

    if(inst->imm) {
        uint8_t data[4] = { 
            (inst->op2 >> (8*0)) & 0xFF,
            (inst->op2 >> (8*1)) & 0xFF,
            (inst->op2 >> (8*2)) & 0xFF,
            (inst->op2 >> (8*3)) & 0xFF,
        };
        vasm_bytearray_push_many(result, data, 4);
    } else {
        const uint8_t opcode = 0x89; // mov opcode
        uint8_t prefix = 0x48; // register prefix for 64 bit 
        uint8_t operands = 3;
        operands = (operands << 6) | (register_map_x86_64[inst->op1].native << 3) | register_map_x86_64[inst->op2].native;

        uint8_t data[3] = {
            prefix,
            opcode,
            operands, 
        };
        vasm_bytearray_push_many(result, data, 3);
    }

    return VASM_OK;
}

static VASM_Error vasm_translate_inst_cmp(VASM_ByteArray *result, const VASM_Inst *inst)
{
    (void)result;
    (void)inst;
    assert(inst->type == VASM_INST_CMP && "Invalid instruction type passed into vasm_translate_inst_cmp()");
    return VASM_OK;
}

VASM_Error vasm_gen_x86_64(VASM_ByteArray *result, const VASM_Module *module)
{
    assert(result && "Expecting `result` not null for vasm_gen_x86_64()");
    assert(module && "Expecting `module` not null for vasm_gen_x86_64()");

    VASM_Error err = VASM_OK;
    for(size_t i = 0; i < module->program.count; ++i) {
        const VASM_Inst *inst = &module->program.data[i];
        switch(inst->type) {
            case VASM_INST_MOV:
                {
                    err = vasm_translate_inst_mov(result, inst);
                } break;
            case VASM_INST_CMP:
                {
                    err = vasm_translate_inst_cmp(result, inst);
                } break;
            case VASM_INST_SYSCALL:
                {
                    uint8_t data[2] = { 0x0F, 0x05 };
                    vasm_bytearray_push_many(result, data, 2);
                } break;
            default:
                {
                } break;
        }

        if(err != VASM_OK) 
            return err;
    }

    return err;
}
