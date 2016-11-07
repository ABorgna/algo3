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

enum tipo_greedy_t {
    tipo_closest = 0,
    tipo_furthest = 1,
};

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

tipo_greedy_t tipo_greedy = tipo_closest;
string tipo_greedy_name = "closest";

double lastResult = 0;

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    vector<int64_t> orden;

    tie(d, k) = greedy_omNomNom(orden, tipo_greedy == tipo_closest);

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

void prob_extra_info(std::ostream& os) {
    os << lastResult << ",greedy," << tipo_greedy_name;
}

int setTipo(const vector<string>& s) {
    tipo_greedy_name = s[0];
    if (s[0] == "closest") {
        tipo_greedy = tipo_closest;
    } else if (s[0] == "furthest") {
        tipo_greedy = tipo_furthest;
    } else {
        cerr << s[0] << " no es un tipo válido" << endl;
        return 1;
    }
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'t', "tipo", 1, false, &setTipo, "<tipo>",
             "Seleccionar el algoritmo a utilizar.\n"
             "Opciones: closest, furthest. Default=closest"}};
}
