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

// TODO: ver si se devuelve tipo tuple o queda global
int64_t tiempo_recorrido;
int64_t cant_recorrido;
vector<estacion> recorrido;

int agregarCandidato(   map<estacion, pair<tiempo, estacion>> tiempoMin,
                        set<pair<tiempo, estacion>> candidatos,
                        estacion padre, estacion est_candidato, tiempo t_candidato);

bool compare(tuple<estacion, estacion, tiempo> i, tuple<estacion, estacion, tiempo> d) {
    return (get<0>(i) < get<0>(d));
}

int reventarDaistrah (vector<tuple<estacion, estacion, tiempo>> vias,  int64_t estaciones)  {

    sort(vias.begin(), vias.end(), compare);

    map<estacion, pair<tiempo, estacion>> tiempoMin;                            // estacion -> <tiempo en llegar / predecesora>
    set<pair<tiempo, estacion>> candidatos;

    tiempoMin[1] = make_pair(0, 0);                                             // Usamos 0 como precedesor del inicio
    candidatos.insert(make_pair(0, 1));

    while (!candidatos.empty() ) {
        pair<tiempo, estacion> tope = *candidatos.begin();
        tiempo t_actual = tope.first;
        estacion est_actual = tope.second;

        if (est_actual == estaciones) {
            // llegamos a la ultima estación
            // armar el vector de preds y cant de estaciones
            tiempo_recorrido = t_actual;
            estacion pred = tiempoMin[est_actual].second;

            while (pred) {
                recorrido.insert(recorrido.begin(), pred);
                pred = tiempoMin[pred].second;
            }
            cant_recorrido = recorrido.size();

            return 0;
        }

        candidatos.erase(make_pair(t_actual, est_actual));

        // TODO: esto quedó medio feucho
        // buscar la primer arista que vaya desde est_actual en tiempo logaritmico
        auto it = upper_bound(vias.begin(), vias.end(), make_tuple(est_actual, 0, 0));
        while (get<0>(*it) == est_actual) {
            agregarCandidato(tiempoMin, candidatos, get<0>(*it), get<1>(*it), get<2>(*it));
            it++;
        }

    }

    return -1;
}


int agregarCandidato(   map<estacion, pair<tiempo, estacion>> tiempoMin,
                        set<pair<tiempo, estacion>> candidatos,
                        estacion padre, estacion est_candidato, tiempo t_candidato){

    if (tiempoMin.find(est_candidato) != tiempoMin.end()){
        // Been there, done that
        tiempo anterior = tiempoMin[est_candidato].second;

        if (t_candidato < anterior) {
            candidatos.erase(make_pair(anterior, est_candidato));
            tiempoMin.erase(est_candidato);
        } else return -1;
    }

    tiempoMin[est_candidato] = make_pair(t_candidato, padre);
    candidatos.insert(make_pair(t_candidato, est_candidato));

    return 0;

}


/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {}

int prob_solve(std::ostream &os) {}

void prob_reload() {}

vector<uint64_t> prob_vars() {}

void prob_print_input(std::ostream &os) {}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {}

vector<Generator> prob_generators() { return {}; }
