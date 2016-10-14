
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
	clean tar informe help dirs benchmark graphs \
    $(BENCH_OUT_DIR)/ej3-extra1 $(BENCH_OUT_DIR)/ej3-extra2 \
	$(BENCH_OUT_DIR)/ej3-extra3 \
    $(BENCH_OUT_DIR)/ej3-extra1c $(BENCH_OUT_DIR)/ej3-extra2c \
	$(BENCH_OUT_DIR)/ej3-extra3c \
	$(BENCH_OUT_DIR)/ej3-extraMC

all: dirs $(EJERCICIOS)

tar: $(ENTREGABLE)

dirs: $(DIRS)

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

$(BENCHMARKS) : %-bench : $(BENCH_OUT_DIR)/%

ej3-bench-extra2: $(BENCH_OUT_DIR)/ej3-extra2

ej3-bench-extra3: $(BENCH_OUT_DIR)/ej3-extra3

$(BENCH_OUT_DIR)/ej1: ej1
	build/ej1 -a 0:10 0:10 2 1 -A 0.1 -c $(call CHECKPOINT,$@) -o $@

EJ2_BENCH_FN = build/ej2 -a $(2)0:$(2)00:$(2) 4 1 -A 0.1 -o $(1) -c $(call CHECKPOINT,$(1))
$(BENCH_OUT_DIR)/ej2: ej2
	build/ej2 -a 1:100:1 4 1 -A 0.1 -o $@-02 -c $(call CHECKPOINT,$@-02)
	$(call EJ2_BENCH_FN,$@-03,10)
	$(call EJ2_BENCH_FN,$@-04,100)
	$(call EJ2_BENCH_FN,$@-05,1000)
	$(call EJ2_BENCH_FN,$@-06,10000)
	$(call EJ2_BENCH_FN,$@-07,100000)
	$(call EJ2_BENCH_FN,$@-08,1000000)
	$(call EJ2_BENCH_FN,$@-09,10000000)
	$(call EJ2_BENCH_FN,$@-10,100000000)
	$(call EJ2_BENCH_FN,$@-11,1000000000)
	$(call EJ2_BENCH_FN,$@-12,10000000000)
	$(call EJ2_BENCH_FN,$@-13,100000000000)
	$(call EJ2_BENCH_FN,$@-14,1000000000000)
	$(call EJ2_BENCH_FN,$@-15,10000000000000)
	$(call EJ2_BENCH_FN,$@-16,100000000000000)
	$(call EJ2_BENCH_FN,$@-17,1000000000000000)
	$(call EJ2_BENCH_FN,$@-18,10000000000000000)
	$(call EJ2_BENCH_FN,$@-19,100000000000000000)
	cat $@-* > $@
	build/ej2 -a 0:39 4 1 -A 0.1 -G unos -o $@-unos
	build/ej2 -a 0:39 4 1 -A 0.1 -G carry -o $@-carry
	build/ej2 -a 0:39 4 1 -A 0.1 -G random -o $@-rnd
	build/ej2 -a 0:39 4 1 -A 0.1 -G potencia -o $@-pot

$(BENCH_OUT_DIR)/ej3: ej3
	@echo "k0=50, k1=0, k2=0, C=1-5000"
	#build/ej3 -a 50 0 0 1:5000:2 10 4 -A 0.01 -c $(call CHECKPOINT,$@-c1) -o $@-c1
	@echo "k0=17, k1=17, k2=17, C=1-100"
	#build/ej3 -a 17 17 17 1:100 10 10 -A 0.01 -c $(call CHECKPOINT,$@-c3) -o $@-c3
	@echo "k0=1-5000, k1=0, k2=0, C=50"
	build/ej3 -a 1:500:2 0 0 50 10 4 -A 0.01 -c $(call CHECKPOINT,$@-k1) -o $@-k1
	@echo "k0=1-100, k1=1-100, k2=0, C=50"
	build/ej3 -a 1:100 0:100 0 50 10 4 -A 0.01 -c $(call CHECKPOINT,$@-k2) -o $@-k2
	@echo "k0=1-20, k1=1-20, k2=1-20, C=50"
	build/ej3 -a 1:20 1:20 1:20 50 10 4 -A 0.01 -c $(call CHECKPOINT,$@-k3) -o $@-k3
	cat $@-* > $@

$(BENCH_OUT_DIR)/ej3-extra1c: ej3
	build/ej3 -a 50 0 0 1:100000000000000:10000 10 1 -A 0.01 -c $(call CHECKPOINT,$@) -o $@ ;\

$(BENCH_OUT_DIR)/ej3-extra2c: ej3
	build/ej3 -a 25 25 0 1:100000000000000:100 10 1 -A 0.01 -c $(call CHECKPOINT,$@) -o $@ ;\

$(BENCH_OUT_DIR)/ej3-extra3c: ej3
	build/ej3 -a 17 17 17 1:100000000000000:10 10 1 -A 0.01 -c $(call CHECKPOINT,$@) -o $@ ;\

$(BENCH_OUT_DIR)/ej3-extra1: ej3
	build/ej3 -a 1:100000000000000:10 0 0 50 10 1 -A 0.01 -o $@ ;\

$(BENCH_OUT_DIR)/ej3-extraMC: ej3
	i=1; c=10; while true; do \
		build/ej3 -a "$$i" 0 0 "$$c" 10 1 -A 0.01 -o $@ ;\
		i=$$(($$i + 100)); \
		c=$$(($$c + 100)); \
		done

$(BENCH_OUT_DIR)/ej3-extra2: ej3
	i=1; while true; do \
		build/ej3 -a "$$i" "$$i" 0 50 10 1 -A 0.01 -o $@ ;\
		i=$$(($$i + 1)); \
		done

$(BENCH_OUT_DIR)/ej3-extra3: ej3
	i=1; while true; do \
		build/ej3 -a "$$i" "$$i" "$$i" 50 10 1 -A 0.01 -o $@ ;\
		i=$$(($$i + 1)); \
		done

$(GRAPHS) : %-graph : $(GRAPHS_OUT_DIR)/%

$(GRAPHS_OUT_DIR)/ej1: $(BENCH_OUT_DIR)/ej1
	tools/grapher.py -i $< -o $@.png --yscale log --colorize \
		-x "Cantidad de gente" \
		-t "Tiempo de ejecución respecto a la cantidad de arqueologos y caníbales" \
		-L '2**x * x**3 * 4e-8 + 2e-6' '2^(N+M) * (N^4+M^4)'
	tools/grapher.py -i $< -o $@-trim.png --yscale log -e 0 \
		-x "Cantidad de gente" \
		-t "Tiempo de ejecución respecto a la cantidad de arqueologos y caníbales, cuando existe una solución" \
		-L '2**x * x**3 * 4e-8 + 1e-5' '2^(N+M) * (N^4+M^4)'

$(GRAPHS_OUT_DIR)/ej2: $(BENCH_OUT_DIR)/ej2
	tools/grapher.py -i $< -o $@-all.png --xscale log \
		-x "Peso de la llave" \
		-t "Tiempo de ejecucion respecto al peso de la llave" \
		-e 0 \
		-L 'log(x) * 9e-8 + 7e-7 if x!=0 else 0' 'log(n)'
	tools/grapher.py -i $< -o $@-comp.png --colorize \
		-x "Cantidad de dígitos de P en base 3" \
		-t "Tiempo de ejecucion respecto al peso de la llave" \
		-e 1 2 3 4 -E 'Primer dígito en 2 y el resto en 1 (acarreo continuo)' 'Todos los dígitos en 1' 'Dígitos aleatorios' 'Potencia de 3' \

$(GRAPHS_OUT_DIR)/ej3: $(BENCH_OUT_DIR)/ej3
	tools/grapher.py -i $< -o $@-c1.png -x "Cantidad de objetos" \
		-t "Una mochila, k0=50" --mode simple "50 0 0 +>100000" \
		-L 'x * 1.45e-6 + 0.1' 'n'
	tools/grapher.py -i $< -o $@-c2.png -x "Cantidad de objetos" \
		-t "Dos mochilas, k0=k1=25" --mode simple "25 25 0 +>100" \
		-L 'x * 2.81e-5' 'n'
	tools/grapher.py -i $< -o $@-c3.png -x "Cantidad de objetos" \
		-t "Tres mochilas, k0=k1=k2=17" --mode simple "17 17 17 +>100" \
		-L 'x * 1.4e-4' 'n'
	tools/grapher.py -i $< -o $@-k1.png -x "Capacidad de la mochila" \
		-t "Una mochila, c=50" --mode simple "* 0 0 50" \
		-L 'x * 3.4e-6' 'n'
	tools/grapher.py -i $< -o $@-k2.png -x "Productoria de las capacidades" \
		-t "Dos mochilas, k0=k1, c=50" --mode simple "* *>0 0 50" \
		-L 'x * 3.85e-6' 'n'
	tools/grapher.py -i $< -o $@-k2a.png -x "Capacidad de cada mochila" \
		-t "Dos mochilas de misma capacidad, k0=k1, c=50" \
		--mode simple "+ >0 0 50" \
		-L 'x**2 * 4e-6' 'n^2'
	tools/grapher.py -i $< -o $@-k3.png -x "Productoria de las capacidades" \
		-t "Tres mochilas, k0=k1=k2, c=50" \
		--mode simple "* *>0 *>0 50" \
		-L 'x * 1.57e-6' 'n'
	tools/grapher.py -i $< -o $@-k3a.png -x "Capacidad de cada mochila" \
		-t "Tres mochilas, k0=k1=k2, c=50" \
		--mode simple "+ >0 >0 50" \
		-L 'x**3 * 1.56e-6' 'n^3'
	tools/grapher.py -i $<-extraMC -o $@-mc.png \
		-x "Capacidad de la mochila * cantidad de objetos" \
		-t "Una mochila, k0 = c" \
		--mode simple "* 0 0 *" \
		-L 'x * 3.5e-8' 'n'

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
