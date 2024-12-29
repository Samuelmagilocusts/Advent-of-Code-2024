#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <chrono>
#include <windows.h>
#include <cstdlib>

struct Cell {
    int x, y, steps, turns;
    char last_dir;
    std::vector<std::pair<int, int>> path;

    bool operator<(const Cell &other) const {
        if (turns != other.turns) {
            return turns > other.turns; // Fewer turns are better
        }
        return steps > other.steps; // Fewer steps are better when turns are equal
    }

    Cell(int x_, int y_, int steps_, int turns_, char last_dir_,
         const std::vector<std::pair<int, int>> &path_)
        : x(x_), y(y_), steps(steps_), turns(turns_), last_dir(last_dir_), path(path_) {}
};

struct Vis_Cell {
    bool left, right, up, down;

    Vis_Cell() : left(false), right(false), up(false), down(false) {}

    Vis_Cell(bool l, bool r, bool u, bool d) : left(l), right(r), up(u), down(d) {}
};

std::vector<std::vector<std::pair<int,int>>> chosen_paths;
std::vector<int> totals;

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

void update_vis(std::map<std::pair<int,int>, Vis_Cell> &vis, char dir, int x, int y) {
    switch (dir) {
        case 'l':
            vis[{y, x}].left = true;
            break;
        case 'r':
            vis[{y, x}].right = true;
            break;
        case 'u':
            vis[{y, x}].up = true;
            break;
        case 'd':
            vis[{y, x}].down = true;
            break;
    }
}

bool dir_not_visited(std::map<std::pair<int,int>, Vis_Cell> &vis, char dir, int x, int y) {
    switch (dir) {
        case 'l':
            return vis[{y, x}].left == false;
        case 'r':
            return vis[{y, x}].right == false;
        case 'u':
            return vis[{y, x}].up == false;
        case 'd':
            return vis[{y, x}].down == false;
    }
}

bool last_dir_closed(std::map<std::pair<int,int>, Vis_Cell> &vis, char dir, int x, int y) {
    switch (dir) {
        case 'l':
            return vis[{y, x}].right;
        case 'r':
            return vis[{y, x}].left;
        case 'u':
            return vis[{y, x}].down;
        case 'd':
            return vis[{y, x}].up;
    }
}

int num_walls_closed(std::map<std::pair<int,int>, Vis_Cell> &vis, int x, int y) {
    int walls = 0;

    if (vis[{y,x}].left == true) walls++;
    if (vis[{y,x}].right == true) walls++;
    if (vis[{y,x}].up == true) walls++;
    if (vis[{y,x}].down == true) walls++;

    return walls;
}

char find_open_wall(std::map<std::pair<int,int>, Vis_Cell> &vis, int x, int y) {
    if (vis[{y,x}].left == false) return 'l';
    if (vis[{y,x}].right == false) return 'r';
    if (vis[{y,x}].up == false) return 'u';
    if (vis[{y,x}].down == false) return 'd';
}

bool not_backtrack(char last_dir, char new_dir) {
    switch (last_dir) {
        case 'l':
            return new_dir != 'r';
        case 'r':
            return new_dir != 'l';
        case 'u':
            return new_dir != 'd';
        case 'd':
            return new_dir != 'u';
    }
}

void bfs(std::vector<std::vector<char>> &grid, int start_x, int start_y) {
    int max_size = grid.size();
    std::priority_queue<Cell> process;
    std::map<std::pair<int, int>, Vis_Cell> vis;

    int nums[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    char dirs[4] = {'l', 'u', 'r', 'd'};

    process.push(Cell(start_x, start_y, 0, 0, 's', {}));

    while (!process.empty()) {
        Cell current = process.top();
        process.pop();

        if (grid[current.y][current.x] == 'E') {
            chosen_paths.push_back(current.path);
            totals.push_back(current.turns * 1000 + current.steps);
            break;
        }

        if (vis.find({current.y, current.x}) == vis.end()) {
            vis[{current.y, current.x}] = Vis_Cell(false, false, false, false);
        } else {
            int walls = num_walls_closed(vis, current.x, current.y);
            if (walls == 3) {
                char open_wall = find_open_wall(vis, current.x, current.y);
                update_vis(vis, open_wall, current.x, current.y);
            }
        }

        for (int i = 0; i < 4; i++) {
            int new_x = current.x + nums[i][0];
            int new_y = current.y + nums[i][1];
            char new_dir = dirs[i];

            if (grid[new_y][new_x] == '#') {
                update_vis(vis, new_dir, current.x, current.y);

            } else {
                if (dir_not_visited(vis, new_dir, current.x, current.y) && not_backtrack(current.last_dir, new_dir)) {
                    std::vector<std::pair<int, int>> new_path = current.path;
                    new_path.push_back({new_x, new_y});
                    int new_steps = current.steps + 1;
                    int new_turns = current.turns + (current.last_dir != new_dir ? 1 : 0);

                    if (grid[new_y][new_x] != 'S' && grid[new_y][new_x] != 'E') grid[new_y][new_x] = 'O';
                    // print_grid(grid);

                    if (vis.find({new_x, new_y}) != vis.end() && last_dir_closed(vis, new_dir, new_x, new_y)) {  // Update last cell
                        update_vis(vis, new_dir, current.x, current.y);
                    }

                    process.push(Cell(new_x, new_y, new_steps, new_turns, new_dir, new_path));
                }
            }
        }
    }
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

int main() {
    std::ifstream file("test.txt");
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<int,int>> dead_ends;

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

    print_grid(grid);

    bfs(grid, start_x, start_y);

    if (!chosen_paths.empty()) {
        for (int i = 0; i < chosen_paths[0].size(); i++) {
            int x = chosen_paths[0][i].first;
            int y = chosen_paths[0][i].second;

            if (i == chosen_paths[0].size()-1) {
                grid[y][x] = 'E';
            } else {
                grid[y][x] = 'O';
            }

        }
    }

    print_grid(grid);

    int total = totals.empty() ? 0 : totals[0];

    std::cout << "AOC Day 16 Part 1 Total: " << total << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    return 0;
}
