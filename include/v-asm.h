#ifndef VASM_H_
#define VASM_H_

#include <stddef.h> // size_t sizeof()
#include <stdint.h> // uintX_t, intX_t
#include <stdbool.h> // bool, true, false
#include <assert.h> // assert()
#include <stdio.h> // FILE

typedef enum VASM_Error {
    VASM_OK = 0,
    VASM_GENERIC_ERROR,
    VASM_INVALID_REGISTER_ERROR,
} VASM_Error;

typedef enum VASM_Register_Code {
    VASM_INVALID_REGISTER = 0,
    VASM_R0, VASM_R1, VASM_R2, VASM_R3, VASM_R4, VASM_R5, VASM_R6, VASM_R7,
    VASM_R8, VASM_R9, VASM_RA, VASM_RB, VASM_RC, VASM_RD, VASM_RE, VASM_RF,

    VASM_D0, VASM_D1, VASM_D2, VASM_D3, VASM_D4, VASM_D5, VASM_D6, VASM_D7,
    VASM_W0, VASM_W1, VASM_W2, VASM_W3, VASM_W4, VASM_W5, VASM_W6, VASM_W7,
    VASM_L0, VASM_L1, VASM_L2, VASM_L3, VASM_L4, VASM_L5, VASM_L6, VASM_L7,
    VASM_H0, VASM_H1, VASM_H2, VASM_H3, VASM_H4, VASM_H5, VASM_H6, VASM_H7,

    VASM_COUNT_REGISTERS,
} VASM_Register_Code;

typedef struct VASM_Register_Info {
    VASM_Register_Code code;
    const char *name;
    size_t size;
} VASM_Register_Info;

typedef enum VASM_Inst_Type {
    VASM_INST_MOV = 0, VASM_INST_PUSH, VASM_INST_POP, VASM_INST_LDB, VASM_INST_LDW, VASM_INST_LDD, VASM_INST_LDQ, 
    VASM_INST_STB, VASM_INST_STW, VASM_INST_STD, VASM_INST_STQ, 
    VASM_INST_ADD, VASM_INST_SUB, VASM_INST_MUL, VASM_INST_DIV, VASM_INST_IMUL, VASM_INST_IDIV, 
    VASM_INST_OR, VASM_INST_AND, VASM_INST_NOT, VASM_INST_SHL, VASM_INST_SHR,
    VASM_INST_CMP, VASM_INST_JE, VASM_INST_JN, VASM_INST_JL, VASM_INST_JLE, VASM_INST_JG, VASM_INST_JGE,
    VASM_INST_CALL, VASM_INST_RET, VASM_INST_SYSCALL,

    VASM_COUNT_INSTS,
} VASM_Inst_Type;

typedef struct VASM_Inst_Info {
    VASM_Inst_Type type;
    const char *name;
    bool has_immediate_mode;
} VASM_Inst_Info;

typedef struct VASM_Inst {
    VASM_Inst_Type type;  // 8-bit
    bool imm; // 1 bit
    uint16_t op1; // 16-bit
    uint64_t op2; // 39-bit
} VASM_Inst;

typedef struct VASM_Module VASM_Module;
struct VASM_Module {
    const char *name;
    VASM_Module *next;
    VASM_Module *prev;

    struct {
        VASM_Inst *data;
        size_t count, capacity;
    } program;
};

typedef struct VASM_ByteArray {
    uint8_t *data;
    size_t count, capacity;
} VASM_ByteArray;

void vasm_bytearray_push(VASM_ByteArray *arr, uint8_t byte);
void vasm_bytearray_push_many(VASM_ByteArray *arr, const uint8_t *bytes_data, size_t nbytes);
bool vasm_bytearray_save(const VASM_ByteArray *arr, const char *file_path);

void vasm_module_push(VASM_Module *module, VASM_Inst inst);
void vasm_module_emplace(VASM_Module *module, VASM_Inst_Type type, bool imm, uint16_t op1, uint64_t op2);
VASM_Error vasm_load_module_from_source(VASM_Module *result, const char *source, size_t source_size);

VASM_Error vasm_gen_x86_64(VASM_ByteArray *result, const VASM_Module *module);
VASM_Error vasm_gen_arm64(VASM_ByteArray *result, const VASM_Module *module);
VASM_Error vasm_gen_riscv64(VASM_ByteArray *result, const VASM_Module *module);
VASM_Error vasm_gen_elf_x86_64(VASM_ByteArray *result, const VASM_Module *module);

VASM_Inst_Info vasm_get_inst_info(VASM_Inst_Type type);
VASM_Register_Info vasm_get_register_info(VASM_Register_Code code);
void vasm_dump_inst(const VASM_Inst *inst);

#endif // VASM_H_
