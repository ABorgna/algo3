#pragma once

#include <stdint.h>
#include <vector>
#include <set>
#include <tuple>

using namespace std;

pair<double, uint64_t> exacto_bruteforce(vector<int64_t> &orden);
pair<double, uint64_t> exacto_backtracking(vector<int64_t> &orden);
pair<double, uint64_t> exacto_dinamica(vector<int64_t> &orden);
pair<double, uint64_t> greedy_omNomNom(vector<int64_t> &orden);
pair<double, uint64_t> local_dos_opt(vector<int64_t> &orden);

