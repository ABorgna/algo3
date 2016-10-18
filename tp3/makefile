
GCC=g++
CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++11 -march=native
#CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

OUT_DIR = build
SRC_DIR = src
OBJ_DIR = obj
BENCH_OBJ_DIR = $(OBJ_DIR)/bench
GRAPH_OBJ_DIR = $(OBJ_DIR)/graph

BENCH_OUT_DIR = bench
GRAPHS_OUT_DIR = graph

DIRS = $(OUT_DIR) $(OBJ_DIR) $(BENCH_OBJ_DIR) $(GRAPH_OBJ_DIR) \
	   $(BENCH_OUT_DIR) $(GRAPHS_OUT_DIR)

INCLUDES = $(SRC_DIR)/include/
CFLAGS += -I $(INCLUDES)

HEURISTICAS = exacto greedy local
EXTRA_SOURCES = $(SRC_DIR)/bench/benchmark.cpp $(SRC_DIR)/bench/pcg.cpp \
				$(SRC_DIR)/graph/generators.cpp $(SRC_DIR)/graph/pokegraph.cpp \
				$(SRC_DIR)/boilerplate.cpp $(SRC_DIR)/utils.cpp

BINARIOS = $(HEURISTICAS:%=$(OUT_DIR)/%)
EXTRA_OBJS = $(EXTRA_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TESTS = $(HEURISTICAS:%=%-test)

BENCHMARKS = $(HEURISTICAS:%=%-bench)
BENCHMARKS_OUT = $(HEURISTICAS:%=$(BENCH_OUT_DIR)/%)

GRAPHS =  $(HEURISTICAS:%=%-graph)
GRAPHS_OUT = $(HEURISTICAS:%=$(GRAPHS_OUT_DIR)/%)

ENTREGABLE=entregable.tar.gz

# Funcion auxiliar para calcular a partir de que problema
# continuar los bechmarks
CHECKPOINT = 0`wc -l $(1) 2>/dev/null | sed 's/ .*//'`

.PHONY: all $(HEURISTICAS) $(BENCHMARKS) $(BENCHMARKS_OUT) \
	$(GRAPHS) $(GRAPHS_OUT) $(TESTS) \
	clean tar informe help dirs benchmark graphs tests

all: dirs $(HEURISTICAS)

tar: $(ENTREGABLE)

dirs: $(DIRS)

tests: $(TESTS)

benchmarks: $(BENCHMARKS)

graphs: $(GRAPHS)

$(ENTREGABLE): informe
	tar -czf $@ makefile tools src -C doc informe.pdf

$(HEURISTICAS) : % : build/%

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

$(BENCH_OUT_DIR)/exacto: exacto
	@echo "TODO"

$(BENCH_OUT_DIR)/greedy: greedy
	@echo "TODO"

$(BENCH_OUT_DIR)/local: local
	@echo "TODO"

$(GRAPHS) : %-graph : $(GRAPHS_OUT_DIR)/%

$(GRAPHS_OUT_DIR)/exacto: $(BENCH_OUT_DIR)/exacto
	@echo "TODO"

$(GRAPHS_OUT_DIR)/greedy: $(BENCH_OUT_DIR)/greedy
	@echo "TODO"

$(GRAPHS_OUT_DIR)/local: $(BENCH_OUT_DIR)/local
	@echo "TODO"

informe: $(GRAPHS)
	make -C doc/

help:
	@echo "make                compilar los ejercicios"
	@echo "make tests          correr todos los tests"
	@echo "make benchmarks     correr los benchmarks"
	@echo "make graphs         generar todos los graficos"
	@echo "make {HEURISTICA}   compilar uno solo"
	@echo "make HRS-test       correr los tests disponibles"
	@echo "make HRS-bench      correr el benchmark de un ejercicio"
	@echo "make HRS-graph      generar los graficos de un ejercicio"
	@echo "make tar            generar el archivo entregable"

clean:
	rm -f build/*
	find obj/ -name '*.o' -type f -exec rm -f "{}" \;
	rm -f obj/bench/*
	rm -f obj/graph/*
	rm -f graph/*
	rm -f $(ENTREGABLE)

