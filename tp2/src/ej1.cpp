#include <interfaz.h>

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

/**************************************
 * Funciones del framework de testeo
 **************************************/

void prob_load(std::istream &is) {}

int prob_solve(std::ostream &os) {}

void prob_reload() {}

vector<uint64_t> prob_vars() {}

void prob_print_input(std::ostream &os) {}

vector<Option> prob_custom_options() { return {}; }

void generator_random(const std::vector<uint64_t> &v) {}

vector<Generator> prob_generators() { return {}; }

