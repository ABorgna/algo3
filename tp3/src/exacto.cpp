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
    poda_backtracking = 1,
};

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;

poda_t poda = poda_none;

vector<int64_t> orden;

// Bruteforce solution

pair<double, uint64_t> bruteforce() {
    // Inicializar la combinacion
    // Suponemos que siempre hay solucion
    orden.resize(ngyms + nstops);
    iota(orden.begin(), orden.end(), 0);

    double mejorDist = numeric_limits<double>::infinity();
    uint64_t mejorComb = 0;
    uint64_t comb = 0;
    do {
        if (esCaminoValido(orden.begin(), orden.end(), bagSize, graph)) {
            double dist = distanciaCamino(orden.begin(), orden.end(), graph);
            if (dist < mejorDist) {
                mejorDist = dist;
                mejorComb = comb;
            }
        }
        comb++;
    } while (next_permutation(orden.begin(), orden.end()));

    // Hacemos rollback hasta la mejor permutacion
    // Es mas rápido que guardarse la combinacion cada vez que vemos un maximo
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

// Backtracking solution

pair<double, uint64_t> backtracking() {
    // Inicializar la combinacion
    // Suponemos que siempre hay solucion
    orden = vector<int64_t>(ngyms + nstops, -1);

    double mejorDist = numeric_limits<double>::infinity();
    auto mejorOrden = orden;
    uint64_t mejorOrdenLen = 0;

    uint64_t pos = 0;
    int64_t gymCounter = 0;
    vector<bool> used(ngyms + nstops, false);
    vector<int64_t> powerAcumulado(ngyms + nstops, 0);
    vector<int64_t> distanciaAcumulada(ngyms + nstops, 0);

    while (orden[0] < ngyms + nstops) {
        if (pos == orden.size() || orden[pos] >= ngyms + nstops) {
            pos--;
            used[orden[pos]] = false;
            if (graph.isGym(orden[pos]))
                gymCounter--;
        }

        do {
            orden[pos]++;
        } while (used[orden[pos]] and orden[pos] < ngyms + nstops);
        if (orden[pos] >= ngyms + nstops)
            continue;

        powerAcumulado[pos] = pos > 0 ? powerAcumulado[pos - 1] : 0;

        bool valido =
            esCaminoValido(orden.begin() + pos, orden.begin() + pos + 1,
                           bagSize, graph, powerAcumulado[pos]);

        if (!valido)
            continue;

        if (pos > 0) {
            distanciaAcumulada[pos] =
                graph.distance(orden[pos - 1], orden[pos]) +
                distanciaAcumulada[pos - 1];
        }

        if (!graph.isGym(orden[pos]) or gymCounter < ngyms - 1) {
            if(graph.isGym(orden[pos]))
                gymCounter++;
            used[orden[pos]] = true;
            pos++;
            orden[pos] = -1;
        } else {
            // Llegamos a una posible solucion
            if(distanciaAcumulada[pos] < mejorDist) {
                mejorDist = distanciaAcumulada[pos];
                mejorOrden = orden;
                mejorOrdenLen = pos+1;
            }
        }
    }

    orden = mejorOrden;
    orden.resize(mejorOrdenLen);
    return {mejorDist, mejorOrdenLen};
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
        case poda_backtracking:
            tie(d, k) = backtracking();
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
    } else if (s[0] == "backtracking") {
        poda = poda_backtracking;
    } else {
        cerr << s[0] << " no es una poda válida" << endl;
        return 1;
    }
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'p', "poda", 1, false, &setPrune, "<poda>",
             "Seleccionar el tipo de poda a usar.\n"
             "Opciones: none, backtracking. Default=none"}};
}
