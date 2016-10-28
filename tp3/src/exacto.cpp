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
extern int64_t generator;

poda_t poda = poda_none;

vector<int64_t> orden;

// ------------------------------------ Solución por fuerza bruta

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

// ------------------------------------ Solución por Backtracking

    // Inicializar la combinacion
    // Suponemos que siempre hay solucion

    vector<int64_t> distanciaAcumulada;
    double mejorDist;
    uint64_t mejorOrdenLen;
    vector<int64_t> mejorOrden;
    vector<int64_t> powerAcumulado;
    vector<bool> used;

void backtracking_rec(uint64_t pos, int64_t gymCounter) {

    // Calculo distancia para orden[0..pos-1]
    if (pos > 1) {
        distanciaAcumulada[pos-1] = graph.distance( orden[pos-2], orden[pos-1]) + distanciaAcumulada[pos - 2];
    }

    // Vemos si orden[0..pos-1] es solución
    if (pos > 0) {
        if (graph.isGym(orden[pos-1]))
            gymCounter++;

        if (gymCounter == ngyms) {
            // Llegamos a una posible solucion
            if(distanciaAcumulada[pos-1] < mejorDist) {
                mejorDist = distanciaAcumulada[pos-1];
                mejorOrden = orden;
                mejorOrdenLen = pos;
                return;
            }
        }
    }

    bool valido;
    powerAcumulado[pos] = pos > 0 ? powerAcumulado[pos-1] : 0;

    for (int i = 0; i < ngyms + nstops; i++) {
        orden[pos] = i;

        int64_t pow_anterior = powerAcumulado[pos];
        valido = esCaminoValido(orden.begin() + pos, orden.begin() + pos + 1, bagSize, graph, powerAcumulado[pos]);

        if (not valido or used[i]) {
            powerAcumulado[pos] = pow_anterior;
            continue;
        }

        used[i] = true;
        backtracking_rec(pos+1, gymCounter);
        powerAcumulado[pos] = pow_anterior;
        used[i] = false;
    }
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
            orden = vector<int64_t>(ngyms + nstops, -1);
            mejorDist = numeric_limits<double>::infinity();
            mejorOrden = orden;
            used = vector<bool>(ngyms + nstops, false);
            powerAcumulado = vector<int64_t>(ngyms + nstops, 0);
            distanciaAcumulada = vector<int64_t>(ngyms + nstops, 0);
            mejorOrdenLen = 0;

            backtracking_rec(0,0);
            orden = mejorOrden;
            tie(d, k) = make_pair(mejorDist, orden.size());

            break;
    }

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10 + poda;
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
