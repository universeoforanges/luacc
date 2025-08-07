# LuaCC - The Lua Bytecode Compiler written in C
## Completely rewritten as of 6/3/25!
LuaCC is a compiler which compiles Lua bytecode to assembly, which can be assembled & linked into machine code. Pretty useful, if you ask me

Only Lua 5.1 bytecode and NASM assembly is supported as of now. Extended support will be added later

LuaCC only supports Linux as of now

***WARNING: If you see unsafe code (such as freeing freed memory, undefined behavior or forgetting to free memory), PLEASE MAKE AN ISSUE!!***

## Usage
```
luacc [options] input file(s)
```
e.g. `luacc --asm=nasm bytecode.lua`

## Compilation
LuaCC requires barely any dependencies and only relies on the C standard library & OS APIs. So as long as you have a C compiler (`gcc` is a solid option) & CMake installed on your system, you should be able to compile LuaCC without any issues.

To compile LuaCC, run the following commands in a terminal:

```bash
mkdir build
cd build
cmake ..
make
```

## Why LuaCC?
LuaCC was created to be a hobby project and was made purely for fun. However, you can contribute to make LuaCC better.
