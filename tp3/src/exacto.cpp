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
const uint64_t N_HEURISTICA = 0;

enum poda_t {
    poda_none = 0,
};

extern uint64_t ngyms, nstops, bagSize;
extern PokeGraph graph;

poda_t poda = poda_none;

vector<uint64_t> orden;

pair<double, uint64_t> bruteforce() {
    // Inicializar la combinacion
    // Suponemos que siempre hay solucion
    orden.resize(ngyms + nstops);
    iota(orden.begin(), orden.end(), 0);

    double mejorDist = numeric_limits<double>::infinity();
    uint64_t mejorComb = 0;
    uint64_t comb = 0;
    do {
        if (esCaminoValido(orden, bagSize, graph)) {
            double dist = distanciaCamino(orden, graph);
            if (dist < mejorDist) {

                cerr << "comb";
                for(auto i : orden) cerr << " " << i;
                cerr << endl;

                mejorDist = dist;
                mejorComb = comb;
            }
        }
        comb++;
    } while (next_permutation(orden.begin(), orden.end()));

    // Hacemos rollback hasta la mejor permutacion
    // TODO: es mejor hacer rollback o guardarse la combinacion?
    while (comb > mejorComb) {
        comb--;
        prev_permutation(orden.begin(), orden.end());
    }

    // Recortamos las paradas del final
    auto lastGym = find_if(orden.rbegin(), orden.rend(),
                           [](uint64_t i) { return graph.isGym(i); });
    orden.resize(orden.size() - distance(orden.rbegin(), lastGym));

    return {mejorDist, orden.size()};
}

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    switch (poda) {
        case poda_none:
            tie(d, k) = bruteforce();
            break;
    }

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 10 + poda;
}

int setPrune(const vector<string>& s) {
    if (s[0] == "none") {
        poda = poda_none;
    } else {
        cerr << s[0] << " no es una poda válida" << endl;
        return 1;
    }
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'p', "poda", 1, false, &setPrune, "<poda>",
             "Seleccionar el tipo de poda a usar.\n"
             "Opciones: none. Default=none"}};
}
