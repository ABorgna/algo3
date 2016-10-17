#include <generators.h>
#include <interfaz.h>
#include <pokegraph.h>

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

extern uint64_t ngyms, nstops, bagSize;
extern PokeGraph graph;

/**************************************
 * Funciones del framework de testeo
 * las que faltan estan en boilerplate.cpp
 **************************************/

int prob_solve(std::ostream &os) {
    int64_t res = 0;

    os << res << endl;

    return res >= 0 ? 0 : -1;
}

vector<Option> prob_custom_options() { return {}; }
