
GCC=g++
#CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14
CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

OUT_DIR = build
SRC_DIR = src
OBJ_DIR = obj

DIRS = $(OUT_DIR) $(OBJ_DIR)

DEPEND = makefile

EJERCICIOS = ej1 ej2 ej3
DEFAULT_MAIN = $(SRC_DIR)/ejercicio.cpp

INCLUDES = $(SRC_DIR)/include/
SOURCES = $(DEFAULT_MAIN) $(EJERCICIOS:%=src/%.cpp)
BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)
DEFAULT_MAIN_OBJ = $(DEFAULT_MAIN:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CFLAGS += -I $(INCLUDES)

ENTREGABLE=entregable.tar.gz

.PHONY: all $(EJERCICIOS) clean tar

all: $(DIRS) $(EJERCICIOS)

tar: $(ENTREGABLE)

$(ENTREGABLE): informe
	tar -czf $@ src -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : $(OBJ_DIR)/%.o $(DEFAULT_MAIN_OBJ)
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
	rm -f obj/*
	rm -f $(ENTREGABLE)

