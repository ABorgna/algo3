#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>

#include <math.h>
#include <stdint.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 2;

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;

vector<int64_t> orden;

pair<double, uint64_t> searchAndDestroy() {
    // TODO: escribir algo
    // Suponemos que siempre hay solucion

    orden = vector<int64_t>(1);

    double dist = distanciaCamino(orden.begin(), orden.end(), graph);

    return {dist, orden.size()};
}

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    tie(d, k) = searchAndDestroy();

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

vector<Option> prob_custom_options() { return {}; }
