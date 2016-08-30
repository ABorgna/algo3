
GCC=g++
CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14
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

BENCHMARKS = $(EJERCICIOS:%=%-bench)
BENCHMARKS_OUT = $(EJERCICIOS:%=$(BENCH_OUT_DIR)/%)

GRAPHS =  $(EJERCICIOS:%=%-graph)
GRAPHS_OUT = $(EJERCICIOS:%=$(GRAPHS_OUT_DIR)/%)

ENTREGABLE=entregable.tar.gz

.PHONY: all $(EJERCICIOS) $(BENCHMARKS) $(BENCHMARKS_OUT) $(GRAPHS) \
	clean tar informe help dirs benchmark

all: dirs $(EJERCICIOS)

tar: $(ENTREGABLE)

dirs: $(DIRS)

benchmarks: $(BENCHMARKS)

graphs: $(GRAPHS)

$(ENTREGABLE): informe
	tar -czf $@ src -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : $(OBJ_DIR)/%.o $(EXTRA_OBJS)
	$(GCC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) $(CFLAGS) -c -o $@ $<

$(DIRS):
	mkdir -p $@

$(BENCHMARKS) : %-bench : $(BENCH_OUT_DIR)/%

$(BENCH_OUT_DIR)/ej1: ej1
	build/ej1 -a 0:3 0:3 -A 1 -c 0`wc -l $@ 2>/dev/null | sed 's/ .*//'` -o $@

$(BENCH_OUT_DIR)/ej2: ej2
	build/ej2 -a 1:1000:1 -A 0.01 -o $@-0 \
		-c 0`wc -l $@-0 2>/dev/null | sed 's/ .*//'`
	build/ej2 -a 2000:1000000:1000 -A 0.01 -o $@-1 \
		-c 0`wc -l $@-1 2>/dev/null | sed 's/ .*//'`
	build/ej2 -a 2000000:1000000000:1000000 -A 0.01 -o $@-2 \
		-c 0`wc -l $@-2 2>/dev/null | sed 's/ .*//'`
	build/ej2 -a 2000000000:1000000000000:1000000000 -A 0.01 -o $@-3 \
		-c 0`wc -l $@-3 2>/dev/null | sed 's/ .*//'`
	build/ej2 -a 2000000000000:1000000000000000:1000000000000 -A 0.01 -o $@-4 \
		-c 0`wc -l $@-4 2>/dev/null | sed 's/ .*//'`
	cat $@-0 > $@
	cat $@-1 >> $@
	cat $@-2 >> $@
	cat $@-3 >> $@
	cat $@-4 >> $@

$(BENCH_OUT_DIR)/ej3: ej3
	@echo "TODO"

$(GRAPHS) : %-graph : $(GRAPHS_OUT_DIR)/%

$(GRAPHS_OUT_DIR)/ej1: $(BENCH_OUT_DIR)/ej1-bench
	tools/grapher.py -i $< -o $@ --log -x "Peso" -t "Tiempo aoeuaoueou"

$(GRAPHS_OUT_DIR)/ej2: $(BENCH_OUT_DIR)/ej2-bench
	tools/grapher.py -i $< -o $@ --log -x "Peso" -t "Tiempo aoeuaoueou"

$(GRAPHS_OUT_DIR)/ej3: $(BENCH_OUT_DIR)/ej3-bench
	@echo "TODO"

informe: $(GRAPHS)
	make -C doc/

help:
	@echo "make 			para compilar los ejercicios"
	@echo "make ejX 		para compilar uno solo"
	@echo "make benchmarks 	para correr los benchmarks"
	@echo "make ejX-bech 	para correr el benchmark de un ejercicio"
	@echo "make graphs 		para generar todos los graficos"
	@echo "make ejX-graph 	para generar los graficos de un ejercicio"
	@echo "make tar 		para generar el archivo entregable"

clean:
	rm -f build/*
	find obj/ -name '*.o' -type f -exec rm -f "{}" \;
	rm -f obj/bench/*
	rm -f graph/*
	rm -f $(ENTREGABLE)

