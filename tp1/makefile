
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

# Funcion auxiliar para calcular a partir de que problema
# continuar los bechmarks
CHECKPOINT = 0`wc -l $(1) 2>/dev/null | sed 's/ .*//'`

.PHONY: all $(EJERCICIOS) $(BENCHMARKS) $(BENCHMARKS_OUT) \
	$(GRAPHS) $(GRAPHS_OUT) \
	clean tar informe help dirs benchmark graphs

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
	build/ej1 -a 0:9 0:9 -A 0.01 -c $(call CHECKPOINT,$@) -o $@

$(BENCH_OUT_DIR)/ej2: ej2
	build/ej2 -a 1:100:1 -A 0.01 -o $@-0 \
		-c $(call CHECKPOINT,$@-0)
	build/ej2 -a 100:10000:10 -A 0.01 -o $@-1 \
		-c $(call CHECKPOINT,$@-1)
	build/ej2 -a 10000:1000000:1000 -A 0.01 -o $@-2 \
		-c $(call CHECKPOINT,$@-2)
	build/ej2 -a 1000000:100000000:100000 -A 0.01 -o $@-3 \
		-c $(call CHECKPOINT,$@-3)
	build/ej2 -a 100000000:10000000000:10000000 -A 0.01 -o $@-4 \
		-c $(call CHECKPOINT,$@-4)
	build/ej2 -a 10000000000:1000000000000:1000000000 -A 0.01 -o $@-5 \
		-c $(call CHECKPOINT,$@-5)
	build/ej2 -a 1000000000000:100000000000000:100000000000 -A 0.01 -o $@-6 \
		-c $(call CHECKPOINT,$@-6)
	build/ej2 -a 100000000000000:10000000000000000:10000000000000 -A 0.01 -o $@-7 \
		-c $(call CHECKPOINT,$@-7)
	cat $@-0 > $@
	cat $@-1 >> $@
	cat $@-2 >> $@
	cat $@-3 >> $@
	cat $@-4 >> $@
	cat $@-5 >> $@
	cat $@-6 >> $@
	cat $@-7 >> $@

$(BENCH_OUT_DIR)/ej3: ej3
	@echo "k0=50, k1=0, k2=0, C=1-50"
	build/ej3 -a 50 0 0 1:50 1 10 -A 0.001 -c $(call CHECKPOINT,$@-c1) -o $@-c1
	@echo "k0=17, k1=17, k2=17, C=1-50"
	build/ej3 -a 17 17 17 1:50 1 10 -A 0.001 -c $(call CHECKPOINT,$@-c3) -o $@-c3
	@echo "k0=1-50, k1=0, k2=0, C=50"
	build/ej3 -a 1:50 0 0 50 1 10 -A 0.001 -c $(call CHECKPOINT,$@-k1) -o $@-k1
	@echo "k0=1-17, k1=0-17, k2=0-17, C=50"
	build/ej3 -a 1:17 0:17 0:17 50 1 2 -A 0.001 -c $(call CHECKPOINT,$@-full) -o $@-full
	cat $@-c1 > $@
	cat $@-c3 >> $@
	cat $@-k1 >> $@
	cat $@-full >> $@

$(GRAPHS) : %-graph : $(GRAPHS_OUT_DIR)/%

$(GRAPHS_OUT_DIR)/ej1: $(BENCH_OUT_DIR)/ej1
	tools/grapher.py -i $< -o $@ --yscale log -x "Cantidad de gente" -t "Tiempo mínimo de arqueologos y caníbales"

$(GRAPHS_OUT_DIR)/ej2: $(BENCH_OUT_DIR)/ej2
	tools/grapher.py -i $< -o $@ --xscale log -x "Peso" -t "Tiempo aoeuaoueou"

$(GRAPHS_OUT_DIR)/ej3: $(BENCH_OUT_DIR)/ej3
	tools/grapher.py -i $< -o $@-c1 -x "Cantidad de objetos" \
		-t "Una mochila, k=50" simple "50 0 0 +"
	tools/grapher.py -i $< -o $@-c3 -x "Cantidad de objetos" \
		-t "Tres mochilas, k=50" simple "17 17 17 +"
	tools/grapher.py -i $< -o $@-k1 -x "Suma de las capacidades" \
		-t "Una mochila, c=50" simple "+ 0 0 50"
	tools/grapher.py -i $< -o $@-k2 -x "Suma de las capacidades" \
		-t "Dos mochilas, c=50" simple "+ +>0 0 50"
	tools/grapher.py -i $< -o $@-k3 -x "Suma de las capacidades" \
		-t "Tres mochilas, c=50" simple "+ +>0 +>0 50"
	tools/grapher.py -i $< -o $@-kp1 -x "Productoria de las capacidades" \
		-t "Una mochila, c=50" simple "* 0 0 50"
	tools/grapher.py -i $< -o $@-kp2 -x "Productoria de las capacidades" \
		-t "Dos mochilas, c=50" simple "* *>0 0 50"
	tools/grapher.py -i $< -o $@-kp3 -x "Productoria de las capacidades" \
		-t "Tres mochilas, c=50" simple "* *>0 *>0 50"

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

