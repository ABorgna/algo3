#include <dungeon.h>
#include <interfaz.h>

#include <vector>

void generateDungeon(int64_t n, int64_t m, double density,
                     std::vector<std::vector<bool>> &maze,
                     std::pair<int64_t, int64_t> &start,
                     std::pair<int64_t, int64_t> &end) {
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

    int64_t x, y;

    x = rnd(1, n - 2);
    y = rnd(1, m - 2);
    maze[y][x] = false;
    start = {x, y};

    x = rnd(1, n - 2);
    y = rnd(1, m - 2);
    maze[y][x] = false;
    end = {x, y};
}
