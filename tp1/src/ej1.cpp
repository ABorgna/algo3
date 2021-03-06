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

// Hay 2^(N+M) estados posibles
typedef struct estado_t {
    // Para todo, true = del lado derecho
    vector<bool> arq;
    vector<bool> can;
    bool linterna = false;

    estado_t(){};
    estado_t(uint32_t n, uint32_t m) : arq(n), can(m) {}

    // La priority_queue requiere <
    bool operator<(const struct estado_t &o) const {
        return tie(linterna, arq, can) < tie(o.linterna, o.arq, o.can);
    }
    bool operator==(const struct estado_t &o) const {
        return tie(linterna, arq, can) == tie(o.linterna, o.arq, o.can);
    }
} estado;

vector<uint32_t> arq;
vector<uint32_t> can;

bool agregarCandidato(map<estado, uint32_t> &dist,
                      set<pair<uint32_t, estado>> &candidatos, estado &hijo,
                      uint32_t peso);

int64_t tiempoMinimo(const vector<uint32_t> &arq, const vector<uint32_t> &can) {
    // Empiezan todos del lado izquierdo
    estado inicial(arq.size(), can.size());
    estado objetivo;
    objetivo.arq = vector<bool>(arq.size(), true);
    objetivo.can = vector<bool>(can.size(), true);
    objetivo.linterna = true;

    // Distancia minima hasta el estado
    map<estado, uint32_t> dist;
    dist.insert({inicial, 0});

    // Mantenemos la lista de candidatos a explorar,
    // y vemos siempre el mas cercano primero
    set<pair<uint32_t, estado>> candidatos;
    candidatos.insert(make_pair(0, inicial));

    int64_t res = -1;
    while (!candidatos.empty()) {
        auto p = candidatos.begin();
        uint32_t costo = p->first;
        estado nodo = p->second;
        candidatos.erase(p);

        // Llegamos a la solución
        if (nodo == objetivo) {
            res = costo;
            break;
        }

        // Hacemos un population count
        uint32_t arqIzq = 0;
        uint32_t canIzq = 0;

        for (uint32_t i = 0; i < arq.size(); i++)
            if (nodo.arq[i])
                arqIzq++;
        for (uint32_t i = 0; i < can.size(); i++)
            if (nodo.can[i])
                canIzq++;

        // Checkeamos que no sea un caso invalido
        if ((arq.size() - arqIzq > 0) &&
            (arq.size() - arqIzq) < (can.size() - canIzq)) {
            continue;
        }
        if (arqIzq > 0 && arqIzq < canIzq) {
            continue;
        }

        // Generamos los pasos posibles a partir de donde estamos
        estado hijo = nodo;
        hijo.linterna = !hijo.linterna;

        for (uint32_t i = 0; i < arq.size(); i++) {
            // Checkear si está de este lado
            if (nodo.arq[i] != nodo.linterna)
                continue;

            hijo.arq[i] = !nodo.linterna;
            for (uint32_t j = i; j < arq.size(); j++) {
                if (nodo.arq[j] != nodo.linterna)
                    continue;
                hijo.arq[j] = !nodo.linterna;

                // El movimiento siempre cuesta la menor de las velocidades
                uint32_t costoExtra = max(arq[i], arq[j]);
                uint32_t distancia = costo + costoExtra;

                // Agregamos este hijo a la lista
                agregarCandidato(dist, candidatos, hijo, distancia);

                if (i != j)
                    hijo.arq[j] = nodo.linterna;
            }
            // Lo volvemos a como estaba
            hijo.arq[i] = nodo.linterna;
        }

        for (uint32_t i = 0; i < can.size(); i++) {
            // Checkear si está de este lado
            if (nodo.can[i] != nodo.linterna)
                continue;

            hijo.can[i] = !nodo.linterna;
            for (uint32_t j = i; j < can.size(); j++) {
                if (nodo.can[j] != nodo.linterna)
                    continue;
                hijo.can[j] = !nodo.linterna;

                // El movimiento siempre cuesta la menor de las velocidades
                uint32_t costoExtra = max(can[i], can[j]);
                uint32_t distancia = costo + costoExtra;

                // Agregamos este hijo a la lista
                agregarCandidato(dist, candidatos, hijo, distancia);

                if (i != j)
                    hijo.can[j] = nodo.linterna;
            }
            // Lo volvemos a como estaba
            hijo.can[i] = nodo.linterna;
        }

        for (uint32_t i = 0; i < arq.size(); i++) {
            // Checkear si está de este lado
            if (nodo.arq[i] != nodo.linterna)
                continue;

            hijo.arq[i] = !nodo.linterna;
            for (uint32_t j = 0; j < can.size(); j++) {
                if (nodo.can[j] != nodo.linterna)
                    continue;
                hijo.can[j] = !nodo.linterna;

                // El movimiento siempre cuesta la menor de las velocidades
                uint32_t costoExtra = max(arq[i], can[j]);
                uint32_t distancia = costo + costoExtra;

                // Agregamos este hijo a la lista
                agregarCandidato(dist, candidatos, hijo, distancia);

                hijo.can[j] = nodo.linterna;
            }
            // Lo volvemos a como estaba
            hijo.arq[i] = nodo.linterna;
        }
    }

    return res;
}

bool agregarCandidato(map<estado, uint32_t> &dist,
                      set<pair<uint32_t, estado>> &candidatos, estado &hijo,
                      uint32_t distancia) {
    auto s = dist.find(hijo);
    if (s != dist.end()) {
        // El hijo ya tiene una distancia
        // lo agregamos solo si la distancia resultante es menor
        uint32_t distVieja = s->second;

        if (distancia < distVieja) {
            // Hay que borrar la posición en la cola de candidatos
            // para insertarlo de nuevo
            auto p = candidatos.find({distVieja, hijo});
            candidatos.erase(p);
            dist.erase(dist.find(hijo));
        } else {
            // La distancia anterior ya era mejor que venir por este camino
            return false;
        }
    }

    dist.insert({hijo, distancia});
    candidatos.insert({distancia, hijo});

    return true;
}

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {
    uint32_t cantArq;
    uint32_t cantCan;
    arq.resize(0);
    can.resize(0);

    is >> cantArq >> cantCan;

    arq.reserve(cantArq);
    can.reserve(cantCan);

    for (uint32_t i = 0; i < cantArq; i++) {
        uint32_t a;
        is >> a;
        arq.push_back(a);
    }

    for (uint32_t i = 0; i < cantCan; i++) {
        uint32_t a;
        is >> a;
        can.push_back(a);
    }
}

int prob_solve(std::ostream &os) {
    int64_t t = tiempoMinimo(arq, can);

    os << t << endl;

    return t == -1 ? 1 : 0;
}

void prob_reload() {}

vector<uint64_t> prob_vars() {
    vector<uint64_t> res;

    res.push_back(arq.size());
    res.push_back(can.size());

    return res;
}

void prob_print_input(std::ostream &os) {
    for (auto v : prob_vars()) os << v << " ";
    os << endl;
}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {
    prob_reload();

    uint64_t cantArq = v[0];
    uint64_t cantCan = v[1];

    arq.resize(0);
    can.resize(0);

    for (uint64_t i = 0; i < cantArq; i++) {
        arq.push_back(rnd(1, 1000000));
    }

    for (uint64_t i = 0; i < cantCan; i++) {
        can.push_back(rnd(1, 1000000));
    }
}

vector<Generator> prob_generators() { return {{"random", generator_random}}; }
