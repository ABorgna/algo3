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
vector<bool> visitados;

int reventarDaistrah() {
    visitados = vector<bool>(n, false);
    vector<tiempo> tiempoMin(n, -1);
    padre = vector<estacion>(n);

    tiempoMin[0] = 0;
    padre[0] = -1;

    while (true) {
        bool first = true;
        estacion est_actual = -1;
        for (int j = 0; j < (int64_t) n; j++) {
            if (!visitados[j]) {
                if (first) {
                    first = false;
                    est_actual = j;
                } else if (tiempoMin[j] >= 0 && tiempoMin[j] < tiempoMin[est_actual])
                    est_actual = j;
            }
        }

        if (visitados[est_actual])
            break;

        tiempo t_actual = tiempoMin[est_actual];
        visitados[est_actual] = true;

        if (est_actual == (int64_t) n - 1) {
            return t_actual;
        }

        for (auto a : adj[est_actual]) {
            estacion vecino = a.first;
            tiempo nuevo = t_actual + a.second;

            tiempo anterior = tiempoMin[vecino];

            if (anterior >= 0 && nuevo >= anterior)
                continue;

            tiempoMin[vecino] = nuevo;
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

    adj = vector<vector<pair<estacion, tiempo>>>(
        n, vector<pair<estacion, tiempo>>());

    for (int i = 0; i < m; i++) {
        estacion desde, hasta;
        tiempo peso;
        is >> desde >> hasta >> peso;
        adj[desde - 1].push_back({hasta - 1, peso});
    }
}

int prob_solve(std::ostream &os) {
    int res = reventarDaistrah();

    os << res << endl;

    if (res != -1) {
        vector<estacion> estaciones;
        estacion pred = n - 1;

        while (pred != -1) {
            estaciones.insert(estaciones.begin(), pred);
            pred = padre[pred];
        }

        os << estaciones.size() << endl;

        for (uint64_t i = 0; i < estaciones.size(); i++) {
            if (i)
                os << " ";
            os << estaciones[i] + 1;
        }
        os << endl;
    }

    return res;
}

void prob_reload() {}

vector<uint64_t> prob_vars() {
    return {n};
}

void prob_print_input(std::ostream &os) {
    uint64_t m = 0;
    for (auto &v : adj)
        m += v.size();

    os << n << " " << m << endl;

    for (uint64_t a=0 ; a<adj.size(); a++) {
        for(auto &v : adj[a]) {
            os << a << " " << v.first << " " << v.second << endl;
        }
    }
}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {
    n = v[0];

    adj = vector<vector<pair<estacion, tiempo>>>(n);
    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < n; j++) {
            if (i == j or rnd(0, 1))
                continue;
            adj[i].push_back({j, rnd(1, 1000)});
        }
    }
}

void generator_completo(const std::vector<uint64_t> &v) {
    n = v[0];

    adj = vector<vector<pair<estacion, tiempo>>>(n);
    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < n; j++) {
            if (i == j)
                continue;
            adj[i].push_back({j, rnd(1, 1000)});
        }
    }
}

vector<Generator> prob_generators() {
    return {{"completo", generator_completo}, {"random", generator_random}};
}
