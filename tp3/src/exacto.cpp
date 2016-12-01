#include <algoritmos.h>
#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>

#include <iostream>
#include <limits>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 0;

enum poda_t {
    poda_none = 0,
    poda_backtracking = 1,
    poda_distancias = 2,
    poda_greedy = 3,
};

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

poda_t poda = poda_backtracking;
string poda_name = "backtracking";

double lastResult = 0;

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    vector<int64_t> orden;

    switch (poda) {
        case poda_none:
            tie(d, k) = exacto_bruteforce(orden);
            break;
        case poda_backtracking:
            tie(d, k) = exacto_backtracking(orden);
            break;
        case poda_distancias:
            tie(d, k) = exacto_backtracking_distancias(orden);
            break;
        case poda_greedy:
            tie(d, k) = exacto_backtracking_greedy(orden);
            break;
    }

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10 + poda;
}

void prob_extra_info(std::ostream& os) {
    os << lastResult << ",exacto," << poda_name;
}

int setPrune(const vector<string>& s) {
    poda_name = s[0];
    if (s[0] == "none") {
        poda = poda_none;
    } else if (s[0] == "backtracking") {
        poda = poda_backtracking;
    } else if (s[0] == "distancias") {
        poda = poda_distancias;
    } else if (s[0] == "greedy"){
        poda = poda_greedy;
    } else {
        cerr << s[0] << " no es una poda válida" << endl;
        return 1;
    }
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'p', "poda", 1, false, &setPrune, "<poda>",
             "Seleccionar el tipo de poda a usar.\n"
             "Opciones: none, backtracking, distancias, greedy. Default=backtracking"}};
}
