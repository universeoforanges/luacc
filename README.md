# LuaCC - The Lua Compiler Compiler written in C
## Completely rewritten as of 6/3/25!
LuaCC is a compiler which compiles Lua bytecode to assembly, which can be assembled & linked into machine code. Pretty useful, if you ask me

Only Lua 5.1 bytecode and NASM assembly is supported as of now. Extended support will be added later

LuaCC only supports Linux as of now

***WARNING: If you see unsafe code (such as freeing freed memory, undefined behavior or forgetting to free memory), PLEASE MAKE AN ISSUE!!***

## Usage
```
luacc [options] input file(s)
```
e.g. `luacc -o assembly.asm bytecode.lua`

## Compilation
compilation instructions coming soon

## Why LuaCC?
to learn, and why not???
