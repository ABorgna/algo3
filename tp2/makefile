
GCC=g++
CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14 -march=native
#CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

OUT_DIR = build
SRC_DIR = src
OBJ_DIR = obj
BENCH_OBJ_DIR = $(OBJ_DIR)/bench
BENCH_OUT_DIR = bench
GRAPHS_OUT_DIR = graph

DIRS = $(OUT_DIR) $(OBJ_DIR) $(BENCH_OBJ_DIR) $(BENCH_OUT_DIR) $(GRAPHS_OUT_DIR)

INCLUDES = $(SRC_DIR)/include/
CFLAGS += -I $(INCLUDES)

EJERCICIOS = ej1 ej2 ej3
EXTRA_SOURCES = $(SRC_DIR)/bench/benchmark.cpp $(SRC_DIR)/bench/pcg.cpp

BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)
EXTRA_OBJS = $(EXTRA_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TESTS = $(EJERCICIOS:%=%-test)

BENCHMARKS = $(EJERCICIOS:%=%-bench)
BENCHMARKS_OUT = $(EJERCICIOS:%=$(BENCH_OUT_DIR)/%)

GRAPHS =  $(EJERCICIOS:%=%-graph)
GRAPHS_OUT = $(EJERCICIOS:%=$(GRAPHS_OUT_DIR)/%)

ENTREGABLE=entregable.tar.gz

# Funcion auxiliar para calcular a partir de que problema
# continuar los bechmarks
CHECKPOINT = 0`wc -l $(1) 2>/dev/null | sed 's/ .*//'`

.PHONY: all $(EJERCICIOS) $(BENCHMARKS) $(BENCHMARKS_OUT) \
	$(GRAPHS) $(GRAPHS_OUT) $(TESTS) \
	clean tar informe help dirs benchmark graphs tests

all: dirs $(EJERCICIOS)

tar: $(ENTREGABLE)

dirs: $(DIRS)

tests: $(TESTS)

benchmarks: $(BENCHMARKS)

graphs: $(GRAPHS)

$(ENTREGABLE): informe
	tar -czf $@ makefile tools src -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : $(OBJ_DIR)/%.o $(EXTRA_OBJS)
	$(GCC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) $(CFLAGS) -c -o $@ $<

$(DIRS):
	mkdir -p $@

$(TESTS) : %-test : %
	@find test/ -name '$^.*.test' -exec sh -c \
		'echo "---- Test" $${0#test/} "----" && \
		$(OUT_DIR)/$^ -i $$0 -t $${0%.test}.res' {}  \;

$(BENCHMARKS) : %-bench : $(BENCH_OUT_DIR)/%

$(BENCH_OUT_DIR)/ej1: ej1
	@echo "TODO: tests ej1"

$(BENCH_OUT_DIR)/ej2: ej2
	@echo "TODO: tests ej2"

$(BENCH_OUT_DIR)/ej3: ej3
	@echo "TODO: tests ej3"

$(GRAPHS) : %-graph : $(GRAPHS_OUT_DIR)/%

$(GRAPHS_OUT_DIR)/ej1: $(BENCH_OUT_DIR)/ej1
	@echo "TODO: graphs ej1"

$(GRAPHS_OUT_DIR)/ej2: $(BENCH_OUT_DIR)/ej2
	@echo "TODO: graphs ej2"

$(GRAPHS_OUT_DIR)/ej3: $(BENCH_OUT_DIR)/ej3
	@echo "TODO: graphs ej3"

informe: $(GRAPHS)
	make -C doc/

help:
	@echo "make 			compilar los ejercicios"
	@echo "make ejX 		compilar uno solo"
	@echo "make ejX-test	correr los tests disponibles"
	@echo "make benchmarks 	correr los benchmarks"
	@echo "make ejX-bech 	correr el benchmark de un ejercicio"
	@echo "make graphs 		generar todos los graficos"
	@echo "make ejX-graph 	generar los graficos de un ejercicio"
	@echo "make tar 		generar el archivo entregable"

clean:
	rm -f build/*
	find obj/ -name '*.o' -type f -exec rm -f "{}" \;
	rm -f obj/bench/*
	rm -f graph/*
	rm -f $(ENTREGABLE)

