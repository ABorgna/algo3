#include <algoritmos.h>
#include <pokegraph.h>

#include <utils.h>
#include <algorithm>
#include <functional>
#include <numeric>

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;

// ------------------------------------ Solución exacta por fuerza bruta

pair<double, uint64_t> exacto_bruteforce(vector<int64_t> &orden) {
    // Inicializar la combinacion
    // Suponemos que siempre hay solucion
    orden = vector<int64_t>(ngyms + nstops);
    iota(orden.begin(), orden.end(), 0);

    double mejorDist = numeric_limits<double>::infinity();
    uint64_t mejorComb = 0;
    uint64_t comb = 0;
    do {
        if (esCaminoValido(orden.begin(), orden.end(), bagSize, graph)) {
            double dist = distanciaCamino(orden.begin(), orden.end(), graph);
            if (dist < mejorDist) {
                mejorDist = dist;
                mejorComb = comb;
            }
        }
        comb++;
    } while (next_permutation(orden.begin(), orden.end()));

    // Hacemos rollback hasta la mejor permutacion
    // Es mas rápido que guardarse la combinacion cada vez que vemos un maximo
    while (comb > mejorComb) {
        comb--;
        prev_permutation(orden.begin(), orden.end());
    }

    // Recortamos las paradas del final
    auto lastGym = find_if(orden.rbegin(), orden.rend(),
                           [](uint64_t i) { return graph.isGym(i); });
    orden.resize(orden.size() - distance(orden.rbegin(), lastGym));

    return {mejorDist, orden.size()};
}

// ------------------------------------ Solución exacta por Backtracking

pair<double, uint64_t> exacto_backtracking(vector<int64_t> &orden) {
    orden = vector<int64_t>(ngyms + nstops, -1);

    vector<int64_t> distanciaAcumulada(ngyms + nstops, 0);
    double mejorDist = numeric_limits<double>::infinity();
    uint64_t mejorOrdenLen = 0;
    vector<int64_t> mejorOrden = orden;
    vector<int64_t> powerAcumulado(ngyms + nstops, 0);
    vector<bool> used(ngyms + nstops, false);

    function<void(uint64_t, int64_t)> recursiva = [&](uint64_t pos,
                                                      int64_t gymCounter) {
        // Calculo distancia para orden[0..pos-1]
        if (pos > 1) {
            distanciaAcumulada[pos - 1] =
                graph.distance(orden[pos - 2], orden[pos - 1]) +
                distanciaAcumulada[pos - 2];
        }

        // Vemos si orden[0..pos-1] es solución
        if (pos > 0) {
            if (graph.isGym(orden[pos - 1]))
                gymCounter++;

            if (gymCounter == ngyms) {
                // Llegamos a una posible solucion
                if (distanciaAcumulada[pos - 1] < mejorDist) {
                    mejorDist = distanciaAcumulada[pos - 1];
                    mejorOrden = orden;
                    mejorOrdenLen = pos;
                    return;
                }
            }
        }

        bool valido;
        powerAcumulado[pos] = pos > 0 ? powerAcumulado[pos - 1] : 0;

        for (int i = 0; i < ngyms + nstops; i++) {
            orden[pos] = i;

            int64_t pow_anterior = powerAcumulado[pos];
            valido =
                esCaminoValido(orden.begin() + pos, orden.begin() + pos + 1,
                               bagSize, graph, powerAcumulado[pos]);

            if (not valido or used[i]) {
                powerAcumulado[pos] = pow_anterior;
                continue;
            }

            used[i] = true;
            recursiva(pos + 1, gymCounter);
            powerAcumulado[pos] = pow_anterior;
            used[i] = false;
        }
    };

    recursiva(0, 0);
    orden = mejorOrden;

    return {mejorDist, orden.size()};
}

// ------------------------------------ Solución exacta por dinamic programming

pair<double, uint64_t> exacto_dinamica(vector<int64_t> &orden) {
    // Para ngyms+nstops <= 64
    int64_t n = ngyms + nstops;

    if (n > 64)
        return {0, 0};

    for (int64_t init = 0; init < n; init++) {
        if (graph[init].power > 0)
            continue;

        vector<vector<double>> d(
            1 << n, vector<double>(n, numeric_limits<double>::infinity()));
    }

    // TODO
    orden = {1};

    return {0, 0};
}

// ------------------------------------ Solución greedy trivial

pair<double, uint64_t> greedy_omNomNom(vector<int64_t> &orden) {
    // Inicializar la combinacion
    // Suponemos que siempre hay solucion
    orden = vector<int64_t>(ngyms + nstops);

    // Simple greedy is best greedy
    iota(orden.begin(), orden.end(), 0);

    double dist = distanciaCamino(orden.begin(), orden.end(), graph);

    // Recortamos las paradas del final
    auto lastGym = find_if(orden.rbegin(), orden.rend(),
                           [](uint64_t i) { return graph.isGym(i); });
    orden.resize(orden.size() - distance(orden.rbegin(), lastGym));

    return {dist, orden.size()};
}

// ------------------------------------ Solución greedy trivial

pair<double, uint64_t> local_dos_opt(vector<int64_t> &orden) {
    // Unleash the greedy (se puede arrancar también con un iota sobre orden,
    // pero quizás es mejor arrancar un poco más cerca de un resultado
    // 'no-tan-fruta')
    greedy_omNomNom(orden);

    double dist, dist_vecino;
    bool mejora, valido;
    vector<int64_t> orden_vecino(orden.size(), -1);

    do {
        mejora = false;
        dist = distanciaCamino(orden.begin(), orden.end(), graph);
        for (uint64_t i = 0; i < orden.size() - 1; i++) {
            for (uint64_t j = i + 1; j < orden.size(); j++) {
                // Swap 2opt i-j
                orden_vecino = orden;
                reverse_copy(orden.begin() + i, orden.begin() + i + j,
                             orden_vecino.begin() + i);

                // TODO: Para que esto funcione como debe, orden no puede tener
                // trimeado el resto de las paradas (tienen que estar para que
                // el swapeo de la mayor cantidad posible de combinaciones)

                int64_t pow = 0;
                valido =
                    esCaminoValido(orden_vecino.begin(), orden_vecino.end(),
                                   bagSize, graph, pow);

                if (not valido)
                    continue;

                dist_vecino = distanciaCamino(orden_vecino.begin(),
                                              orden_vecino.end(), graph);

                // Si mejoró seguimos iterando nuevas combinaciones con el nuevo
                // orden
                if (dist_vecino < dist) {
                    mejora = true;
                    dist = dist_vecino;
                    orden = orden_vecino;
                }
            }
        }

    } while (mejora);

    return {dist, orden.size()};
}
