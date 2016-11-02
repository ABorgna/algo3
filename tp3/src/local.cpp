#include <algoritmos.h>
#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 2;

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

double lastResult = 0;
bool verbose = false;

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    vector<int64_t> orden;

    tie(d, k) = local_dos_opt(orden, verbose);

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

void prob_extra_info(std::ostream& os) { os << lastResult; }

int setVerbose(__attribute__((unused)) const vector<string>& s) {
    verbose = true;
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'v', "verbose", 0, false, &setVerbose, "",
             "Seleccionar el tipo de poda a usar."}};
}
