# LUACC
### The Lua Compiler Compiler
Compiles Lua bytecode to assembly (.asm), from there it is assembled to an object file (.o/obj), and then linked into machine code (.exe/.out).

NOTE: there are programs out there that do the same job but better. just letting you know, the code here is garbage

## The process:
Assuming NASM is your assembler, and LD is your linker.
### (for Windows)
Lua source .LUA -(luac)> Lua bytecode .OUT/.LUAC -(luacc)> Assembly .ASM -(nasm)> Object file .OBJ -(ld)> Machine code .EXE
### (for Unix)
Lua source .LUA -(luac)> Lua bytecode .OUT/.LUAC -(luacc)> Assembly .ASM -(nasm)> Object file .O -(ld)> Machine code .OUT

## Usage:
```
luacc [arguments] <input files>
```
Examples: `luacc -o app.exe my_app.out`, `luacc hello.out`

## Compiling instructions coming soon