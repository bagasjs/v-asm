#include "v-asm.h"
#include <stdio.h>

#include <elf.h>
#include <libelf.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SYS_EXIT 60

int main(void)
{
    VASM_Module mod = {0};
    VASM_ByteArray result = {0};
    vasm_module_emplace(&mod, VASM_INST_MOV, true, VASM_R0, SYS_EXIT);
    vasm_module_emplace(&mod, VASM_INST_MOV, true, VASM_R1, 0);
    vasm_module_emplace(&mod, VASM_INST_SYSCALL, true, 0, 0);

    for(size_t i = 0; i < mod.program.count; ++i) {
        vasm_dump_inst(&mod.program.data[i]);
    }

    if(vasm_gen_x86_64(&result, &mod) != VASM_OK) {
        fprintf(stderr, "Failed to generate x86_64 machine code\n");
        return -1;
    }

    vasm_bytearray_save(&result, "result.bin");

    return 0;
}

