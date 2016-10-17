#include <math.h>
#include <pokegraph.h>

// Node

bool Node::isGym() const { return power >= 0; }

// Graph

PokeGraph::PokeGraph(){};

PokeGraph::PokeGraph(vector<Node> gs, vector<Node> ss)
    : _gyms(move(gs)), _stops(move(ss)){};

Node PokeGraph::operator[](uint64_t i) {
    return i >= _gyms.size() ? _stops[i - _gyms.size()] : _gyms[i];
};

double PokeGraph::distance(uint64_t i, uint64_t j) {
    return distance((*this)[i], (*this)[j]);
};

double PokeGraph::distance(const Node& u, const Node& v) {
    uint64_t dx = u.pos.x - v.pos.x;
    uint64_t dy = u.pos.y - v.pos.y;

    return sqrt(dx * dx + dy * dy);
};

const vector<Node>& PokeGraph::gyms() { return _gyms; }

const vector<Node>& PokeGraph::stops() { return _stops; }
