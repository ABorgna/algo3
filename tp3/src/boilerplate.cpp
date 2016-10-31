#include <generators.h>
#include <pokegraph.h>

#include <stdint.h>
#include <iostream>
#include <vector>

using namespace std;

int64_t ngyms, nstops, bagSize;
PokeGraph graph;
string generatorName = "none";
int64_t generator = 0;

void prob_load(std::istream& is) {
    is >> ngyms >> nstops >> bagSize;

    vector<Node> gyms;
    vector<Node> stops;
    gyms.reserve(ngyms);
    stops.reserve(nstops);

    for (int64_t i = 0; i < ngyms; i++) {
        int64_t x, y, power;
        is >> x >> y >> power;
        gyms.push_back({{x, y}, power});
    }

    for (int64_t i = 0; i < ngyms; i++) {
        int64_t x, y;
        is >> x >> y;
        stops.push_back({{x, y}, -3});
    }

    graph = PokeGraph(move(gyms), move(stops));
}

void prob_reload() {}

vector<uint64_t> prob_vars() {
    return {(uint64_t)ngyms, (uint64_t)nstops, (uint64_t)bagSize};
}

void prob_print_input(std::ostream& os) {
    os << ngyms << " " << nstops << " " << bagSize << endl;
    for (const Node& n : graph.gyms()) {
        os << n.pos.x << " " << n.pos.y << " " << n.power << endl;
    }
    for (const Node& n : graph.stops()) {
        os << n.pos.x << " " << n.pos.y << endl;
    }
}

std::string prob_print_generator() { return generatorName; };

vector<Generator> prob_generators() { return defaultGenerators(); }
