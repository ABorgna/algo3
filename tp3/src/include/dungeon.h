#pragma once

#include <interfaz.h>

#include <stdint.h>
#include <vector>

template<typename T>
void generateDungeon(int64_t m, int64_t n, double density,
                     std::vector<std::vector<T>> &maze,
                     T wall, T empty) {
    maze.resize(0);
    maze.reserve(m);

    double kf = density / (-3 * density * density + 6 * density + 1);
    double uf = kf * 2;
    uint64_t k = kf * UINT64_MAX;
    uint64_t u = uf * UINT64_MAX;

    for (int64_t y = 0; y < m; y++) {
        maze.push_back(std::vector<T>(n, empty));

        for (int64_t x = 0; x < n; x++) {
            if (x == 0 or y == 0 or x == n - 1 or y == m - 1) {
                maze[y][x] = wall;
                continue;
            }

            bool up = maze[y - 1][x];
            bool left = maze[y][x - 1];
            bool diag = maze[y - 1][x - 1];

            if (up and left and diag)
                continue;

            if (up and left) {
                maze[y][x] = wall;
                continue;
            }

            //if (rnd() < (up or left ? p : p/4)) {
            if (rnd() < (up or left ? u : k)) {
                maze[y][x] = wall;
            }
        }
    }
}

