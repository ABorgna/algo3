#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>
#include <option.h>
#include <generator.h>

// Esta función es llamada para que el problema interprete
//la entrada e inicialice las variables de una instancia.
void prob_load(std::istream& is);

// Esta función es llamada para resolver la instancia actual
//e imprimir la solución.
int prob_solve(std::ostream& os);

// Esta función es llamada para reinicializar variables auxiliares internas
//y permitir la resolución de la instancia como si fuera recién interpretada
void prob_reload();

// Esta función devuelve un vector con las variables principales del problema.
// Este vector se usa para comunicar las características de una instancia generada
// por el generador de instancias aleatorizado.
std::vector<uint64_t> prob_vars();

// Esta función retorna los generadores del problema
std::vector<Generator> prob_generators();

// Esta función es llamada para imprimir el input del problema.
void prob_print_input(std::ostream& os);

// Esta función devuelve un vector con las opciones custom del problema
std::vector<Option> prob_custom_options();

// Esta función devuelve números aleatorios en el rango [inicio, fin]
uint64_t rnd(uint64_t inicio, uint64_t fin);

// Esta función devuelve la diferencia entre dos tiempos
uint64_t diff(timespec start, timespec end);

