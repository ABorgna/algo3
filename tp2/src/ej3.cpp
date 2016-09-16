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

uint64_t n;
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

        if (est_actual == (int64_t) n - 1) {
            return t_actual;
        } 

        for (auto a : adj[est_actual]) {
            estacion vecino = a.first;
            tiempo nuevo = t_actual + a.second;

            tiempo anterior = tiempoMin[vecino];

            if (anterior >= 0) {
                if(nuevo >= anterior)
                    continue;

                // Been there, done that
                candidatos.erase({anterior, vecino});
            }

            tiempoMin[vecino] = nuevo;
            candidatos.insert({nuevo, vecino});
            padre[vecino] = est_actual;
        }
    }

    return -1;
}

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {
    int64_t m;
    is >> n >> m;

    adj = vector<vector<pair<estacion, tiempo>>>(n, vector<pair<estacion, tiempo>>());


    for (int i = 0; i < m; i++) {
        estacion desde, hasta;
        tiempo peso;
        is >> desde >> hasta >> peso;
        adj[desde-1].push_back({hasta-1, peso});
    }
}

int prob_solve(std::ostream &os) {
    int res = reventarDaistrah();

    os << res << endl;

    if (res != -1) {
        vector<estacion> estaciones;
        estacion pred = n-1;

        while (pred != -1) {
            estaciones.insert(estaciones.begin(), pred);
            pred = padre[pred];
        }

        os << estaciones.size() << endl;

        for (uint64_t i = 0; i < estaciones.size(); i++) {
            if (i) os << " ";
            os << estaciones[i]+1;
        }
        os << endl;
    }

    return res;
}

void prob_reload() {}

vector<uint64_t> prob_vars() {
    uint64_t m = 0;
    for (uint64_t i = 0; i < adj.size(); i++) {
        m += adj[i].size();
    }
    return {n, m};
}

void prob_print_input(std::ostream &os) {
    os << n;

    uint64_t m = 0;
    for (uint64_t i = 0; i < adj.size(); i++) {
        m += adj[i].size();
    }

    os << m << endl;
}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {}

void generator_completo(const std::vector<uint64_t> &v) {
    n = v[0];

    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < n; j++) {
            if (i == j) continue;
            adj[j].push_back({i, rnd(1, 1000)});
            adj[i].push_back({j, rnd(1, 1000)});
        }
    }
}

vector<Generator> prob_generators() { return {{"random", generator_completo}}; }
