#pragma once

#include <stdint.h>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

pair<double, uint64_t> exacto_bruteforce(vector<int64_t> &orden);
pair<double, uint64_t> exacto_backtracking(vector<int64_t> &orden);
pair<double, uint64_t> exacto_backtracking_distancias(vector<int64_t> &orden);
pair<double, uint64_t> greedy_omNomNom(vector<int64_t> &orden, bool closest_first);
pair<double, uint64_t> local_dos_opt(vector<int64_t> &orden, bool verbose,
                                     int corridas);
pair<double, uint64_t> local_swap(vector<int64_t> &orden, bool verbose,
                                  int corridas);
pair<double, uint64_t> local_swap_min(vector<int64_t> &orden, bool verbose,
                                int corridas);
pair<double, uint64_t> grasp_alternado(vector<int64_t> &orden, double expLimite,
                             double expInicios);
pair<double, uint64_t> grasp_trim(vector<int64_t> &orden, double expLimite,
                             double expInicios);
