#include <utils.h>
#include <cassert>

bool esCaminoValido(vector<int64_t>::iterator start,
                    vector<int64_t>::iterator end, int64_t bagSize,
                    const PokeGraph& graph) {
    int64_t power = 0;
    return esCaminoValido(start, end, bagSize, graph, power);
}

bool esCaminoValido(vector<int64_t>::iterator start,
                    vector<int64_t>::iterator end, int64_t bagSize,
                    const PokeGraph& graph, int64_t& powerAcum) {
    for (auto it = start; it != end; it++) {
        // Las paradas siempre valen -3
        powerAcum -= graph[*it].power;
        if (powerAcum < 0) {
            break;
        } else if (powerAcum > bagSize) {
            powerAcum = bagSize;
        }
    }
    return powerAcum >= 0;
}

double distanciaCamino(vector<int64_t>::iterator start,
                       vector<int64_t>::iterator end, const PokeGraph& graph) {
    assert(start != end);
    double dist = 0;
    double sinceLastGym = 0;
    uint64_t last = *start;
    for (auto it = start; it != end; it++) {
        sinceLastGym += graph.distance(last, *it);
        if (graph.isGym(*it)) {
            dist += sinceLastGym;
            sinceLastGym = 0;
        }
        last = *it;
    }
    return dist;
}
