ROOT_DIR = ../..
SRC_FILES = ./src/*.c $(ROOT_DIR)/src/*.c
OUT = bin/luacc51.exe

C = clang
CCFLAGS = -std=c17 -I $(ROOT_DIR)/include --static

luacc:
	$(C) $(CCFLAGS) $(SRC_FILES) -o $(OUT)

.PHONY: run clean

run:
	$(OUT)

clean:
	del $(OUT)