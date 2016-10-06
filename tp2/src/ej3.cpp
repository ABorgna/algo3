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
int64_t generator = 0;
vector<vector<pair<estacion, tiempo>>> adj;
vector<estacion> padre;  // estacion -> <tiempo en llegar / predecesora>

int reventarDaistrah() {
    vector<bool> visitado = vector<bool>(n, false);
    vector<tiempo> tiempoMin(n, INT64_MAX);
    padre = vector<estacion>(n);

    tiempoMin[0] = 0;
    padre[0] = -1;

    while (true) {
        tiempo act_t = INT64_MAX;
        estacion est_t = -1;
        for (int j = 0; j < n; j++) {
            if (!visitado[j] and tiempoMin[j] < act_t) {
                act_t = tiempoMin[j];
                est_t = j;
            }
        }
        if (est_t == -1)
            break;

        visitado[est_t] = true;

        if (est_t == n - 1)
            return act_t;

        for (auto a : adj[est_t]) {
            estacion vecino = a.first;
            tiempo nuevo = act_t + a.second;
            tiempo anterior = tiempoMin[vecino];

            if (nuevo >= anterior)
                continue;

            tiempoMin[vecino] = nuevo;
            padre[vecino] = est_t;
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

    return (res == -1 ? 1 : 0) + generator * 10;
}

void prob_reload() {}

vector<uint64_t> prob_vars() { return {(uint64_t)n}; }

void prob_print_input(std::ostream &os) {
    int64_t m = 0;
    for (auto &v : adj) m += v.size();

    os << n << " " << m << endl;

    for (uint64_t a = 0; a < adj.size(); a++) {
        for (auto &v : adj[a]) {
            os << a << " " << v.first << " " << v.second << endl;
        }
    }
}

vector<Option> prob_custom_options() { return {}; }

void generator_completo(const std::vector<uint64_t> &v) {
    // n*(n-1) aristas
    generator = 1;
    n = v[0];

    adj = vector<vector<pair<estacion, tiempo>>>(n);
    for (int64_t i = 0; i < n; i++) {
        adj[i].resize(n - 1);
        for (int64_t j = 0; j < i; j++) {
            adj[i][j] = {j, rnd(1, 1000)};
        }
        for (int64_t j = i + 1; j < n; j++) {
            adj[i][j - 1] = {j, rnd(1, 1000)};
        }
    }
}

void generator_random(const std::vector<uint64_t> &v) {
    // Aprox n*(n-1)/2 aristas
    generator = 2;
    n = v[0];

    adj = vector<vector<pair<estacion, tiempo>>>(n);
    for (int64_t i = 0; i < n; i++) {
        adj[i].reserve(n);
        for (int64_t j = 0; j < n; j++) {
            if (i == j or rnd(0, 1))
                continue;
            adj[i].push_back({j, rnd(1, 1000)});
        }
    }
}

void generator_ciclo(const std::vector<uint64_t> &v) {
    // n aristas
    generator = 3;
    n = v[0];

    adj = vector<vector<pair<estacion, tiempo>>>(n);
    for (int64_t i = 0; i < n - 1; i++) {
        adj[i].push_back({i + 1, rnd(1, 1000)});
    }
    adj[n - 1].push_back({0, rnd(1, 1000)});
}

void generator_grilla(const std::vector<uint64_t> &v) {
    // emula una grilla de sqrt(n) * sqrt(n),
    // aprox n*2 aristas
    //
    // *-*-*
    // | | |
    // *-*-*
    // | | |
    // *-*-*
    //      \   <- los que sobran van al palito
    //       *

    generator = 4;
    n = v[0];

    int64_t sq = sqrt(n);

    adj = vector<vector<pair<estacion, tiempo>>>(n);
    for (int64_t i = 0; i < sq * sq; i++) {
        int x = i % sq;
        int y = i / sq;

        if (x > 0)
            adj[i].push_back({i - 1, rnd(1, 1000)});
        if (x < sq - 1)
            adj[i].push_back({i + 1, rnd(1, 1000)});
        if (y > 0)
            adj[i].push_back({i - sq, rnd(1, 1000)});
        if (y < sq - 1)
            adj[i].push_back({i + sq, rnd(1, 1000)});
    }
    for (int64_t i = max((int64_t)1,sq * sq); i < n; i++) {
        adj[i-1].push_back({i, rnd(1, 1000)});
    }
}

vector<Generator> prob_generators() {
    return {{"completo", generator_completo},
            {"random", generator_random},
            {"ciclo", generator_ciclo},
            {"grilla", generator_grilla}};
}
