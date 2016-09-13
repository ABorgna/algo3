#include <dungeon.h>
#include <interfaz.h>

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

typedef struct point_t {
    int64_t x;
    int64_t y;

    bool operator==(const struct point_t &o) const {
        return tie(x, y) == tie(o.x, o.y);
    }
} point;

int64_t n, m, pmax;
vector<vector<bool>> mapa;  // True == pared
point origen;
point destino;

void agregarCandidato(queue<pair<point, int64_t>> &candidatos,
                      vector<vector<vector<int64_t>>> &dist, point v, int64_t p,
                      int64_t d);

int64_t bfs() {
    vector<vector<vector<int64_t>>> dist(
        m, vector<vector<int64_t>>(n, vector<int64_t>(pmax + 1, -1)));

    queue<pair<point, int64_t>> candidatos;

    // Inicializar el primer lugar
    dist[origen.y][origen.x][0] = 0;
    candidatos.emplace(origen, 0);

    while (!candidatos.empty()) {
        auto cand = candidatos.front();
        candidatos.pop();
        point u = cand.first;
        int64_t p = cand.second;
        int64_t d = dist[u.y][u.x][p];

        if (u == destino) {
            return d;
        }

        // Agregamos los vecinos, si no estan visitados
        agregarCandidato(candidatos, dist, {u.x - 1, u.y}, p, d);
        agregarCandidato(candidatos, dist, {u.x + 1, u.y}, p, d);
        agregarCandidato(candidatos, dist, {u.x, u.y - 1}, p, d);
        agregarCandidato(candidatos, dist, {u.x, u.y + 1}, p, d);
    }

    return -1;
}

void agregarCandidato(queue<pair<point, int64_t>> &candidatos,
                      vector<vector<vector<int64_t>>> &dist, point v, int64_t p,
                      int64_t d) {
    // Nunca agregamos los bordes del mapa
    if (v.x <= 0 or v.y <= 0 or v.x >= n - 1 or v.y >= m - 1)
        return;

    // Si hay una pared en v nos cuesta una demolicion mas llegar
    if (mapa[v.y][v.x]) {
        p++;
        if (p > pmax)
            return;
    }

    // Si ya está visitado no lo agregamos
    if (dist[v.y][v.x][p] != -1)
        return;

    // Lo agregamos a la cola
    candidatos.emplace(v, p);
    dist[v.y][v.x][p] = d + 1;
}

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {
    is >> m >> n >> pmax;

    mapa = vector<vector<bool>>(m, vector<bool>(n, false));

    for (int64_t y = 0; y < m; y++) {
        for (int64_t x = 0; x < n; x++) {
            char celda;
            is >> celda;

            switch (celda) {
                case 'o':
                    origen = {x, y};
                    break;
                case 'x':
                    destino = {x, y};
                    break;
                case '#':
                    mapa[y][x] = true;
                    break;
            }
        }
    }
}

int prob_solve(std::ostream &os) {
    int64_t res = bfs();

    os << res << endl;

    return res >= 0 ? 0 : -1;
}

void prob_reload() {}

vector<uint64_t> prob_vars() {
    return {(uint64_t)n, (uint64_t)m, (uint64_t)pmax};
}

void prob_print_input(std::ostream &os) {
    os << m << " " << n << " " << pmax << endl;

    for (int64_t y = 0; y < m; y++) {
        for (int64_t x = 0; x < n; x++) {
            if (origen.x == x and origen.y == y) {
                cout << 'o';
            } else if (destino.x == x and destino.y == y) {
                cout << 'x';
            } else {
                cout << (mapa[y][x] ? '#' : '.');
            }
        }
        cout << endl;
    }
}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {
    n = v[0];
    m = v[1];
    pmax = v[2];

    generateDungeon(n, m, 0.6, mapa);

    int64_t x, y;

    x = rnd(1, n - 2);
    y = rnd(1, m - 2);
    mapa[y][x] = false;
    origen = {x, y};

    x = rnd(1, n - 2);
    y = rnd(1, m - 2);
    mapa[y][x] = false;
    destino = {x, y};
}

vector<Generator> prob_generators() { return {{"random", &generator_random}}; }
