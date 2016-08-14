
GCC=g++
CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

OUT_DIR = build

DEPEND = makefile $(OUT_DIR)

EJERCICIOS = src/ej1.cpp src/ej2.cpp src/ej3.cpp
BINARIOS = $(EJERCICIOS:src/%.cpp=$(OUT_DIR)/%)

all: $(BINARIOS)

$(BINARIOS): $(OUT_DIR)/% : src/%.cpp $(DEPEND)
	$(GCC) $(CFLAGS) $< -o $@

%.o: %.cpp %.h
	$(GCC) $(CFLAGS) -c -o $@ $<

$(OUT_DIR):
	mkdir -p $@

clean:
	rm -f build/*

