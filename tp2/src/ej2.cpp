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

typedef struct point_t {
    int64_t x;
    int64_t y;

    bool operator==(const struct point_t &o) const {
        return tie(x, y) == tie(o.x, o.y);
    }
    bool operator!=(const struct point_t &o) const {
        return tie(x, y) != tie(o.x, o.y);
    }
    bool operator<(const struct point_t &o) const {
        return tie(x, y) < tie(o.x, o.y);
    }
} point;

typedef struct arista_t {
    point p;
    point q;
    int64_t peso;

    bool operator==(const struct arista_t &o) const {
        return tie(p, q, peso) == tie(o.p, o.q, o.peso) or
               tie(p, q, peso) == tie(o.q, o.p, o.peso) ;
    }
    bool operator<(const struct arista_t &o) const {
        return tie(peso, p, q) < tie(o.peso, o.p, o.q);
    }
} arista;

vector<vector<int64_t>> altura_uf;
vector<vector<point>> padre_uf;

void init_uf(){
    altura_uf = vector<vector<int64_t>>(m,vector<int64_t>(n,1));

    padre_uf.resize(m);
    for(int64_t y = 0; y < m ; y++) {
        padre_uf[y].resize(n);
        for(int64_t x = 0; x < n ; x++){
            padre_uf[y][x] = {x,y};
        }
    }
}

point find_uf(point p){
    if (padre_uf[p.y][p.x] != p)
        padre_uf[p.y][p.x] = find_uf(padre_uf[p.y][p.x]);
    return padre_uf[p.y][p.x];
}

void union_uf(point p, point q){
    p = find_uf(p);
    q = find_uf(q);
    if (altura_uf[p.y][p.x] < altura_uf[q.y][q.x])
        padre_uf[p.y][p.x] = q;
    else
        padre_uf[q.y][q.x] = p;
    if (altura_uf[p.y][p.x] == altura_uf[q.y][q.x])
        altura_uf[p.y][p.x]++;
}

int64_t kruskal(){
    init_uf();
    set<arista> candidatos;
    int64_t res = 0;
    int64_t componentes_conexas = 0;

    //Agregar las aristas
    for (int64_t y = 1 ; y < m - 1 ; y++){
        for (int64_t x = 1 ; x < n - 1 ; x++){
            if (mapa[y][x] == 0){
                if (mapa[y][x+1] == 0)
                    candidatos.insert({{x,y}, {x+1,y}, 0});
                if (mapa[y+1][x] == 0)
                    candidatos.insert({{x,y}, {x,y+1}, 0});
                componentes_conexas++;
            }
            if (mapa[y][x] > 0){
                point p ={0,0} ,q;
                if(mapa[y-1][x] == 0) {
                    q = p;
                    p = {x,y-1};
                } 
                if(mapa[y+1][x] == 0) {
                    q = p;
                    p = {x,y+1};
                } 
                if(mapa[y][x-1] == 0) {
                    q = p;
                    p = {x-1,y};
                } 
                if(mapa[y][x+1] == 0) {
                    q = p;
                    p = {x+1,y};
                }
                candidatos.insert({p, q, mapa[y][x]});
            }
        }
    }

    //Hacer Kruskal
    for(auto &ar : candidatos) {
        if (find_uf(ar.p) == find_uf(ar.q))
            continue;
        union_uf(ar.p, ar.q);
        res += ar.peso;
        componentes_conexas--;

        if(componentes_conexas == 1)
            break;
    }

    return componentes_conexas == 1 ? res : -1;
}

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
    int64_t res = kruskal();

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

void generator_conexo(const std::vector<uint64_t> &v) {
    m = v[0];
    n = v[1];

    generateDungeon<int64_t>(m, n, 0.7, mapa, -1, 0);

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

void generator_no_conexo(const std::vector<uint64_t> &v) {
    generator_conexo(v);

    // Asignamos valores a las paredes
    for (int64_t y = 1; y < m - 1; y++)
        for (int64_t x = 1; x < n - 1; x++)
            if (mapa[y][x] > 0 and rnd(0,1))
                mapa[y][x] = -1;
}

vector<Generator> prob_generators() {
    return {{"conexo", &generator_conexo},
            {"no_conexo", &generator_no_conexo}};
}
