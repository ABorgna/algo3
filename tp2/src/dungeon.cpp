#include <dungeon.h>
#include <interfaz.h>

#include <vector>

void generateDungeon(int64_t n, int64_t m, double density,
                     std::vector<std::vector<bool>> &maze) {
    maze.resize(0);
    maze.reserve(m);

    uint64_t p = density * UINT64_MAX;

    for (int64_t y = 0; y < m; y++) {
        maze.push_back(std::vector<bool>(n, false));

        for (int64_t x = 0; x < n; x++) {
            if (x == 0 or y == 0 or x == n - 1 or y == m - 1) {
                maze[y][x] = true;
                continue;
            }

            bool up = maze[y - 1][x];
            bool left = maze[y][x - 1];
            bool diag = maze[y - 1][x - 1];

            if (up and left and diag)
                continue;

            if (up and left) {
                maze[y][x] = true;
                continue;
            }

            if (rnd() < (up or left ? p : p/4)) {
                maze[y][x] = true;
            }
        }
    }
}
