#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <luacc/luac_func_chunk.h>

// generates assembly code, duh
void luacc_func_chunk_generate(const luacc_function_chunk_t *func_chunk, const char *output_fname);

#endif // _CODEGEN_H
