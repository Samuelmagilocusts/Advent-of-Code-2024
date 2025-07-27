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
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <utility>
#include <list>

std::mutex paths_mutex;
std::mutex visited_mutex;

struct yoyo_linked_list {
    yoyo_linked_list* last;
    std::pair<int, int> value;
    uint64_t size;
    yoyo_linked_list* next;

    yoyo_linked_list() : last(0), value({0,0}), size(1), next(0) {}
};

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

void load_grid_print(const std::vector<std::vector<char>> &grid, yoyo_linked_list* path, bool forward = false) {
    std::vector<std::vector<char>> new_grid = grid;
    while (path) {
        if (new_grid[path->value.first][path->value.second] != 'S' && new_grid[path->value.first][path->value.second] != 'E')
        new_grid[path->value.first][path->value.second] = 'O';
        std::cout << "(" << path->value.first << "," << path->value.second << "), ";
        if (forward) {
            path = path->next;
        } else {
            path = path->last;
        }
    }
    std::cout << "\n";

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

void print_dead(std::set<std::pair<std::pair<int, int>, char>>& dead, std::vector<std::vector<char>>& grid) {
    std::vector<std::vector<char>> new_grid = grid;
    for (std::pair<std::pair<int, int>, char> data : dead) {
        grid[data.first.first][data.first.second] = 'x';
    }
    print_grid(new_grid);
}

void kill_dead_ends(std::vector<std::vector<char>> &grid, int x = 0, int y = 0) {
    if (grid[y][x] == '.') {
        
        int nums[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
        // char dirs[4] = {'l', 'u', 'r', 'd'};
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

std::vector<std::pair<int, int>> get_options(const std::vector<std::vector<char>> &grid, std::map<int, std::vector<std::pair<uint64_t, uint64_t>>> &x_axis, std::map<int, std::vector<std::pair<uint64_t, uint64_t>>> &y_axis, std::pair<int, int> current_pos) {
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

    return options;
}

int path_total(yoyo_linked_list* input_path, bool forward) {
    yoyo_linked_list* path = input_path;
    int num_of_steps = 0;
    int num_of_corners = (path->size-1) * 1000;
    for (size_t i = path->size-1; i > 0; i--) {
        if (i > 0) {
            num_of_steps += abs(path->value.first - path->last->value.first) + abs(path->value.second - path->last->value.second);
        }
        if (forward) {
            path = path->next;
        } else {
            path = path->last;
        }
    }

    return num_of_corners + num_of_steps;
}

bool list_contains(yoyo_linked_list* input_list, const std::pair<int, int> input, bool forword = true) {
    yoyo_linked_list* list = input_list;
    while (list) {
        if (list->value == input) return 1;
        if (forword) {
            list = list->next;
        } else {
            list = list->last;
        }
    }
    return 0;
}

void domino_effect(yoyo_linked_list* input_list, std::set<std::pair<int, int>>& intersection, std::set<std::pair<std::pair<int, int>, char>>& unuseful, char dir) {
    yoyo_linked_list* list = input_list;

    while (list && unuseful.find({list->value, dir}) == unuseful.end() && intersection.find(list->value) == intersection.end()) {
        unuseful.insert({list->value, dir});
        list = list->last;
    }
}

char get_new_direction(std::pair<int, int> one, std::pair<int, int> two) {
    if (one.first == two.first) { // horizontal
        if (one.second < two.second) { // should never be equal. the where i've been uniqueness check should take care of this.
            return 'r';
        } else {
            return 'l';
        }


    } else if (one.second == two.second) { // vertical
        if (one.first < two.first) {
            return 'd';
        } else {
            return 'u';
        }
    } else {
        return 0;
    }
}

uint64_t gen_hash_from_linklist(yoyo_linked_list* endpoint) {
    uint64_t output = 0;
    yoyo_linked_list* here = endpoint;
    while (here) {
        output ^= here->value.first + here->value.second * 140 + 0x9e3779b9;
        here = here->last;
    }
    return output;
}

yoyo_linked_list* find_path(
    std::map<std::pair<uint64_t, uint64_t>, std::vector<std::pair<uint64_t, uint64_t>>>& cornors, 
    std::pair<uint64_t, uint64_t> start, 
    std::pair<uint64_t, uint64_t> end, 
    std::vector<yoyo_linked_list*>& paths,
    std::vector<std::vector<char>>& grid
) {
    uint64_t offset = sizeof(yoyo_linked_list), max_offset = sizeof(yoyo_linked_list) * 1000000000;
    yoyo_linked_list* start_eleemnt = (yoyo_linked_list*)malloc(max_offset);
    std::set<uint64_t> result_hashes;

    start_eleemnt->last = 0; start_eleemnt->next = 0; start_eleemnt->size = 1; start_eleemnt->value = start;

    std::queue<yoyo_linked_list*> my_q;
    my_q.push(start_eleemnt);
    std::set<std::pair<uint64_t, uint64_t>> used;

    while (!my_q.empty()) {
        yoyo_linked_list* current = my_q.front();
        std::pair<uint64_t, uint64_t> current_pair = current->value;
        my_q.pop();

        if (current_pair == end) {
            uint64_t result_hash = gen_hash_from_linklist(current);
            if (result_hashes.find(result_hash) == result_hashes.end()) {
                paths.push_back(current);
                result_hashes.insert(result_hash);
            }
            continue;
        };

        std::vector<std::pair<uint64_t, uint64_t>> yoyodude = cornors[current_pair];
        std::cout << "yoyodude: " << yoyodude.size() << " \n";

        for (std::pair<uint64_t, uint64_t> new_pair : cornors[current_pair]) {

            std::cout << "";
            if (used.find(new_pair) == used.end()) {
                yoyo_linked_list* new_data = (yoyo_linked_list*)(start_eleemnt + offset);
                if (max_offset <= offset) {
                    new_data = (yoyo_linked_list*)malloc(sizeof(yoyo_linked_list));
                }
                new_data->value = new_pair;
                new_data->next = 0;
                new_data->last = current;
                new_data->size = current->size + 1;

                my_q.push(new_data);

                offset += sizeof(yoyo_linked_list);
                used.insert(new_pair);
            }
        }
    }
    return 0;
}


int main() {
    // int MAX = INT_MAX; // 7+1 for test, 11+1 for test2, 80+1 for input
    std::ifstream file("test2.txt");
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<int,int>> dead_ends;
    std::vector<std::pair<uint64_t, uint64_t>> graph;
    std::map<std::pair<uint64_t, uint64_t>, std::vector<std::pair<uint64_t, uint64_t>>> cornors;
    std::list<std::pair<uint64_t, uint64_t>> cornors_list;
    std::map<int, std::vector<std::pair<uint64_t, uint64_t>>> x_axis;
    std::map<int, std::vector<std::pair<uint64_t, uint64_t>>> y_axis;
    std::map<std::pair<int, int>, std::vector<std::pair<int,int>>> all_options;
    std::set<std::pair<int, int>> intersection;
    std::vector<yoyo_linked_list*> paths;

    int total = 0;

    auto start = std::chrono::high_resolution_clock::now();

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            grid.push_back(std::vector<char>(line.begin(), line.end()));
        }
        file.close();
    }

    // TODODODODO Change this so that the cornors gets the next cornor. currently it is grabbing the next adjacent node.

    int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'S') {
                start_x = j;
                start_y = i;
                x_axis[j].push_back({i,j});
                y_axis[i].push_back({i,j});
                // for (std::pair<int, int> r : std::vector<std::pair<int, int>>{{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}}) {
                //     if (grid[r.first][r.second] == '.') {
                //         cornors[{i,j}].push_back(r);
                //     }
                // }

            }

            if (grid[i][j] == 'E') {
                end_x = j;
                end_y = i;
                x_axis[j].push_back({i,j});
                y_axis[i].push_back({i,j});
                // for (std::pair<int, int> r : std::vector<std::pair<int, int>>{{i + 1, j}, {i - 1, j}, {i, j + 1}, {i, j - 1}}) {
                //     if (grid[r.first][r.second] == '.') {
                //         cornors[{i,j}].push_back(r);
                //     }
                // }
            }
        }
    }

    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t g = 0; g < grid[i].size(); g++) {
            if (grid[i][g] == '.') {
                graph.push_back({i, g});
            }   
        }
    }


    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == '.') {
                int hor = 0, ver = 0, q = 4;
                if (grid[y][x+1] == '#') {
                    hor++;
                    q--;
                }
                if (grid[y][x-1] == '#') {
                    hor++;
                    q--;
                }
                if (grid[y+1][x] == '#') {
                    ver++;
                    q--;
                }
                if (grid[y-1][x] == '#') {
                    ver++;
                    q--;
                }

                if ((ver != 2 && ver != 0) || (hor != 2 && hor != 0)) {
                    cornors_list.push_back({y,x});
                }
                
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

                    if (q <= 2 && intersection.find({y,x}) == intersection.end()) {
                        intersection.insert({y,x});
                    }

                }
            }
        }
    }

    for (int i = 0; i < grid.size(); i++) {
        
    }

    std::ofstream dump("dump");
    
    for (const std::pair<const std::pair<uint64_t, uint64_t>, std::vector<std::pair<uint64_t, uint64_t>>>& kv : cornors) {
        const std::pair<uint64_t, uint64_t>& key = kv.first;
        const std::vector<std::pair<uint64_t, uint64_t>>& value = kv.second;
        dump << "Key: " << key.first << ":" << key.second << " values: ";
        for (const std::pair<uint64_t, uint64_t>& p_value : value) {
            dump << p_value.first << ":" << p_value.second << ", ";
        }

        dump << "\n";
    }

    dump.close();

    yoyo_linked_list* result = find_path(cornors, {start_y, start_x}, {end_y, end_x}, paths, grid);

    if (!paths.empty()) {
        yoyo_linked_list* best_node;
        std::cout << "Amount of paths " << paths.size() << "\n";
        for (size_t i = 0; i < paths.size(); i++) {
            int result = path_total(paths[i], false);
            if (total == 0 || result < total) {
                total = result;
                best_node = paths[i];
            }
            // load_grid_print(grid, paths[i]);
        }

        load_grid_print(grid, best_node);
    }


    std::cout << "AOC Day 16 Part 1 Total: " << total << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    return 0;
}