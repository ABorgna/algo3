#include <generators.h>
#include <pokegraph.h>

#include <stdint.h>
#include <iostream>
#include <vector>

using namespace std;

uint64_t ngyms, nstops, bagSize;
PokeGraph graph;

void prob_load(std::istream &is) {
    is >> ngyms >> nstops >> bagSize;

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    for (uint64_t i = 0; i < ngyms; i++) {
        int64_t x,y,power;
        is >> x >> y >> power;
        gyms.push_back({{x, y}, power});
    }

    for (uint64_t i = 0; i < ngyms; i++) {
        int64_t x,y;
        is >> x >> y;
        gyms.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}

void prob_reload() {}

vector<uint64_t> prob_vars() { return {ngyms, nstops, bagSize}; }

void prob_print_input(std::ostream &os) {
    os << ngyms << " " << nstops << ""  << bagSize << endl;
    for(const Node& n : graph.gyms()) {
        os << n.pos.x << " " << n.pos.y << " " << n.power << endl;
    }
    for(const Node& n : graph.stops()) {
        os << n.pos.x << " " << n.pos.y << endl;
    }
}

vector<Generator> prob_generators() {
    setupGraph(&graph);
    return defaultGenerators();
}
