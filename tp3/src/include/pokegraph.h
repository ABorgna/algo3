#pragma once

#include <stdint.h>
#include <vector>

using namespace std;

struct position {
    int64_t x;
    int64_t y;
};

struct Node {
    position pos;
    int64_t power;

    bool isGym() const;
};

class PokeGraph {
   public:
    PokeGraph();
    PokeGraph(vector<Node> gyms, vector<Node> stops);

    Node& operator[](uint64_t i);
    const Node& operator[](uint64_t i) const;
    bool isGym(uint64_t i) const;
    double distance(uint64_t i, uint64_t j) const;
    double distance(const Node& u, const Node& v) const;

    const vector<Node>& gyms() const;
    const vector<Node>& stops() const;

   private:
    vector<Node> _gyms;
    vector<Node> _stops;
};
