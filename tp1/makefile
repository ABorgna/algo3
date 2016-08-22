
GCC=g++
#CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14
CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

OUT_DIR = build

DEPEND = makefile

EJERCICIOS = ej1 ej2 ej3 v2

SOURCES = $(EJERCICIOS:%=src/%.cpp)
BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)

.PHONY: all $(EJERCICIOS) clean

all: $(OUT_DIR) $(EJERCICIOS)

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : src/%.cpp $(DEPEND)
	$(GCC) $(CFLAGS) $< -o $@

%.o: %.cpp %.h
	$(GCC) $(CFLAGS) -c -o $@ $<

$(OUT_DIR):
	mkdir -p $@

informe:
	make -C doc/

clean:
	rm -f build/*

