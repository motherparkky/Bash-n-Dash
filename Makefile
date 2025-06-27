CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11 -Iinclude

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDFLAGS = -lncurses -lrt -pthread
else ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -lncurses -pthread
endif

SRC  = $(wildcard src/*.c)
OBJ  = $(patsubst src/%.c,build/%.o,$(SRC))
BIN  = bash_n_dash

all: $(BIN)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p build

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -rf build $(BIN)

.PHONY: all clean
