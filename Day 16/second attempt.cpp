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

std::vector<std::vector<std::pair<int,int>>> chosen_paths;
int MAX = 0;

struct Vis_Cell {
    bool left, right, up, down;

    Vis_Cell() : left(false), right(false), up(false), down(false) {}

    Vis_Cell(bool l, bool r, bool u, bool d) : left(l), right(r), up(u), down(d) {}
};

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

int path_total(std::vector<std::pair<int,int>> path, std::pair<int, int> origin) {
    int num_of_steps = abs(path[0].first - origin.first) + abs(path[0].second - origin.second);
    int num_of_corners = path.size() * 1000;
    for (int i = 0; i < path.size(); i++) {
        if (i > 0) {
            num_of_steps += abs(path[i].first - path[i-1].first) + abs(path[i].second - path[i-1].second);
        }
    }

    return num_of_corners + num_of_steps;
}

char get_direction(std::pair<int, int> start, std::pair<int, int> end) {
    if (start.first == end.first) {
        if (start.second < end.second) {
            return 'r';
        } else if (end.second < start.second) {
            return 'l';
        }
    }

    if (start.second == end.second) {
        if (start.first < end.first) {
            return 'd';
        } else if (end.first < start.first) {
            return 'u';
        }
    }

    return '0';
}

void update_last_cell(std::map<int, std::vector<std::pair<int,int>>> &x_axis, std::map<int, std::vector<std::pair<int,int>>> &y_axis, std::pair<int, int> current_pos, std::map<std::pair<int, int>, Vis_Cell*> cells, char dir) {
    std::pair<int, int> new_key;
    int index = 0, sel_key = 0;
    if (dir == 'l' || dir == 'r') {
        for (auto &[key, vec] : y_axis) {
            auto pos_index = std::find(vec.begin(), vec.end(), current_pos);
            if (pos_index != vec.end()) {
                index = std::distance(vec.begin(), pos_index);
                sel_key = key;
                break;
            }
        }
        new_key = y_axis[sel_key][index];
    } else if (dir == 'u' || dir == 'd') {
        for (auto &[key, vec] : x_axis) {
            auto pos_index = std::find(vec.begin(), vec.end(), current_pos);
            if (pos_index != vec.end()) {
                index = std::distance(vec.begin(), pos_index);
                sel_key = key;
                break;
            }
        }
        new_key = x_axis[sel_key][index];
    }

    switch (dir) {
        case 'l':
            cells[new_key]->left = true;
            break;
        case 'r':
            cells[new_key]->right = true;
            break;
        case 'u':
            cells[new_key]->up = true;
            break;
        case 'd':
            cells[new_key]->down = true;
            break;
    }
}

bool open_dir(Vis_Cell *last_cell, char dir) {
    switch (dir) {
        case 'l':
            return last_cell->left == false;
        case 'r':
            return last_cell->right == false;
        case 'u':
            return last_cell->up == false;
        case 'd':
            return last_cell->down == false;
    }
}

int test = 0;

void shortest_path(const std::vector<std::vector<char>> &grid,
    std::map<int, std::vector<std::pair<int,int>>> &x_axis, 
    std::map<int, std::vector<std::pair<int,int>>> &y_axis, 
    std::pair<int, int> current_pos, 
    std::vector<std::vector<bool>> &visited,
    std::vector<std::pair<int,int>> &path,
    std::pair<int, int> end_pos,
    std::map<std::pair<int, int>, Vis_Cell*> cells,
    char last_dir = 's') 
{

    test++;

    if (grid[current_pos.first][current_pos.second] == 'E') {
        chosen_paths.push_back(path);
        // std::cout << "E\n";
        return;
    }

    if (path.size() > MAX) {
        // std::cout << "Too big " << test++ << "\n";
        return;
    }

    if (std::find(path.begin(), path.end(), current_pos) != path.end()) {
        update_last_cell(x_axis, y_axis, current_pos, cells, last_dir);
    }

    visited[current_pos.first][current_pos.second] = true;
    
    std::vector<std::pair<int, int>> options = get_options(grid, x_axis, y_axis, current_pos);

    for (std::pair<int, int> corner : options) {
        int x = corner.second;
        int y = corner.first;
        char new_dir = get_direction(current_pos, corner);

        if (!visited[y][x] && open_dir(cells[{y,x}], new_dir)) {
            std::vector<std::pair<int, int>> new_path = path;
            new_path.push_back(corner);

            // if (test % 100000 == 0) {
            //     load_grid_print(grid, new_path);
            // }
            shortest_path(grid, x_axis, y_axis, corner, visited, new_path, end_pos, cells, new_dir);
        }
    }


    visited[current_pos.first][current_pos.second] = false;
}




int main() {
    MAX = 7; // 7 for test, 11 for test2, 80 for input
    std::ifstream file("test.txt");
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<int,int>> dead_ends;
    std::vector<std::pair<int,int>> path;
    std::map<int, std::vector<std::pair<int,int>>> x_axis;
    std::map<int, std::vector<std::pair<int,int>>> y_axis;
    std::map<std::pair<int, int>, Vis_Cell*> cells;
    std::vector<int> totals;
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
                        cells[{y,x}] = new Vis_Cell(false,false,false,false);
                    }
                    if (std::find(x_axis[x].begin(), x_axis[x].end(), std::make_pair(y, x)) == x_axis[x].end()) {
                        x_axis[x].push_back({y,x});
                        cells[{y,x}] = new Vis_Cell(false,false,false,false);
                    }
                }

                if (ver != 2 && ver != 0) {
                    if (std::find(y_axis[y].begin(), y_axis[y].end(), std::make_pair(y, x)) == y_axis[y].end()) {
                        y_axis[y].push_back({y,x});
                        cells[{y,x}] = new Vis_Cell(false,false,false,false);
                        
                    }
                    if (std::find(x_axis[x].begin(), x_axis[x].end(), std::make_pair(y, x)) == x_axis[x].end()) {
                        x_axis[x].push_back({y,x});
                        cells[{y,x}] = new Vis_Cell(false,false,false,false);
                    }
                }
            }
        }
    }

    // std::vector<std::vector<char>> new_grid = grid;
    // print_corners(new_grid, x_axis, y_axis);

    // print_grid(grid);
    std::vector<std::vector<bool>> vis(grid.size(), std::vector<bool>(grid.size(), false));
    shortest_path(grid, x_axis, y_axis, {start_y, start_x}, vis, path, {end_y, end_x}, cells);

    if (!chosen_paths.empty()) {

        int path_index = 0;
        for (int i = 0; i < chosen_paths.size(); i++) {
            int result = path_total(chosen_paths[i], {start_y, start_x});
            if (total == 0 || result < total) {
                total = result;
                path_index = i;
            }
            // std::vector<std::vector<char>> new_grid2 = grid;
            // load_grid_print(new_grid2, chosen_paths[i]);
        }

        for (int i = 0; i < chosen_paths[path_index].size(); i++) {
            int x = chosen_paths[path_index][i].second;
            int y = chosen_paths[path_index][i].first;

            if (i == chosen_paths[path_index].size()-1) {
                grid[y][x] = 'E';
            } else {
                grid[y][x] = 'O';
            }

        }
    }

    // print_grid(grid);


    std::cout << "AOC Day 16 Part 1 Total: " << total << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    return 0;
}