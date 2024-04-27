#include "v-asm.h"

#include <libelf.h>

VASM_Error vasm_gen_elf_x86_64(VASM_ByteArray *result, const VASM_Module *module)
{
    VASM_Error err = VASM_OK;
    err = vasm_gen_x86_64(result, module);
    if(err != VASM_OK) return err;


    return err;
}
