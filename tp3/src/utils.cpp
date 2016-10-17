#include <utils.h>

bool esCaminoValido(vector<uint64_t> orden, int64_t bagSize,
                    const PokeGraph& graph) {
    int64_t power = 0;
    for (uint64_t i : orden) {
        // Las paradas siempre valen -3
        power += graph[i].power;
        if (power > 0) {
            return false;
        } else if (-power > bagSize) {
            power = -bagSize;
        }
    }
    return true;
}

double distanciaCamino(vector<uint64_t> orden, const PokeGraph& graph) {
    double dist = 0;
    double sinceLastGym = 0;
    uint64_t last = orden[0];
    for (uint64_t i : orden) {
        sinceLastGym += graph.distance(last, i);
        if (graph.isGym(i)) {
            dist += sinceLastGym;
            sinceLastGym = 0;
        }
        last = i;
    }
    return dist;
}
