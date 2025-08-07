#include <luacc/codegen.h>
#include <luacc/array.h>
#include <luacc/log.h>

#include <string.h>
#include <stdio.h>

static void _luacc_enter_func(FILE *out, const char *name)
{
	fprintf(out, "%s:\n", name);
	fprintf(out, "\tpush rbp\n");
	fprintf(out, "\tmov rbp, rsp\n");
}

static void _luacc_gen_func(FILE *out, const luacc_function_chunk_t *func_chunk, const char *name)
{
	for (size_t i = 0; i < func_chunk->prototypes->len; i++)
	{
		const luacc_function_chunk_t *proto = (luacc_function_chunk_t *) func_chunk->prototypes->data[i];

		char proto_name_buf[256];

		if (proto->name->len > 0)
			snprintf(proto_name_buf, sizeof(proto_name_buf), "%s", proto->name->str);
		else
			snprintf(proto_name_buf, sizeof(proto_name_buf), "%s_proto_%u", name, (uint32_t) i);

		_luacc_gen_func(out, func_chunk->prototypes->data[i], proto_name_buf);
	}
	
	_luacc_enter_func(out, name);
	
	bool_t function_ended = LUACC_FALSE;
	for (size_t i = 0; i < func_chunk->instructions->len; i++)
	{
		if (function_ended)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "there shouldn't be any instructions after the RETURN instruction at index %u", (uint32_t) i);
			luacc_log(LUACC_LOG_LEVEL_WARNING, buf);

			continue;
		}

		const luacc_instruction_t *instr = (luacc_instruction_t *) func_chunk->instructions->data[i];

		const int REG_A = instr->a;
		const int REG_B = instr->b;
		const int REG_C = instr->c;
		const int REG_BX = instr->bx;
		const int REG_SBX = instr->sbx;
		
		switch (instr->opcode)
		{
		case LUACC_INSTR_OPCODE_MOVE:
			fprintf(out, "\t; MOVE %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_LOADK:
			fprintf(out, "\t; LOADK %d %d\n", REG_A, REG_BX);
			break;
		case LUACC_INSTR_OPCODE_LOADBOOL:
			fprintf(out, "\t; LOADBOOL %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_LOADNIL:
			fprintf(out, "\t; LOADNIL %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_GETUPVAL:
			fprintf(out, "\t; GETUPVAL %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_GETGLOBAL:
			fprintf(out, "\t; GETGLOBAL %d %d\n", REG_A, REG_BX);
			break;
		case LUACC_INSTR_OPCODE_GETTABLE:
			fprintf(out, "\t; GETTABLE %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_SETGLOBAL:
			fprintf(out, "\t; SETGLOBAL %d %d\n", REG_A, REG_BX);
			break;
		case LUACC_INSTR_OPCODE_SETUPVAL:
			fprintf(out, "\t; SETUPVAL %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_SETTABLE:
			fprintf(out, "\t; SETTABLE %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_NEWTABLE:
			fprintf(out, "\t; NEWTABLE %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_SELF:
			fprintf(out, "\t; SELF %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_ADD:
			fprintf(out, "\t; ADD %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_SUB:
			fprintf(out, "\t; SUB %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_MUL:
			fprintf(out, "\t; MUL %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_DIV:
			fprintf(out, "\t; DIV %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_MOD:
			fprintf(out, "\t; MOD %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_POW:
			fprintf(out, "\t; POW %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_UNM:
			fprintf(out, "\t; UNM %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_NOT:
			fprintf(out, "\t; NOT %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_LEN:
			fprintf(out, "\t; LEN %d %d\n", REG_A, REG_B);
			break;
		case LUACC_INSTR_OPCODE_CONCAT:
			fprintf(out, "\t CONCAT %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_JMP:
			fprintf(out, "\t; JMP %d\n", REG_SBX);
			break;
		case LUACC_INSTR_OPCODE_EQ:
			fprintf(out, "\t; EQ %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_LT:
			fprintf(out, "\t; LT %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_TEST:
			fprintf(out, "\t; TEST %d %d\n", REG_A, REG_C);
			break;
		case LUACC_INSTR_OPCODE_TESTSET:
			fprintf(out, "\t; TESTSET %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_CALL:
			fprintf(out, "\t; CALL %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_TAILCALL:
			fprintf(out, "\t; TAILCALL %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_RETURN:
			fprintf(out, "\t; RETURN %d %d\n", REG_A, REG_B);
			fprintf(out, "\tpop rbp\n");
			fprintf(out, "\tret\n");
			
			function_ended = LUACC_TRUE;
			break;
		case LUACC_INSTR_OPCODE_FORLOOP:
			fprintf(out, "\t; FORLOOP %d %d\n", REG_A, REG_SBX);
			break;
		case LUACC_INSTR_OPCODE_FORPREP:
			fprintf(out, "\t; FORPREP %d %d\n", REG_A, REG_SBX);
			break;
		case LUACC_INSTR_OPCODE_TFORLOOP:
			fprintf(out, "\t; TFORLOOP %d %d\n", REG_A, REG_C);
			break;
		case LUACC_INSTR_OPCODE_SETLIST:
			fprintf(out, "\t; SETLIST %d %d %d\n", REG_A, REG_B, REG_C);
			break;
		case LUACC_INSTR_OPCODE_CLOSE:
			fprintf(out, "\t; CLOSE %d\n", REG_A);
			break;
		case LUACC_INSTR_OPCODE_CLOSURE:
			fprintf(out, "\t; CLOSURE %d %d\n", REG_A, REG_BX);
			break;
		case LUACC_INSTR_OPCODE_VARARG:
			fprintf(out, "\t; VARARG %d %d\n", REG_A, REG_B);
			break;
		default:
			char buf[256];
			snprintf(buf, sizeof(buf), "invalid instruction opcode %d", instr->opcode);

			luacc_log(LUACC_LOG_LEVEL_WARNING, buf);
		}
	}
}

void luacc_func_chunk_generate(const luacc_function_chunk_t *func_chunk, const char *output_fname)
{
	FILE *out = fopen(output_fname, "w+");

	if (!out)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "luacc_func_chunk_generate(): could not open %s for writing", output_fname);

		luacc_log(LUACC_LOG_LEVEL_ERROR, buf);

		return;
	}
	
	fprintf(out, ";\n; this assembly was generated by luacc - do not edit if you're dumb\n");
	fprintf(out, "; entry point prototype: %s\n;\n\n", func_chunk->name->str);

	_luacc_gen_func(out, func_chunk, func_chunk->name->str);
	
	fclose(out);
}
