#include <generators.h>
#include <interfaz.h>

#include <cassert>

// Globals :/
extern uint64_t ngyms, nstops, bagSize;
extern PokeGraph graph;

// Utilities

vector<Generator> defaultGenerators() {
    return {
        {"random", &randomGenerator}, {"separated", &separatedGenerator},
    };
}

// Generators

void randomGenerator(const vector<uint64_t>& v) {
    ngyms = v[0];
    nstops = v[1];
    bagSize = v[2];

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    int64_t maxEnergy = nstops * 3;

    for (uint64_t i = 0; i < ngyms; i++) {
        int64_t x = rnd(0, 65535);
        int64_t y = rnd(0, 65535);
        int64_t power = min(rnd(0, 10), maxEnergy - (ngyms - i - 1));
        gyms.push_back({{x, y}, power});
        maxEnergy -= power;
    }

    for (uint64_t i = 0; i < ngyms; i++) {
        int64_t x = rnd(0, 65535);
        int64_t y = rnd(0, 65535);
        stops.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}

void separatedGenerator(const vector<uint64_t>& v) {
    ngyms = v[0];
    nstops = v[1];
    bagSize = v[2];

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    int64_t maxEnergy = nstops * 3;

    for (uint64_t i = 0; i < ngyms; i++) {
        int64_t x = rnd(0, 16383);
        int64_t y = rnd(0, 16383);
        int64_t power = min(rnd(0, 10), maxEnergy - (ngyms - i - 1));
        gyms.push_back({{x, y}, power});
        maxEnergy -= power;
    }

    for (uint64_t i = 0; i < ngyms; i++) {
        int64_t x = rnd(32768, 49152);
        int64_t y = rnd(0, 16383);
        stops.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}
