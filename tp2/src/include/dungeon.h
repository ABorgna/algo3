#pragma once

#include <stdint.h>
#include <vector>

void generateDungeon(int64_t m, int64_t n, double density,
                     std::vector<std::vector<bool>> &maze);
