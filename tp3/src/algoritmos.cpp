#include <algoritmos.h>
#include <interfaz.h>  // rnd
#include <pokegraph.h>

#include <utils.h>
#include <algorithm>
#include <functional>
#include <numeric>

#include <cassert>
#include <iostream>

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
    orden = vector<int64_t>(ngyms + nstops, 0);

    double mejorDist = numeric_limits<double>::infinity();
    uint64_t mejorOrdenLen = 0;
    vector<int64_t> mejorOrden = orden;
    vector<bool> used(ngyms + nstops, false);

    function<void(uint64_t, int64_t, int64_t, double)> recursiva = [&](
        int64_t pos, int64_t gymCounter, int64_t powAcumulado,
        double distancia) {

        if (pos >= ngyms + nstops) {
            // Algo salió mal
            return;
        }

        // Calculo distancia para orden[0..pos]
        if (pos >= 1) {
            distancia += graph.distance(orden[pos - 1], orden[pos]);
        }

        // Vemos si orden[0..pos] es solución
        if (graph.isGym(orden[pos]))
            gymCounter++;

        if (gymCounter == ngyms) {
            // Llegamos a una posible solucion
            if (distancia < mejorDist) {
                mejorDist = distancia;
                mejorOrden = orden;
                mejorOrdenLen = pos;
            }
            return;
        }

        if (pos + 1 == ngyms + nstops)
            return;

        for (int i = 0; i < ngyms + nstops; i++) {
            if (used[i])
                continue;

            orden[pos + 1] = i;
            int64_t newPowAcumulado = powAcumulado;

            if (not esCaminoValido(orden.begin() + pos + 1,
                                   orden.begin() + pos + 2, bagSize, graph,
                                   newPowAcumulado))
                continue;

            used[i] = true;
            recursiva(pos + 1, gymCounter, newPowAcumulado, distancia);
            used[i] = false;
        }
    };

    for (int i = 0; i < ngyms + nstops; i++) {
        if (graph[i].power > 0)
            continue;
        orden[0] = i;
        used[i] = true;
        recursiva(0, 0, -graph[i].power, 0);
        used[i] = false;
    }

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
    double distRecorrida = 0;

    orden = std::vector<int64_t>();

    set<pair<int64_t, int64_t>> gyms;
    for (int i = 0; i < ngyms; i++) {
        gyms.emplace(graph[i].power, i);
    }

    set<int64_t> paradas;
    for (int i = 0; i < nstops; i++) {
        paradas.insert(i + ngyms);
    }

    int64_t potas_actual = 0;

    // Arranco en un gimnasio.
    int nodo_actual;

    // Si no es un gimnasio gratis, arranco en una parada random.
    if (gyms.begin()->first > 0) {
        nodo_actual = *paradas.begin();
        paradas.erase(paradas.begin());
        potas_actual = min((int64_t)3, bagSize);
    } else {
        nodo_actual = gyms.begin()->second;
        gyms.erase(gyms.begin());
    }
    orden.push_back(nodo_actual);

    while (!gyms.empty()) {
        if (gyms.begin()->first <= potas_actual) {
            // hay por lo menos un gym al que ir
            // Checkear la distancia para guardarse el mínimo
            auto gym_candidato = gyms.begin();
            double dist_candidato =
                graph.distance(nodo_actual, gym_candidato->second);

            for (auto it = gyms.begin();
                 it != gyms.end() and it->first <= potas_actual; it++) {
                if (graph.distance(nodo_actual, it->second) < dist_candidato) {
                    gym_candidato = it;
                    dist_candidato =
                        graph.distance(nodo_actual, gym_candidato->second);
                }
            }

            // Voy a ese gimnasio y pierdo las potas que pide
            distRecorrida += dist_candidato;
            nodo_actual = gym_candidato->second;
            potas_actual -= gym_candidato->first;
            gyms.erase(gym_candidato);

        } else {
            // Chequeamos si quedan pokeparadas
            if (not paradas.empty()) {
                // agarramos una pokeparada
                // Buscar el de mínima distancia linealmente en paradas
                auto parada_mas_cercana = paradas.begin();
                double dist_candidato =
                    graph.distance(nodo_actual, *parada_mas_cercana);

                for (auto it = paradas.begin(); it != paradas.end(); it++) {
                    if (graph.distance(nodo_actual, *parada_mas_cercana) <
                        dist_candidato) {
                        parada_mas_cercana = it;
                        dist_candidato =
                            graph.distance(nodo_actual, *parada_mas_cercana);
                    }
                }

                // Voy a la parada y actualizo mi cantidad de potas
                distRecorrida += dist_candidato;
                nodo_actual = *parada_mas_cercana;

                potas_actual = min(potas_actual + 3, bagSize);
                paradas.erase(nodo_actual);
            } else {
                distRecorrida = numeric_limits<double>::infinity();
                orden.resize(0);
                break;
            }
        }
        orden.push_back(nodo_actual);
    }
    return {distRecorrida, orden.size()};
}

// ------------------------------------ Solución greedy random

pair<double, uint64_t> greedy_random(vector<int64_t> &orden) {
    double distRecorrida = 0;

    orden = std::vector<int64_t>();
    set<pair<int64_t, int64_t>> gyms;
    for (int i = 0; i < ngyms; i++) {
        gyms.emplace(graph[i].power, i);
    }

    set<int64_t> paradas;
    for (int i = 0; i < nstops; i++) {
        paradas.insert(i + ngyms);
    }

    int64_t potas_actual = 0;

    // Arranco en un gimnasio.
    int nodo_actual;

    // Si no es un gimnasio gratis, arranco en una parada random.
    if (gyms.begin()->first > 0) {
        nodo_actual = *paradas.begin();
        paradas.erase(paradas.begin());
        potas_actual = min((int64_t)3, bagSize);
    } else {
        nodo_actual = gyms.begin()->second;
        gyms.erase(gyms.begin());
    }
    orden.push_back(nodo_actual);

    while (!gyms.empty()) {
        if (gyms.begin()->first <= potas_actual) {
            // hay por lo menos un gym al que ir
            // Checkear la distancia para guardarse el mínimo
            vector<pair<int64_t, int64_t>> candidatos;
            double dist_candidato;

            for (auto it = gyms.begin();
                 it != gyms.end() and it->first <= potas_actual; it++) {
                dist_candidato = graph.distance(nodo_actual, it->second);
                candidatos.push_back({dist_candidato, it->second});
            }
            // Ordena por menor distancia
            sort(candidatos.begin(), candidatos.end());

            // el próximo gimnasio aleatoriamente entre la mitad más cercana
            uint64_t indice_random = rnd(0, candidatos.size() / 2);
            pair<int64_t, int64_t> siguiente = candidatos[indice_random];
            // Voy a ese gimnasio y pierdo las potas que pide
            distRecorrida += siguiente.first;
            nodo_actual = siguiente.second;
            potas_actual -= graph[nodo_actual].power;
            auto it =
                gyms.find({graph[siguiente.second].power, siguiente.second});
            gyms.erase(it);

        } else {
            // Chequeamos si quedan pokeparadas
            if (not paradas.empty()) {
                // agarramos una pokeparada
                // Buscar el de mínima distancia linealmente en paradas
                vector<pair<int64_t, int64_t>> candidatos;
                double dist_candidato;

                for (auto it = paradas.begin(); it != paradas.end(); it++) {
                    dist_candidato = graph.distance(nodo_actual, *it);
                    candidatos.push_back({dist_candidato, *it});
                }
                // Ordena por menor distancia
                sort(candidatos.begin(), candidatos.end());

                // la próxima parada aleatoriamente entre la mitad más cercana
                uint64_t indice_random = rnd(0, candidatos.size() / 2);
                pair<int64_t, int64_t> siguiente = candidatos[indice_random];

                // Voy a la parada y actualizo mi cantidad de potas

                distRecorrida += siguiente.first;
                nodo_actual = siguiente.second;
                potas_actual = min(potas_actual + 3, bagSize);
                auto it = paradas.find(nodo_actual);
                paradas.erase(it);

            } else {
                distRecorrida = numeric_limits<double>::infinity();
                orden.resize(0);
                break;
            }
        }
        orden.push_back(nodo_actual);
    }
    return {distRecorrida, orden.size()};
}

// ---------------------------- G R A S P

pair<double, uint64_t> grasp(vector<int64_t> &orden) {
    // Llamo a Greedy random para tener alguna solucion
    vector<int64_t> mejor1 , actual1 ; // PlaceHolders
    pair<double, uint64_t> mejor = greedy_random(mejor1);
    pair<double, uint64_t> actual;

    bool flag = true;
    // Mi idea es usar algunas veces 2opt y otras veces swap de nodos
    int limit = orden.size();

    for (int i = 0; i < limit; ++i) {
    actual = greedy_random(actual1);
        if (flag) {
           actual = local_dos_opt(actual1,false,i);
        }

        else {
            actual = local_swap(actual1,false,i);
        }
        flag = !flag;
        mejor = actual.first < mejor.first ? actual : mejor;
        orden = actual.first < mejor.first ? actual1 : mejor1;
    }

    return mejor;
}

// ------------------------------------ Corrida de opt (devuelve dist y si hubo mejora)

pair<double, bool> iterar2opt(vector<int64_t> &orden, bool verbose) {

    // Para que esto funcione como debe, orden no puede tener
    // trimeado el resto de las paradas (tienen que estar para que
    // el swapeo de la mayor cantidad posible de combinaciones)

    double dist, dist_vecino;
    bool huboMejora, valido;
    vector<int64_t> orden_vecino(orden.size(), -1);

    // distanciaCamino solo mide hasta el último gym
    dist = distanciaCamino(orden.begin(), orden.end(), graph);

    // devolvemos:
    huboMejora = false;

    for (uint64_t i = 0; i < orden.size(); i++) {
        for (uint64_t j = i + 2; j <= orden.size(); j++) {
            // Swap 2opt [i..j)
            orden_vecino = orden;
            uint64_t desde = i;
            uint64_t hasta = j;
            while (hasta != desde) {
                // A esto los seres humanos decentes le dicen 'módulo'
                // pero experimentando con el operador % me salió fruta
                hasta = (hasta == 0) ? orden.size() - 1 : hasta - 1;
                if (hasta == desde)
                    break;
                orden_vecino[desde] = orden[hasta];
                orden_vecino[hasta] = orden[desde];
                desde = (desde == orden.size() - 1) ? 0 : desde + 1;
            }

            // Nos interesa saber si hay camino válido (y su longitud)
            // únicamente hasta el último gym, el resto está por
            // 'completitud'
            // para swaps.

            auto ultimo_gym = orden_vecino.end();
            for (auto it = orden_vecino.begin(); it != orden_vecino.end();
                 it++) {
                if (graph.isGym(*it))
                    ultimo_gym = it;
            }

            assert(ultimo_gym != orden_vecino.end());

            int64_t pow = 0;
            valido = esCaminoValido(orden_vecino.begin(), ultimo_gym + 1,
                                    bagSize, graph, pow);

            if (not valido)
                continue;

            dist_vecino = distanciaCamino(orden_vecino.begin(),
                                          orden_vecino.end(), graph);

            // Si mejoró seguimos iterando nuevas combinaciones con el nuevo
            // orden
            if (dist_vecino < dist) {
                huboMejora = true;
                dist = dist_vecino;
                orden = orden_vecino;
                if (verbose)
                    std::cerr << dist << std::endl;
            }
        }
    }

    return {dist, huboMejora};
}

// ------------------------------------ Solución búsqueda local por 2opt
// Si corridas == 0, se itera hasta que deje de encontrar mejoras
pair<double, uint64_t> local_dos_opt(vector<int64_t> &orden, bool verbose, int corridas) {
    // Unleash the greedy (se puede arrancar también con un iota sobre orden,
    // pero quizás es mejor arrancar un poco más cerca de un resultado
    // 'no-tan-fruta')
    auto p = greedy_omNomNom(orden);

    if (p.first == numeric_limits<double>::infinity()) {
        // No encontró solución
        return p;
    }
    // Para que esto funcione como debe, orden no puede tener
    // trimeado el resto de las paradas (tienen que estar para que
    // el swapeo de la mayor cantidad posible de combinaciones)
    for (int64_t i = 0; i < ngyms + nstops; i++) {
        if (find(orden.begin(), orden.end(), i) == orden.end())
            orden.push_back(i);
    }

    double dist;

    bool seguir;
    do {
        pair<double, bool> ult_corrida = iterar2opt(orden, verbose);
        seguir = ult_corrida.second;
        dist = ult_corrida.first;
        if (corridas > 0)
            seguir &= (--corridas > 0);
    } while (seguir);

    // Trim sobre las paradas tras el último gym

    auto ultimo_gym = orden.rend();
    for (auto it = orden.rbegin(); it != orden.rend(); it++) {
        if (graph.isGym(*it)) {
            ultimo_gym = it;
            break;
        }
    }

    orden.resize(orden.size() - distance(orden.rbegin(), ultimo_gym));

    return {dist, orden.size()};
}


// ------------------------------------ Corrida de swap (devuelve dist y si hubo mejora)

pair<double, bool> iterar_swap(vector<int64_t> &orden, bool verbose) {

    double dist, dist_vecino;
    bool valido;
    vector<int64_t> orden_vecino(orden.size(), -1);

    bool huboMejora = false;
    dist = distanciaCamino(orden.begin(), orden.end(), graph);

    for (size_t i = 0; i < orden.size(); i++) {
        for (size_t j = i + 1; j < orden.size(); j++) {
            orden_vecino = orden;
            orden_vecino[i] = orden[j];
            orden_vecino[j] = orden[i];

            auto ultimo_gym = orden_vecino.end();
            for (auto it = orden_vecino.begin(); it != orden_vecino.end();
                 it++) {
                if (graph.isGym(*it))
                    ultimo_gym = it;
            }

            assert(ultimo_gym != orden_vecino.end());

            int64_t pow = 0;
            valido = esCaminoValido(orden_vecino.begin(), ultimo_gym + 1,
                                    bagSize, graph, pow);

            if (not valido)
                continue;

            dist_vecino = distanciaCamino(orden_vecino.begin(),
                                          orden_vecino.end(), graph);

            if (dist_vecino < dist) {
                huboMejora = true;
                dist = dist_vecino;
                orden = orden_vecino;
                if (verbose)
                    std::cerr << dist << std::endl;
            }
        }
    }

    return {dist, huboMejora};
}

// ------------------------------------ Solución búsqueda local por swap de
// nodos
// Si corridas == 0, se itera hasta que deje de encontrar mejoras
pair<double, uint64_t> local_swap(vector<int64_t> &orden, bool verbose, int corridas) {
    auto p = greedy_omNomNom(orden);

    if (p.first == numeric_limits<double>::infinity()) {
        // No encontró solución
        return p;
    }
    for (int64_t i = 0; i < ngyms + nstops; i++) {
        if (find(orden.begin(), orden.end(), i) == orden.end())
            orden.push_back(i);
    }

    double dist;
    bool seguir;
    do {
        pair<double, bool> ult_corrida = iterar_swap(orden, verbose);
        seguir = ult_corrida.second;
        dist = ult_corrida.first;
        if (corridas > 0)
            seguir &= (--corridas > 0);
    } while (seguir);

    auto ultimo_gym = orden.rend();
    for (auto it = orden.rbegin(); it != orden.rend(); it++) {
        if (graph.isGym(*it)) {
            ultimo_gym = it;
            break;
        }
    }

    orden.resize(orden.size() - distance(orden.rbegin(), ultimo_gym));

    return {dist, orden.size()};
}
