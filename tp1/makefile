
GCC=g++
#CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14
CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

OUT_DIR = build
SRC_DIR = src
OBJ_DIR = obj
BENCH_OBJ_DIR = $(OBJ_DIR)/bench

DIRS = $(OUT_DIR) $(OBJ_DIR) $(BENCH_OBJ_DIR)

INCLUDES = $(SRC_DIR)/include/
CFLAGS += -I $(INCLUDES)

EJERCICIOS = ej1 ej2 ej3
EXTRA_SOURCES = $(SRC_DIR)/bench/benchmark.cpp $(SRC_DIR)/bench/pcg.cpp

BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)
EXTRA_OBJS = $(EXTRA_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

ENTREGABLE=entregable.tar.gz

.PHONY: all $(EJERCICIOS) $(BENCHMARKS) clean tar informe help

all: $(DIRS) $(EJERCICIOS)

tar: $(ENTREGABLE)

$(ENTREGABLE): informe
	tar -czf $@ src -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : $(OBJ_DIR)/%.o $(EXTRA_OBJS)
	$(GCC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) $(CFLAGS) -c -o $@ $<

$(DIRS):
	mkdir -p $@

informe:
	make -C doc/

help:
	@echo "make 		para compilar los ejercicios"
	@echo "make ejX 	para compilar uno solo"
	@echo "make tar 	para generar el archivo entregable"

clean:
	rm -f build/*
	find obj/ -name '*.o' -type f -exec rm -f "{}" \;
	rm -f obj/bench/*
	rm -f $(ENTREGABLE)

