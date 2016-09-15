#include "./include/interfaz.h"

#include <math.h>
#include <stdint.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

typedef int64_t estacion;
typedef int64_t tiempo;

int64_t n;
vector<vector<pair<estacion, tiempo>>> adj;
vector<estacion> padre;  // estacion -> <tiempo en llegar / predecesora>

int reventarDaistrah() {
    vector<tiempo> tiempoMin(n,-1);
    set<pair<tiempo, estacion>> candidatos;
    padre = vector<estacion>(n);

    tiempoMin[0] = 0;
    padre[0] = -1;
    candidatos.insert({0, 0});

    while (!candidatos.empty()) {
        auto it = candidatos.begin();
        tiempo t_actual = it->first;
        estacion est_actual = it->second;
        candidatos.erase(it);

        if (est_actual == n - 1) {
            return t_actual;
        }

        for (auto a : adj[est_actual]) {
            estacion vecino = a.first;
            tiempo peso = a.second;

            tiempo anterior = tiempoMin[vecino];

            if (anterior >= 0) {
                if(peso >= anterior)
                    continue;

                // Been there, done that
                candidatos.erase({anterior, vecino});
            }

            tiempoMin[vecino] = peso;
            candidatos.insert({peso, vecino});
        }
    }

    return -1;
}

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {}

int prob_solve(std::ostream &os) {}

void prob_reload() {}

vector<uint64_t> prob_vars() {}

void prob_print_input(std::ostream &os) {}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {}

vector<Generator> prob_generators() { return {}; }
