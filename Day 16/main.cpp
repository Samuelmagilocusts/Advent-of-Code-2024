#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <chrono>
#include <windows.h>

struct Cell {
    int x, y, steps, turns;
    char last_dir;
    std::vector<std::pair<int, int>> path;

    // Priority queue comparator (smaller total cost comes first)
    bool operator<(const Cell &other) const {
        int cost = turns;
        int other_cost = other.turns;
        return cost > other_cost;
    }

    Cell(int x_, int y_, int steps_, int turns_, char last_dir_, const std::vector<std::pair<int, int>> &path_)
        : x(x_), y(y_), steps(steps_), turns(turns_), last_dir(last_dir_), path(path_) {}
};


std::vector<std::pair<int,int>> chosen_path;

int bfs(const std::vector<std::vector<char>> &grid, int start_x, int start_y) {
    int max_size = grid.size();
    std::priority_queue<Cell> process;
    std::vector<std::vector<std::map<char, int>>> vis(max_size, std::vector<std::map<char, int>>(max_size));

    // Movement directions and corresponding labels
    int nums[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    char dirs[4] = {'l', 'u', 'r', 'd'};

    // Start cell
    process.push(Cell(start_x, start_y, 0, 0, 's',{}));

    while (!process.empty()) {
        Cell current = process.top();
        process.pop();

        // Check if we reached the endpoint
        if (grid[current.y][current.x] == 'E') {
            chosen_path = current.path;
            return current.turns * 1000 + current.steps;
        }

        // Explore all 4 directions
        for (int i = 0; i < 4; i++) {
            int new_x = current.x + nums[i][0];
            int new_y = current.y + nums[i][1];
            char new_dir = dirs[i];

            // Check boundaries and obstacles
            if (grid[new_y][new_x] != '#') {
                std::vector<std::pair<int, int>> new_path = current.path;
                new_path.push_back({new_x, new_y});
                int new_steps = current.steps + 1;
                int new_turns = current.turns + (current.last_dir != new_dir ? 1 : 0);

                // Check if this path is better
                if (!vis[new_y][new_x].count(new_dir) || new_steps < vis[new_y][new_x][new_dir]) {
                    vis[new_y][new_x][new_dir] = new_steps;
                    process.push(Cell(new_x, new_y, new_steps, new_turns, new_dir, new_path));
                }
            }
        }
    }

    return -1; // If endpoint is unreachable
}

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void print_grid(const std::vector<std::vector<char>>& grid) {
    for (const auto& line : grid) {
        for (char thing : line) {
            if (thing == '#') {
                setColor(10); 
            } else if (thing == 'O') {
                setColor(5); 
            } else if (thing == '.') {
                setColor(11); 
            } else {
                setColor(6); // Default White for other characters
            }

            std::cout << thing << " ";
        }
        std::cout << "\n";
    }
    setColor(7); // Reset to default color
    std::cout << "\n";
}

int main() {
    std::ifstream file("input.txt");
    std::vector<std::vector<char>> grid;

    auto start = std::chrono::high_resolution_clock::now();

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            grid.push_back(std::vector<char>(line.begin(), line.end()));
        }
        file.close();
    }

    int start_x = 0, start_y = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'S') {
                start_x = j;
                start_y = i;
            }
        }
    }

    int result = bfs(grid, start_x, start_y);

    for (int i = 0; i < chosen_path.size(); i++) {
        int x = chosen_path[i].first;
        int y = chosen_path[i].second;

        if (i == chosen_path.size()-1) {
            grid[y][x] = 'E';
        } else {
            grid[y][x] = 'O';
        }

    }

    print_grid(grid);

    std::cout << "AOC Day 16 Part 1 Total: " << result << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    return 0;
}
