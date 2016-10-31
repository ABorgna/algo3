#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>
#include <utils.h>

#include <math.h>
#include <stdint.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

// Cambiar esto para cada heuristica
const uint64_t N_HEURISTICA = 2;

extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;

extern vector<int64_t> orden;

//TODO: Esto dudosamente se haga así
extern pair<double, uint64_t> omNomNom();

pair<double, uint64_t> dos_opt() {

    // Unleash the greedy (se puede arrancar también con un iota sobre orden, pero quizás es mejor arrancar un poco más cerca de un resultado 'no-tan-fruta')
    omNomNom();
    
    double dist, dist_vecino;
    bool mejora, valido;
    vector<int64_t> orden_vecino(orden.size(), -1);
     
    do {
        mejora = false;
        dist = distanciaCamino(orden.begin(), orden.end(), graph);
        for (uint64_t i = 0; i < orden.size() - 1; i++) {
            for (uint64_t j = i + 1; j < orden.size(); j++) {
            
                //Swap 2opt i-j
                orden_vecino = orden;
                reverse_copy(orden.begin() + i, orden.begin() + i + j, orden_vecino.begin() + i);
                
                //TODO: Para que esto funcione como debe, orden no puede tener trimeado el resto de las paradas (tienen que estar para que el swapeo de la mayor cantidad posible de combinaciones)
                
                int64_t pow = 0;
                valido = esCaminoValido(orden_vecino.begin(), orden_vecino.end(), bagSize, graph, pow);
                
                if (not valido)
                    continue;
                
                dist_vecino = distanciaCamino(orden_vecino.begin(), orden_vecino.end(), graph);
                
                //Si mejoró seguimos iterando nuevas combinaciones con el nuevo orden
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


/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream& os) {
    double d;
    uint64_t k;

    tie(d, k) = dos_opt();

    os << d << " " << k;
    for (auto i : orden) os << " " << i + 1;
    os << endl;

    return N_HEURISTICA * 100 + generator * 10;
}

vector<Option> prob_custom_options() { return {}; }
