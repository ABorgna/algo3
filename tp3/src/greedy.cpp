#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>
#include <algoritmos.h>

#include <math.h>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 1;

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

double lastResult = 0;

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    vector<int64_t> orden;

    tie(d, k) = greedy_omNomNom(orden);

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

void prob_extra_info(std::ostream& os) { os << lastResult << " greedy"; }

vector<Option> prob_custom_options() { return {}; }
