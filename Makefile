CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic -o
SRC_DIR=./src/c_sources
VALGRIND_FLAGS=--tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --undef-value-errors=no
LIBS=-lm -llapack

.PHONY: clean

build:
	$(CC) $(CFLAGS) editor $(SRC_DIR)/*.c $(LIBS)

valgrind: build
	valgrind $(VALGRIND_FLAGS) ./editor

clean:
	rm editor
