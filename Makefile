CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11 -Iinclude
LDFLAGS = -lncurses -lrt -pthread

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
BIN := bashndash

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJ) | build
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p build

clean:
	rm -rf build $(BIN)

run: all
	./run.sh
