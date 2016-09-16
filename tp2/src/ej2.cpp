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

int64_t n, m;
vector<vector<int64_t>>
    mapa;  // -1 = pared dura, 0 = sala, >0 = pared destruible

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {
    is >> m >> n;

    mapa = vector<vector<int64_t>>(m, vector<int64_t>(n));

    for (int64_t y = 0; y < m; y++) {
        for (int64_t x = 0; x < n; x++) {
            char celda;
            is >> celda;

            switch (celda) {
                case '.':
                    mapa[y][x] = 0;
                    break;
                case '#':
                    mapa[y][x] = -1;
                    break;
                default:
                    mapa[y][x] = celda - '0';
                    break;
            }
        }
    }
}

int prob_solve(std::ostream &os) {
    int64_t res = 0;

    // Todo: resolver

    os << res << endl;

    return res == -1 ? -1 : 0;
}

void prob_reload() {}

vector<uint64_t> prob_vars() { return {(uint64_t)m, (uint64_t)n}; }

void prob_print_input(std::ostream &os) {
    os << m << " " << n << " " << endl;

    for (int64_t y = 0; y < m; y++) {
        for (int64_t x = 0; x < n; x++) {
            if (mapa[y][x] == 0) {
                cout << '.';
            } else if (mapa[y][x] == -1) {
                cout << '#';
            } else {
                cout << mapa[y][x];
            }
        }
        cout << endl;
    }
}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {
    m = v[0];
    n = v[1];

    generateDungeon<int64_t>(m, n, 0.6, mapa, -1, 0);

    // Asignamos valores a las paredes
    for (int64_t y = 1; y < m - 1; y++) {
        for (int64_t x = 1; x < n - 1; x++) {
            if (mapa[y][x] == 0)
                continue;

            bool n = mapa[y - 1][x] == 0;
            bool nw = mapa[y - 1][x - 1] == 0;
            bool ne = mapa[y - 1][x + 1] == 0;
            bool w = mapa[y][x - 1] == 0;
            bool e = mapa[y][x + 1] == 0;
            bool s = mapa[y + 1][x] == 0;
            bool sw = mapa[y + 1][x - 1] == 0;
            bool se = mapa[y + 1][x + 1] == 0;

            bool makeNumber = true;
            int vecinos = n + s + w + e;

            // Si una pared conecta menos de dos salas, es irrompible
            if (vecinos != 2) {
                makeNumber = false;
            } else {
                if(n and s) {
                    if(nw and sw and mapa[y][x-1] != -1)
                        makeNumber = false;
                } else if(e and w) {
                    if(nw and ne and mapa[y-1][x] != -1)
                        makeNumber = false;
                } else if(n and e) {
                    if(ne or mapa[y-1][x+1] > 0)
                        makeNumber = false;
                } else if(e and s) {
                    if(se)
                        makeNumber = false;
                } else if(s and w) {
                    if(sw)
                        makeNumber = false;
                } else if(w and n) {
                    if(nw or mapa[y-1][x-1] > 0)
                        makeNumber = false;
                }
            }

            if (makeNumber) {
                mapa[y][x] = rnd(1, 9);
            }
        }
    }
}

vector<Generator> prob_generators() { return {{"random", &generator_random}}; }
