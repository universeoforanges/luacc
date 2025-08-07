#ifndef _LUAC_FUNC_CHUNK_H
#define _LUAC_FUNC_CHUNK_H

#include <luacc/luac_chunk.h>
#include <luacc/luac_util.h>
#include <luacc/array.h>
#include <luacc/util.h>

#include <stdint.h>

typedef enum
{
	LUACC_INSTR_TYPE_NONE = 0,
	LUACC_INSTR_TYPE_ABC  = 1,
	LUACC_INSTR_TYPE_ABX  = 2,
	LUACC_INSTR_TYPE_ASBX = 3
} luacc_instr_type_t;

typedef enum
{
	LUACC_INSTR_OPCODE_MOVE         = 0,
	LUACC_INSTR_OPCODE_LOADK        = 1,
	LUACC_INSTR_OPCODE_LOADBOOL     = 2,
	LUACC_INSTR_OPCODE_LOADNIL      = 3,
	LUACC_INSTR_OPCODE_GETUPVAL     = 4,
	LUACC_INSTR_OPCODE_GETGLOBAL    = 5,
	LUACC_INSTR_OPCODE_GETTABLE     = 6,
	LUACC_INSTR_OPCODE_SETGLOBAL    = 7,
	LUACC_INSTR_OPCODE_SETUPVAL     = 8,
	LUACC_INSTR_OPCODE_SETTABLE     = 9,
	LUACC_INSTR_OPCODE_NEWTABLE     = 10,
	LUACC_INSTR_OPCODE_SELF         = 11,
	LUACC_INSTR_OPCODE_ADD          = 12,
	LUACC_INSTR_OPCODE_SUB          = 13,
	LUACC_INSTR_OPCODE_MUL          = 14,
	LUACC_INSTR_OPCODE_DIV          = 15,
	LUACC_INSTR_OPCODE_MOD          = 16,
	LUACC_INSTR_OPCODE_POW          = 17,
	LUACC_INSTR_OPCODE_UNM          = 18,
	LUACC_INSTR_OPCODE_NOT          = 19,
	LUACC_INSTR_OPCODE_LEN          = 20,
	LUACC_INSTR_OPCODE_CONCAT       = 21,
	LUACC_INSTR_OPCODE_JMP          = 22,
	LUACC_INSTR_OPCODE_EQ           = 23,
	LUACC_INSTR_OPCODE_LT           = 24,
	LUACC_INSTR_OPCODE_LE           = 25,
	LUACC_INSTR_OPCODE_TEST         = 26,
	LUACC_INSTR_OPCODE_TESTSET      = 27,
	LUACC_INSTR_OPCODE_CALL         = 28,
	LUACC_INSTR_OPCODE_TAILCALL     = 29,
	LUACC_INSTR_OPCODE_RETURN       = 30,
	LUACC_INSTR_OPCODE_FORLOOP      = 31,
	LUACC_INSTR_OPCODE_FORPREP      = 32,
	LUACC_INSTR_OPCODE_TFORLOOP     = 33,
	LUACC_INSTR_OPCODE_SETLIST      = 34,
	LUACC_INSTR_OPCODE_CLOSE        = 35,
	LUACC_INSTR_OPCODE_CLOSURE      = 36,
	LUACC_INSTR_OPCODE_VARARG       = 37
} luacc_instr_opcode_t;

typedef enum
{
	LUACC_CONST_TYPE_NIL     = 0,
	LUACC_CONST_TYPE_BOOL    = 1,
	LUACC_CONST_TYPE_NUMBER  = 3,
	LUACC_CONST_TYPE_STRING  = 4
} luacc_constant_type_t;

typedef struct
{
	uint32_t instruction;

	luacc_instr_opcode_t opcode;
	luacc_instr_type_t type;

	int a, b, c, bx, sbx;
} luacc_instruction_t;

typedef struct
{
	luacc_constant_type_t type;
	
	bool_t bool_val;
	double double_val;
	luacc_lua_str_t *str_val;
} luacc_constant_t;

typedef struct
{
	luacc_chunk_t *src_chunk;

	luacc_lua_str_t *name;
	
	uint64_t first_line; // unused
	uint64_t last_line; // unused 
	
	byte_t upvalues;
	byte_t arguments;
	byte_t varg;
	byte_t stack;

	array_t *instructions;
	array_t *constants;
	array_t *prototypes;
} luacc_function_chunk_t;

#define LUACC_INSTR_OPCODE(i) ((i) & 0x3F)
#define LUACC_ARG_A(i)        (((i) >> 6) & 0xFF)
#define LUACC_ARG_B(i)        (((i) >> 23) & 0x1FF)
#define LUACC_ARG_C(i)        (((i) >> 14) & 0x1FF)
#define LUACC_ARG_Bx(i)       (((i) >> 14) & 0x3FFFF)
#define LUACC_ARG_sBx(i)      (LUACC_ARG_Bx(i) - (((1 << 18) - 1) >> 1))

// instruction stuff

luacc_instruction_t *luacc_decode_instruction(const int instr);
void luacc_free_instruction(luacc_instruction_t *instr);

// function chunk stuff

luacc_function_chunk_t *luacc_parse_function_chunk(const luacc_chunk_t *chunk, size_t *index);
void luacc_free_function_chunk(luacc_function_chunk_t *func_chunk);

#endif // _LUAC_FUNC_CHUNK_H
