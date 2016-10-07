
GCC=g++
CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++11 -march=native
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
	build/ej1 -a 4:100:4 4:100:4 0:100:4 4 -A 0.01 \
		-c $(call CHECKPOINT,$@-base) -o $@-base
	build/ej1 -a 10:600:10 10:1000:10 10 4 -A 0.01 \
		-c $(call CHECKPOINT,$@-size) -o $@-size
	build/ej1 -a 500 500 0:65 4 4 -A 0.01 \
		-c $(call CHECKPOINT,$@-walls) -o $@-walls
	cat $@-* > $@

$(BENCH_OUT_DIR)/ej2: ej2
	build/ej2 -a 10:10000:10 100 4 -A 0.01 \
		-c $(call CHECKPOINT,$@-width) -o $@-width
	build/ej2 -a 100 10:10000:10 4 -A 0.01 \
		-c $(call CHECKPOINT,$@-height) -o $@-height
	build/ej2 -a 10:1110:100 10:10000:100 4 -A 0.01 \
		-c $(call CHECKPOINT,$@-both) -o $@-both
	cat $@-* > $@

$(BENCH_OUT_DIR)/ej3: ej3
	build/ej3 -a 10:20000:100 -A 0.01 -G completo \
		-c $(call CHECKPOINT,$@-completo) -o $@-completo
	build/ej3 -a 10:20000:100 -A 0.01 -G random \
		-c $(call CHECKPOINT,$@-random) -o $@-random
	build/ej3 -a 10:20000:100 -A 0.01 -G ciclo \
		-c $(call CHECKPOINT,$@-ciclo) -o $@-ciclo
	build/ej3 -a 10:20000:100 -A 0.01 -G grilla \
		-c $(call CHECKPOINT,$@-grilla) -o $@-grilla
	cat $@-* > $@

$(GRAPHS) : %-graph : $(GRAPHS_OUT_DIR)/%

$(GRAPHS_OUT_DIR)/ej1: $(BENCH_OUT_DIR)/ej1
	tools/grapher.py -i $< -o $@-mnp.png \
		-x "M*N*P" \
		-t "Tiempo de ejecución en funcion del producto de las tres variables" \
		--mode simple "*<500 *<500 *<100" \
		-e 0 -E 'Tiempo de ejecución' \
		-L 'x * 7.27e-8' 'n'
	tools/grapher.py -i $< -o $@-mn60.png \
		-x "M*N" \
		-t "Tiempo de ejecución en funcion del tamaño del mapa. P = 60" \
		--mode simple "* * 10" \
		-e 0 -E 'Tiempo de ejecución' \
		-L 'x * 8.4e-7' 'n'
	tools/grapher.py -i $< -o $@-p.png \
		-x "P" \
		-t "Tiempo de ejecución en funcion del P máximo. M=N=500" \
		--mode simple "500 500 +<251" \
		-e 0 -E 'Tiempo de ejecución' \
		-L 'x * 0.019' 'n'

$(GRAPHS_OUT_DIR)/ej2: $(BENCH_OUT_DIR)/ej2
	tools/grapher.py -i $< -o $@-width.png \
		-x "C" \
		-t "Tiempo de ejecución en funcion del ancho del mapa. F = 100" \
		--mode simple "+ 100"
	tools/grapher.py -i $< -o $@-height.png \
		-x "F" \
		-t "Tiempo de ejecución en funcion del alto del mapa. C = 100" \
		--mode simple "100 +"
	tools/grapher.py -i $< -o $@-size.png \
		-x "F*C" \
		-t "Tiempo de ejecución en funcion del tamaño del mapa" \
		--mode prod

$(GRAPHS_OUT_DIR)/ej3: $(BENCH_OUT_DIR)/ej3
	tools/grapher.py -i $< -o $@-completo.png \
		-x "Nodos" \
		-t "Tiempo de ejecución en un grafo completo" \
		-e 10 11 -E 'Hay solución' 'No hay solución' \
		-L 'x**2 * 5e-9' 'n^2'
	tools/grapher.py -i $< -o $@-random.png \
		-x "Nodos" \
		-t "Tiempo de ejecución en un grafo random" \
		-e 20 21 -E 'Hay solución' 'No hay solución' \
		-L 'x**2 * 4.2e-9' 'n^2'
	tools/grapher.py -i $< -o $@-ciclo.png \
		-x "Nodos" \
		-t "Tiempo de ejecución en un grafo ciclo" \
		-e 30 31 -E 'Hay solución' 'No hay solución'
	tools/grapher.py -i $< -o $@-grilla.png \
		-x "Nodos" \
		-t "Tiempo de ejecución en un grafo grilla" \
		-e 40 41 -E 'Hay solución' 'No hay solución'
	tools/grapher.py -i $< -o $@-all.png \
		-x "Nodos" \
		-t "Tiempo de ejecución en un grafo" \
		-m simple '+<20001' \
		-e 10 11 20 21 30 31 40 41 \
		-E 'Grafo completo' 'Grafo completo sin solución' \
		   'Grafo random' 'Grafo random sin solución' \
		   'Grafo ciclo' 'Grafo ciclo sin solución' \
		   'Grafo grilla' 'Grafo grilla sin solución' \
		-L 'x**2 * 5e-9' 'n^2'

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

