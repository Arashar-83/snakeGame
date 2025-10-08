CC := g++
CFLAGS := -std=c++17 $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs)

SRC := src/main.cpp
BIN := build/snake

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -O2 -o $(BIN) $(SRC) $(LDFLAGS)

run: all
	./$(BIN)

clean:
	rm -rf build