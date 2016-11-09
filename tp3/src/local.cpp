#include <algoritmos.h>
#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 2;

enum tipo_local_t {
    tipo_dos_opt = 0,
    tipo_swap = 1,
    tipo_swap_min = 2,
};

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;
extern string generatorName;

tipo_local_t tipo_local = tipo_dos_opt;
string tipo_local_name = "dos_opt";

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

    // Inicializamos la solución con un greedy
    greedy_omNomNom(orden, true);

    switch (tipo_local) {
        case tipo_dos_opt:
            tie(d, k) = local_dos_opt(orden, verbose, 0);
            break;
        case tipo_swap:
            tie(d, k) = local_swap(orden, verbose, 0);
            break;
        case tipo_swap_min:
            tie(d, k) = local_swap_min(orden, verbose, 0);
    }

    lastResult = d;

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10 + tipo_local;
}

void prob_extra_info(std::ostream& os) {
    os << lastResult << ",local," << tipo_local_name;
}

int setTipo(const vector<string>& s) {
    tipo_local_name = s[0];
    if (s[0] == "dos_opt") {
        tipo_local = tipo_dos_opt;
    } else if (s[0] == "swap_min") {
        tipo_local = tipo_swap_min;
    } else if (s[0] == "swap") {
        tipo_local = tipo_swap;
    } else {
        cerr << s[0] << " no es un tipo válido" << endl;
        return 1;
    }
    return 0;
}

int setVerbose(__attribute__((unused)) const vector<string>& s) {
    verbose = true;
    return 0;
}

vector<Option> prob_custom_options() {
    return {{'t', "tipo", 1, false, &setTipo, "<tipo>",
             "Seleccionar el tipo de busqueda local a realizar.\n"
             "Opciones: dos_opt, swap, swap_min. Default=dos_opt"},
            {'v', "verbose", 0, false, &setVerbose, "",
             "Seleccionar el tipo de poda a usar."}};
}
