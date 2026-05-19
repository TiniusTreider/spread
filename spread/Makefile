CC = gcc
CFLAGS = -Wall -Wextra -O3 -Iinclude -march=native -flto -lm -std=c11

sources = $(wildcard src/*)
objects = $(patsubst src/%.c,build/%.o,$(sources))
executable = spread

.PHONY: all debug

all: $(executable)

debug: CFLAGS = -Wall -Wextra -g -Iinclude -lm -std=c11
debug: clean $(executable)

$(executable): $(objects)
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

.PHONY: clean

clean:
	rm -f $(executable) $(objects)

