#pragma once

#include <stdint.h>
#include <vector>

void generateDungeon(int64_t n, int64_t m, double density,
                     std::vector<std::vector<bool>> &maze,
                     std::pair<int64_t, int64_t> &start,
                     std::pair<int64_t, int64_t> &end);
