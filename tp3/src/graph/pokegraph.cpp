#include <math.h>
#include <pokegraph.h>

// Node

bool Node::isGym() const { return power >= 0; }

// Graph

PokeGraph::PokeGraph(){};

PokeGraph::PokeGraph(vector<Node> gs, vector<Node> ss)
    : _gyms(move(gs)), _stops(move(ss)){};

Node& PokeGraph::operator[](uint64_t i) {
    return isGym(i) ? _gyms[i] : _stops[i - _gyms.size()];
};

const Node& PokeGraph::operator[](uint64_t i) const {
    return isGym(i) ? _gyms[i] : _stops[i - _gyms.size()];
};

bool PokeGraph::isGym(uint64_t i) const { return i < _gyms.size(); };

double PokeGraph::distance(uint64_t i, uint64_t j) const {
    return distance((*this)[i], (*this)[j]);
};

double PokeGraph::distance(const Node& u, const Node& v) const {
    uint64_t dx = u.pos.x - v.pos.x;
    uint64_t dy = u.pos.y - v.pos.y;

    return sqrt(dx * dx + dy * dy);
};

const vector<Node>& PokeGraph::gyms() const { return _gyms; }

const vector<Node>& PokeGraph::stops() const { return _stops; }
