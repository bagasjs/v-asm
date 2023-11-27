#include "v-asm.h"
#include <stdio.h>

VASM_Error vasm_translate_inst_mov(VASM_ByteArray *result, const VASM_Inst *inst)
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
        assert(0 && "TODO: Not implemented");
    }

    return VASM_OK;
}

VASM_Error vasm_gen_x86_64(VASM_ByteArray *result, const VASM_Module *module)
{
    assert(result && "Expecting `result` not null for vasm_generate_x86_64()");
    assert(module && "Expecting `module` not null for vasm_generate_x86_64()");

    VASM_Error err = VASM_OK;
    for(size_t i = 0; i < module->program.count; ++i) {
        const VASM_Inst *inst = &module->program.data[i];
        switch(inst->type) {
            case VASM_INST_MOV:
                {
                    err = vasm_translate_inst_mov(result, inst);
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
