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

int calculate_heuristic(int y1, int x1, int y2, int x2) {
    return abs(y1 - y2) + abs(x1 - x2);
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

void load_grid_print(const std::vector<std::vector<char>> &grid, std::vector<std::pair<int, int>> &path) {
    std::vector<std::vector<char>> new_grid = grid;
    for (auto &[y, x] : path) {
        new_grid[y][x] = 'O';
    }

    print_grid(new_grid);
}

void print_corners(const std::vector<std::vector<char>> &grid, std::map<int, std::vector<std::pair<int,int>>> &x_axis, std::map<int, std::vector<std::pair<int,int>>> &y_axis) {
    std::vector<std::vector<char>> new_grid = grid;
    for (auto &[key, value] : x_axis) {
        for (auto &[y, x] : value) {
            new_grid[y][x] = 'O';
        }
    }
    for (auto &[key, value] : y_axis) {
        for (auto &[y, x] : value) {
            new_grid[y][x] = 'O';
        }
    }
    print_grid(new_grid);
}

void kill_dead_ends(std::vector<std::vector<char>> &grid, int x = 0, int y = 0) {
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

std::vector<std::pair<int, int>> get_options(const std::vector<std::vector<char>> &grid, std::map<int, std::vector<std::pair<int,int>>> &x_axis, std::map<int, std::vector<std::pair<int,int>>> &y_axis, std::pair<int, int> current_pos) {
    std::vector<std::pair<int, int>> options;
    int current_x = current_pos.second;
    int current_y = current_pos.first;

    for (std::pair<int, int> corner : x_axis[current_x]) { // gets vertical values actually
        int x = corner.second;
        int y = corner.first;
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

    for (std::pair<int, int> corner : y_axis[current_y]) { // gets horizontal values actually
        int x = corner.second;
        int y = corner.first;
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

int path_total(std::vector<std::pair<int,int>> path) {
    int num_of_steps = 0;
    int num_of_corners = (path.size()-1) * 1000;
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) {
            num_of_steps += abs(path[i].first - path[i-1].first) + abs(path[i].second - path[i-1].second);
        }
    }

    return num_of_corners + num_of_steps;
}

void shortest_path(std::map<std::pair<int, int>, std::vector<std::pair<int,int>>> all_options, std::vector<std::vector<std::pair<int,int>>> &paths, std::pair<int, int> start, std::pair<int, int> end, const int MAX) 
{
    std::queue<std::vector<std::pair<int, int>>> process;

    process.push({start});
    
    while (!process.empty()) {
        std::vector<std::pair<int, int>> current_path = process.front();
        process.pop();

        if (current_path.size() > MAX) {
            continue;
        }

        const std::pair<int, int> cell = current_path[current_path.size() - 1];

        if (cell == end) {
            paths.push_back(current_path);
            continue;
        }

        for (std::pair<int, int> corner : all_options[cell]) {
            if (std::find(current_path.begin(), current_path.end(), corner) == current_path.end()) {
                std::vector<std::pair<int, int>> new_path = current_path;
                new_path.push_back(corner);
                process.push(new_path);
            }
        }
    }
}

int main() {
    int MAX = 81; // 7+1 for test, 11+1 for test2, 80+1 for input
    std::ifstream file("input.txt");
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<int,int>> dead_ends;
    std::map<int, std::vector<std::pair<int,int>>> x_axis;
    std::map<int, std::vector<std::pair<int,int>>> y_axis;
    std::map<std::pair<int, int>, std::vector<std::pair<int,int>>> all_options;
    std::vector<std::vector<std::pair<int,int>>> paths;
    int total = 0;

    auto start = std::chrono::high_resolution_clock::now();

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            grid.push_back(std::vector<char>(line.begin(), line.end()));
        }
        file.close();
    }

    int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'S') {
                start_x = j;
                start_y = i;
                x_axis[j].push_back({i,j});
                y_axis[i].push_back({i,j});
            }

            if (grid[i][j] == 'E') {
                end_x = j;
                end_y = i;
                x_axis[j].push_back({i,j});
                y_axis[i].push_back({i,j});
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
                    dead_ends.push_back({x, y});
                } 
            }
        }
    }

    for (std::pair<int,int> pair : dead_ends) {
        kill_dead_ends(grid, pair.first, pair.second);
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
                    if (std::find(y_axis[y].begin(), y_axis[y].end(), std::make_pair(y, x)) == y_axis[y].end()) {
                        y_axis[y].push_back({y,x});
                    }
                    if (std::find(x_axis[x].begin(), x_axis[x].end(), std::make_pair(y, x)) == x_axis[x].end()) {
                        x_axis[x].push_back({y,x});
                    }
                }

                if (ver != 2 && ver != 0) {
                    if (std::find(y_axis[y].begin(), y_axis[y].end(), std::make_pair(y, x)) == y_axis[y].end()) {
                        y_axis[y].push_back({y,x});
                        
                    }
                    if (std::find(x_axis[x].begin(), x_axis[x].end(), std::make_pair(y, x)) == x_axis[x].end()) {
                        x_axis[x].push_back({y,x});
                    }
                }
            }
        }
    }

    for (int i = 0; i < x_axis.size(); i++) {
        for (std::pair<int, int> cell : x_axis[i]) {
            std::vector<std::pair<int, int>> options = get_options(grid, x_axis, y_axis, cell);
            if (all_options.find(cell) == all_options.end()) {
                all_options[cell] = options;
            } 
        }
    }

    for (int i = 0; i < y_axis.size(); i++) {
        for (std::pair<int, int> cell : y_axis[i]) {
            std::vector<std::pair<int, int>> options = get_options(grid, x_axis, y_axis, cell);
            if (all_options.find(cell) == all_options.end()) {
                all_options[cell] = options;
            } 
        }
    }

    // std::vector<std::vector<char>> new_grid = grid;
    // print_corners(new_grid, x_axis, y_axis);

    shortest_path(all_options, paths, {start_y, start_x}, {end_y, end_x}, MAX);

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
            int x = paths[path_index][i].second;
            int y = paths[path_index][i].first;

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

    return 0;
}