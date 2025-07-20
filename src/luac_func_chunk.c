#include <luacc/luac_func_chunk.h>
#include <luacc/luac_util.h>
#include <luacc/array.h>
#include <luacc/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const luacc_instr_type_t TYPES[38] = {
	LUACC_INSTR_TYPE_ABC,        // MOVE
	LUACC_INSTR_TYPE_ABX,        // LOADK
	LUACC_INSTR_TYPE_ABC,        // LOADBOOL
	LUACC_INSTR_TYPE_ABC,        // LOADNIL
	LUACC_INSTR_TYPE_ABC,        // GETUPVAL
	LUACC_INSTR_TYPE_ABX,        // GETGLOBAL
	LUACC_INSTR_TYPE_ABC,        // GETTABLE
	LUACC_INSTR_TYPE_ABX,        // SETGLOBAL
	LUACC_INSTR_TYPE_ABC,        // SETUPVAL
	LUACC_INSTR_TYPE_ABC,        // SETTABLE
	LUACC_INSTR_TYPE_ABC,        // NEWTABLE
	LUACC_INSTR_TYPE_ABC,        // SELF
	LUACC_INSTR_TYPE_ABC,        // ADD
	LUACC_INSTR_TYPE_ABC,        // SUB
	LUACC_INSTR_TYPE_ABC,        // MUL
	LUACC_INSTR_TYPE_ABC,        // DIV
	LUACC_INSTR_TYPE_ABC,        // MOD
	LUACC_INSTR_TYPE_ABC,        // POW
	LUACC_INSTR_TYPE_ABC,        // UNM
	LUACC_INSTR_TYPE_ABC,        // NOT
	LUACC_INSTR_TYPE_ABC,        // LEN
	LUACC_INSTR_TYPE_ABC,        // CONCAT
	LUACC_INSTR_TYPE_ASBX,       // JMP
	LUACC_INSTR_TYPE_ABC,        // EQ
	LUACC_INSTR_TYPE_ABC,        // LT
	LUACC_INSTR_TYPE_ABC,        // LE
	LUACC_INSTR_TYPE_ABC,        // TEST
	LUACC_INSTR_TYPE_ABC,        // TESTSET
	LUACC_INSTR_TYPE_ABC,        // CALL
	LUACC_INSTR_TYPE_ABC,        // TAILCALL
	LUACC_INSTR_TYPE_ABC,        // RETURN
	LUACC_INSTR_TYPE_ASBX,       // FORLOOP
	LUACC_INSTR_TYPE_ASBX,       // FORPREP
	LUACC_INSTR_TYPE_ABC,        // TFORLOOP
	LUACC_INSTR_TYPE_ABC,        // SETLIST
	LUACC_INSTR_TYPE_ABC,        // CLOSE
	LUACC_INSTR_TYPE_ABX,        // CLOSURE
	LUACC_INSTR_TYPE_ABC,        // VARARG
};

const char *OPCODE_NAMES[38] = {
	"MOVE",
	"LOADK",
	"LOADBOOL",
	"LOADNIL",
	"GETUPVAL",
	"GETGLOBAL",
	"GETTABLE",
	"SETGLOBAL",
	"SETUPVAL",
	"SETTABLE",
	"NEWTABLE",
	"SELF",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"POW",
	"UNM",
	"NOT",
	"LEN",
	"CONCAT",
	"JMP",
	"EQ",
	"LT",
	"LE",
	"TEST",
	"TESTSET",
	"CALL",
	"TAILCALL",
	"RETURN",
	"FORLOOP",
	"FORPREP",
	"TFORLOOP",
	"SETLIST",
	"CLOSE",
	"CLOSURE",
	"VARARG"
};

luacc_instruction_t *luacc_decode_instruction(const int instr)
{
	luacc_instruction_t *instruction = (luacc_instruction_t *) malloc(sizeof(luacc_instruction_t));

	if (!instruction)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "failed to allocate memory for an instruction");

	instruction->opcode = LUACC_INSTR_OPCODE(instr);

	if (instruction->opcode >= 38)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "unknown instruction opcode %i", instruction->opcode);

		luacc_log(LUACC_LOG_LEVEL_FATAL, buf);
	}

	instruction->type = TYPES[instruction->opcode];
	instruction->a = LUACC_ARG_A(instr);

	instruction->b = 0;
	instruction->c = 0;
	instruction->bx = 0;
	instruction->sbx = 0;

	switch (instruction->type)
	{
	case LUACC_INSTR_TYPE_ABC:
		instruction->b = LUACC_ARG_B(instr);
		instruction->c = LUACC_ARG_C(instr);
		break;
	case LUACC_INSTR_TYPE_ABX:
		instruction->bx = LUACC_ARG_Bx(instr);
		break;
	case LUACC_INSTR_TYPE_ASBX:
		instruction->sbx = LUACC_ARG_sBx(instr);
		break;
	default:
		free(instruction);
		luacc_log(LUACC_LOG_LEVEL_FATAL, "unknown instruction type. expected LUACC_INSTR_TYPE_ABC, LUACC_INSTR_TYPE_ABX, LUACC_INSTR_TYPE_ASBX");
	}
	
	return instruction;
}

void luacc_free_instruction(luacc_instruction_t *instr)
{
	free(instr);
}

luacc_function_chunk_t *luacc_parse_function_chunk(const luacc_chunk_t *chunk, size_t *index)
{
	luacc_function_chunk_t *func_chunk = (luacc_function_chunk_t *) malloc(sizeof(luacc_function_chunk_t));
	
	if (!func_chunk)
		luacc_log(LUACC_LOG_LEVEL_FATAL, "memory allocation failure while allocating memory for a function chunk");

	func_chunk->name = luacc_get_lua_str(chunk, index);
	func_chunk->first_line = luacc_get_uint(chunk, index);
	func_chunk->last_line = luacc_get_uint(chunk, index);
	func_chunk->upvalues = luacc_get_byte(chunk, index);
	func_chunk->arguments = luacc_get_byte(chunk, index);
	func_chunk->varg = luacc_get_byte(chunk, index);
	func_chunk->stack = luacc_get_byte(chunk, index);
	
	const uint64_t INSTRUCTION_COUNT = luacc_get_uint(chunk, index);

	func_chunk->instructions = luacc_alloc_array();
	luacc_array_resize(func_chunk->instructions, INSTRUCTION_COUNT);

	for (uint64_t i = 0; i < INSTRUCTION_COUNT; i++)
	{
		int instr_code = (int) luacc_get_u32(chunk, index);
		luacc_instruction_t *instr = luacc_decode_instruction(instr_code);

		luacc_array_append(func_chunk->instructions, instr);
	}
	
	const uint64_t CONSTANT_COUNT = luacc_get_uint(chunk, index);

	func_chunk->constants = luacc_alloc_array();
	luacc_array_resize(func_chunk->constants, CONSTANT_COUNT);

	for (uint64_t i = 0; i < CONSTANT_COUNT; i++)
	{
		luacc_constant_t *constant = (luacc_constant_t *) malloc(sizeof(luacc_constant_t));
		constant->type = luacc_get_byte(chunk, index);
		constant->bool_val = LUACC_FALSE;
		constant->double_val = 0.0;
		constant->str_val = NULL;

		switch (constant->type)
		{
		case LUACC_CONST_TYPE_NIL:
			// womp womp
			break;
		case LUACC_CONST_TYPE_BOOL:
			constant->bool_val = luacc_get_byte(chunk, index);

			if (constant->bool_val > 1)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "in %s: invalid bool constant (expected a byte which is either 0x00 or 0x01, but got 0x%x). bool values that aren't 0 will always equate to true", func_chunk->name->str, constant->bool_val);
				
				luacc_log(LUACC_LOG_LEVEL_WARNING, buf);
			}

			break;
		case LUACC_CONST_TYPE_NUMBER:
			constant->double_val = luacc_get_double(chunk, index);
			break;
		case LUACC_CONST_TYPE_STRING:
			constant->str_val = luacc_get_lua_str(chunk, index);
			break;
		default:
			char buf[256];
			snprintf(buf, sizeof(buf), "in %s: invalid constant type (expected 0x00 (NIL), 0x01 (BOOL), 0x03 (LUA_NUMBER), 0x04 (STRING))", func_chunk->name->str);

			luacc_log(LUACC_LOG_LEVEL_FATAL, buf);
		}

		luacc_array_append(func_chunk->constants, constant);
	}

	const uint64_t PROTOTYPE_COUNT = luacc_get_uint(chunk, index);

	func_chunk->prototypes = luacc_alloc_array();
	luacc_array_resize(func_chunk->prototypes, PROTOTYPE_COUNT);

	for (uint64_t i = 0; i < PROTOTYPE_COUNT; i++)
	{
		luacc_function_chunk_t *proto = luacc_parse_function_chunk(chunk, index);
		luacc_array_append(func_chunk->prototypes, proto);
	}

	if (luacc_has_verbose_logging())
	{
		printf("%s: %u instructions, %u constants, %u prototypes\n", func_chunk->name->str, (uint32_t) INSTRUCTION_COUNT, (uint32_t) CONSTANT_COUNT, (uint32_t) PROTOTYPE_COUNT);
		
		for (size_t i = 0; i < func_chunk->instructions->len; i++)
		{
			luacc_instruction_t *instr = func_chunk->instructions->data[i];

			printf("\t%s", OPCODE_NAMES[instr->opcode]);

			for (size_t i = 0; i < 15 - strlen(OPCODE_NAMES[instr->opcode]); i++)
				putchar(' ');

			switch (instr->type) 
			{
			case LUACC_INSTR_TYPE_ABC:
				printf("%d %d %d\n", instr->a, instr->b, instr->c);
				break;
			case LUACC_INSTR_TYPE_ABX:
				printf("%d %d\n", instr->a, instr->bx);
				break;
			case LUACC_INSTR_TYPE_ASBX:
				printf("%d %d\n", instr->a, instr->sbx);
				break;
			default:
				printf("unknown instruction type\n");
			}
		}

		if (func_chunk->constants->len > 0)
		{
			printf("\n\tconstants in %s: \n", func_chunk->name->str);

			for (size_t i = 0; i < func_chunk->constants->len; i++)
			{
				luacc_constant_t *constant = (luacc_constant_t *) func_chunk->constants->data[i];
				printf("\t\t%u: ", (uint32_t) i);

				switch (constant->type)
				{
				case LUACC_CONST_TYPE_NIL:
					printf("NIL");
					break;
				case LUACC_CONST_TYPE_BOOL:
					printf("BOOL %s", constant->bool_val ? "true" : "false");
					break;
				case LUACC_CONST_TYPE_NUMBER:
					printf("NUMBER %.2f", constant->double_val);
					break;
				case LUACC_CONST_TYPE_STRING:
					printf("STRING %s", constant->str_val->str);
					break;
				}

				putchar('\n');
			}
		}
	}

	return func_chunk;
}

void luacc_free_function_chunk(luacc_function_chunk_t *func_chunk)
{
	free(func_chunk->name);

	for (size_t i = 0; i < func_chunk->instructions->len; i++)
		luacc_free_instruction(func_chunk->instructions->data[i]);
	
	luacc_free_array(func_chunk->instructions);
	
	for (size_t i = 0; i < func_chunk->constants->len; i++)
	{
		luacc_constant_t *constant = func_chunk->constants->data[i];

		if (constant->str_val)
			luacc_lua_str_free(constant->str_val);
		
		free(constant);
	}

	luacc_free_array(func_chunk->constants);
	luacc_free_array(func_chunk->prototypes);
}
