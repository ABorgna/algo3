#include <generators.h>
#include <interfaz.h>

#include <cassert>

// Globals :/
extern int64_t ngyms, nstops, bagSize;
extern PokeGraph graph;
extern int64_t generator;

// Utilities

vector<Generator> defaultGenerators() {
    return {
        {"random", &randomGenerator},
        {"separated", &separatedGenerator},
        {"zigzag", &zigzagGenerator},
    };
}

// Generators

void randomGenerator(const vector<uint64_t>& v) {
    generator = 1;

    ngyms = v[0];
    nstops = v[1];
    bagSize = v[2];

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    int64_t maxEnergy = nstops * 3;

    for (int64_t i = 0; i < ngyms; i++) {
        int64_t x = rnd(0, 65535);
        int64_t y = rnd(0, 65535);
        int64_t power = min((int64_t)rnd(0, 10), maxEnergy - (ngyms - i - 1));
        power = min(power, (int64_t)bagSize);
        gyms.push_back({{x, y}, power});
        maxEnergy -= power;
    }

    for (int64_t i = 0; i < nstops; i++) {
        int64_t x = rnd(0, 65535);
        int64_t y = rnd(0, 65535);
        stops.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}

void separatedGenerator(const vector<uint64_t>& v) {
    generator = 2;

    ngyms = v[0];
    nstops = v[1];
    bagSize = v[2];

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    int64_t maxEnergy = nstops * 3;

    for (int64_t i = 0; i < ngyms; i++) {
        int64_t x = rnd(0, 16383);
        int64_t y = rnd(0, 16383);
        int64_t power = min((int64_t)rnd(0, 10), maxEnergy - (ngyms - i - 1));
        power = min(power, (int64_t)bagSize);
        gyms.push_back({{x, y}, power});
        maxEnergy -= power;
    }

    for (int64_t i = 0; i < nstops; i++) {
        int64_t x = rnd(32768, 49152);
        int64_t y = rnd(0, 16383);
        stops.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}

void zigzagGenerator(const vector<uint64_t>& v) {
    generator = 3;

    ngyms = v[0];
    nstops = v[1];
    bagSize = 3;

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    for (int64_t i = 0; i < ngyms; i++) {
        int64_t x = 0;
        int64_t y = rnd(0, 16383);
        int64_t power = i >= nstops ? 0 : 3;
        gyms.push_back({{x, y}, power});
    }

    for (int64_t i = 0; i < nstops; i++) {
        int64_t x = 16383;
        int64_t y = rnd(0, 16383);
        stops.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}
