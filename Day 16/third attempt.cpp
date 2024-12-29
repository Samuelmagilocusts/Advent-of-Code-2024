#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <chrono>
#include <windows.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <unordered_set>

struct Pair {
    int x, y;

    Pair(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
};

void delete_everything(std::map<int, std::vector<Pair*>> &one, std::map<int, std::vector<Pair*>> &two) {
    std::unordered_set<Pair*> deleted;

    for (int i = 0; i < one.size(); i++) {
        for (Pair* cell : one[i]) {
            if (cell && deleted.find(cell) == deleted.end()) {
                delete cell;
                deleted.insert(cell); // Mark as deleted
            }
        }
    }

    for (int i = 0; i < two.size(); i++) {
        for (Pair* cell : two[i]) {
            if (cell && deleted.find(cell) == deleted.end()) {
                delete cell;
                deleted.insert(cell); // Mark as deleted
            }
        }
    }
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

bool not_contains_pair(const std::vector<Pair*> &vec, const int x, const int y) {
    for (Pair *cell : vec) {
        if (cell->x == x && cell->y == y) {
            return false;
        }
    }
    return true;
}

void load_grid_print(const std::vector<std::vector<char>> &grid, std::vector<Pair*> &path) {
    std::vector<std::vector<char>> new_grid = grid;
    for (Pair *cell : path) {
        new_grid[cell->y][cell->x] = 'O';
    }

    print_grid(new_grid);
}

void print_corners(const std::vector<std::vector<char>> &grid, std::map<int, std::vector<Pair*>> &x_axis, std::map<int, std::vector<Pair*>> &y_axis) {
    std::vector<std::vector<char>> new_grid = grid;
    for (auto &[key, value] : x_axis) {
        for (Pair *cell : value) {
            new_grid[cell->y][cell->x] = 'O';
        }
    }
    for (auto &[key, value] : y_axis) {
        for (Pair *cell : value) {
            new_grid[cell->y][cell->x] = 'O';
        }
    }
    print_grid(new_grid);
}

void kill_dead_ends(std::vector<std::vector<char>> &grid, int x, int y) {
    if (grid[y][x] == '.') {
        
        int nums[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
        char dirs[4] = {'l', 'u', 'r', 'd'};
        int branch = 0;
        int new_x = 0, new_y = 0;

        for (int i = 0; i < 4; i++) {
            int temp_x = x+nums[i][0];
            int temp_y = y+nums[i][1];
            if (grid[temp_y][temp_x] != '#') {
                new_x = temp_x;
                new_y = temp_y;
                branch++;
            }
        }
        
        if (branch == 1) {
            grid[y][x] = '#';
            kill_dead_ends(grid, new_x, new_y);
        }
    }
}

std::vector<Pair*> get_options(const std::vector<std::vector<char>> &grid, std::map<int, std::vector<Pair*>> &x_axis, std::map<int, std::vector<Pair*>> &y_axis, const Pair *current_pos) {
    std::vector<Pair*> options;
    int current_x = current_pos->x;
    int current_y = current_pos->y;

    for (Pair* corner : x_axis[current_x]) { // gets vertical values actually
        int x = corner->x;
        int y = corner->y;
        bool in_line = true;
        if (current_y < y) {
            for (int i = current_y+1; i <= y; i++) { // searches the down
                if (grid[i][x] == '#') {
                    in_line = false;
                    break;
                }
            }
        } else {
            for (int i = y; i < current_y; i++) { // searches the up
                if (grid[i][x] == '#') {
                    in_line = false;
                    break;
                }
            }
        }
        if (in_line && std::find(options.begin(), options.end(), corner) == options.end() && corner != current_pos) {
            options.push_back(corner);
        }
    }

    for (Pair* corner : y_axis[current_y]) { // gets horizontal values actually
        int x = corner->x;
        int y = corner->y;
        bool in_line = true;
        if (current_x < x) {
            for (int i = current_x+1; i <= x; i++) { // searches the right
                if (grid[y][i] == '#') {
                    in_line = false;
                    break;
                }
            }
        } else {
            for (int i = x; i < current_x; i++) { // searches the left
                if (grid[y][i] == '#') {
                    in_line = false;
                    break;
                }
            }
        }
        if (in_line && std::find(options.begin(), options.end(), corner) == options.end() && corner != current_pos) {
            options.push_back(corner);
        }
    }

    // for (auto &[y, x] : options) {
    //     std::cout << x << " : " << y << "\n";
    // }

    return options;
}

int path_total(std::vector<Pair*> path) {
    int num_of_steps = 0;
    int num_of_corners = (path.size()-1) * 1000;
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) {
            num_of_steps += abs(path[i]->y - path[i-1]->y) + abs(path[i]->x - path[i-1]->x);
        }
    }

    return num_of_corners + num_of_steps;
}

void shortest_path(const std::vector<std::vector<char>> &grid,
    std::map<int, std::vector<Pair*>> &x_axis, 
    std::map<int, std::vector<Pair*>> &y_axis,
    std::vector<std::vector<Pair*>> &paths,
    Pair *start,
    Pair *end,
    const int MAX) 
{
    std::vector<Pair*> current_path;
    std::queue<std::vector<Pair*>> process;

    process.push({start});
    
    while (!process.empty()) {
        std::vector<Pair*> current_path = process.front();
        process.pop();

        if (current_path.size() > MAX) {
            continue;
        }

        const Pair *cell = current_path[current_path.size() - 1];

        if (cell == end) {
            paths.push_back(current_path);
            continue;
        }

        std::vector<Pair*> options = get_options(grid, x_axis, y_axis, cell);

        for (Pair *corner : options) {
            if (std::find(current_path.begin(), current_path.end(), corner) == current_path.end()) {
                std::vector<Pair*> new_path = current_path;
                new_path.push_back(corner);
                process.push(new_path);
            }
        }
    }
}



int main() {
    int MAX = 12; // 7+1 for test, 11+1 for test2, 80+1 for input
    std::ifstream file("test2.txt");
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<int>> dead_ends;
    std::map<int, std::vector<Pair*>> x_axis;
    std::map<int, std::vector<Pair*>> y_axis;
    std::vector<std::vector<Pair*>> paths;
    Pair *start_pair = new Pair();
    Pair *end_pair = new Pair();
    int total = 0;

    auto start = std::chrono::high_resolution_clock::now();

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            grid.push_back(std::vector<char>(line.begin(), line.end()));
        }
        file.close();
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'S') {
                start_pair->x = j;
                start_pair->y = i;
                x_axis[j].push_back(start_pair);
                y_axis[i].push_back(start_pair);
            }

            if (grid[i][j] == 'E') {
                end_pair->x = j;
                end_pair->y = i;
                x_axis[j].push_back(end_pair);
                y_axis[i].push_back(end_pair);
            }
        }
    }

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == '.') {
                int deadend = 0;
                if (grid[y][x+1] == '#') deadend++;
                if (grid[y][x-1] == '#') deadend++;
                if (grid[y+1][x] == '#') deadend++;
                if (grid[y-1][x] == '#') deadend++;

                if (deadend == 3) {
                    dead_ends.push_back({y,x});
                } 
            }
        }
    }

    for (std::vector<int> &vec : dead_ends) {
        int x = vec[1], y = vec[0];
        kill_dead_ends(grid, x, y);
    }

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == '.') {
                int hor = 0, ver = 0;
                if (grid[y][x+1] == '#') hor++;
                if (grid[y][x-1] == '#') hor++;
                if (grid[y+1][x] == '#') ver++;
                if (grid[y-1][x] == '#') ver++;
                
                if (hor != 2 && hor != 0) {
                    if (not_contains_pair(y_axis[y], x, y)) {
                        y_axis[y].push_back(new Pair(x, y));
                    }
                    if (not_contains_pair(x_axis[x], x, y)) {
                        x_axis[x].push_back(new Pair(x, y));
                    }
                }

                if (ver != 2 && ver != 0) {
                    if (not_contains_pair(y_axis[y], x, y)) {
                        y_axis[y].push_back(new Pair(x, y));
                    }
                    if (not_contains_pair(x_axis[x], x, y)) {
                        x_axis[x].push_back(new Pair(x, y));
                    }
                }
            }
        }
    }

    std::vector<std::vector<char>> new_grid = grid;
    print_corners(new_grid, x_axis, y_axis);
    
    shortest_path(grid, x_axis, y_axis, paths, start_pair, end_pair, MAX);

    if (!paths.empty()) {

        int path_index = 0;
        for (int i = 0; i < paths.size(); i++) {
            int result = path_total(paths[i]);
            if (total == 0 || result < total) {
                total = result;
                path_index = i;
            }
            // std::vector<std::vector<char>> new_grid2 = grid;
            // load_grid_print(new_grid2, paths[i]);
        }

        for (int i = 0; i < paths[path_index].size(); i++) {
            int x = paths[path_index][i]->x;
            int y = paths[path_index][i]->y;

            if (i == paths[path_index].size()-1) {
                grid[y][x] = 'E';
            } else {
                grid[y][x] = 'O';
            }

        }
    }

    print_grid(grid);

    std::cout << "AOC Day 16 Part 1 Total: " << total << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    delete_everything(x_axis, y_axis);

    return 0;
}