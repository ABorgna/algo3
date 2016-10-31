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
const uint64_t N_HEURISTICA = 1;

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

vector<int64_t> orden;

double lastResult = 0;

pair<double, uint64_t> omNomNom() {
    // Inicializar la combinacion
    // Suponemos que siempre hay solucion
    orden.resize(ngyms + nstops);

    // Simple greedy is best greedy
    iota(orden.begin(), orden.end(), 0);

    double dist = distanciaCamino(orden.begin(), orden.end(), graph);

    // Recortamos las paradas del final
    auto lastGym = find_if(orden.rbegin(), orden.rend(),
                           [](uint64_t i) { return graph.isGym(i); });
    orden.resize(orden.size() - distance(orden.rbegin(), lastGym));

    return {dist, orden.size()};
}

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    tie(d, k) = omNomNom();

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

void prob_extra_info(std::ostream& os) { os << lastResult; }

vector<Option> prob_custom_options() { return {}; }
