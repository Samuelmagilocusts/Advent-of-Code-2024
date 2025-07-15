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

std::mutex paths_mutex;
std::mutex visited_mutex;

struct State {
    std::vector<std::pair<int, int>> path;
    int distance = 0, turns = 0;

    State(const std::vector<std::pair<int, int>>& p, int d, int t)
        : path(p), distance(d), turns(t) {}

    bool operator<(const State& other) const {
        return distance + turns * 1000 > other.distance + other.turns * 1000;
    }
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


int test = 0;
void shortest_path(
    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> all_options,
    std::vector<std::vector<std::pair<int, int>>>& paths,
    std::pair<int, int> start,
    std::pair<int, int> end,
    const int MAX
) {
    // Priority queue for processing states (path, cost, corner count).
    std::priority_queue<State> process;
    std::set<std::pair<std::pair<int, int>, int>> visited; // Tracks visited corners with path characteristics.

    // Start with the initial state.
    process.push({{start}, 0, 0});

    auto start_time = std::chrono::high_resolution_clock::now();

    while (!process.empty()) {
        State current = process.top();
        process.pop();

        const auto& path = current.path;
        const auto& cell = path.back();
        int current_distance = current.distance;
        int current_turns = current.turns;

        if (test != current_turns) {
            std::cout << current_turns << " at: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() << "s\n";
            test = current_turns;
            // std::vector<std::vector<char>> temp_grid = grid;
            // load_grid_print(temp_grid, current_path);
        }

        // Stop if path exceeds max allowed corners.
        if (path.size() > MAX) {
            continue;
        }

        // If the end is reached, record the path.
        if (cell == end) {
            paths.push_back(path);
            return;
        }

        // Evaluate options for the current cell.
        const auto& options = all_options[cell];
        for (const auto& corner : options) {
            if (std::find(path.begin(), path.end(), corner) == path.end()) {
                // Build the new path.
                std::vector<std::pair<int, int>> new_path = path;
                new_path.push_back(corner);

                // Compute distance and turns for the new state.
                int new_distance = current_distance +
                                   abs(corner.first - cell.first) +
                                   abs(corner.second - cell.second);
                int new_turns = current_turns + 1;

                // Check if this corner and cost combination was already processed.
                if (visited.find({corner, new_distance + new_turns * 1000}) == visited.end()) {
                    visited.insert({corner, new_distance + new_turns * 1000});
                    process.push({new_path, new_distance, new_turns});
                }
            }
        }
    }
}

void process_thread(
    const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>& all_options,
    std::vector<std::vector<std::pair<int, int>>>& global_paths,
    const std::pair<int, int>& start,
    const std::pair<int, int>& end,
    const int MAX,
    const std::vector<std::pair<int, int>>& initial_options
) {
    std::priority_queue<State> process;
    std::set<std::pair<std::pair<int, int>, int>> local_visited; // Thread-local visited

    // Initialize processing queue with initial options
    for (const auto& corner : initial_options) {
        int distance = abs(corner.first - start.first) + abs(corner.second - start.second);
        process.push({{start, corner}, distance, 1});
    }

    std::vector<std::vector<std::pair<int, int>>> local_paths;
    auto start_time = std::chrono::high_resolution_clock::now();

    // Main processing loop
    while (!process.empty()) {
        State current = process.top();
        process.pop();

        const auto& path = current.path;
        const auto& cell = path.back();
        int current_distance = current.distance;
        int current_turns = current.turns;

        // Skip paths exceeding the max corners
        if (path.size() > MAX) {
            continue;
        }

        if (test != current_turns) {
            std::cout << current_turns << " at: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() << "s\n";
            test = current_turns;
            
        }

        // If the end is reached, save the path
        if (cell == end) {
            local_paths.push_back(path);
            continue;
        }

        // Process all options for the current cell
        const auto& options = all_options.at(cell);
        for (const auto& corner : options) {
            if (std::find(path.begin(), path.end(), corner) == path.end()) {
                std::vector<std::pair<int, int>> new_path = path;
                new_path.push_back(corner);

                int new_distance = current_distance +
                                   abs(corner.first - cell.first) +
                                   abs(corner.second - cell.second);
                int new_turns = current_turns + 1;

                // Ensure no duplicate states are processed
                if (local_visited.find({corner, new_distance + new_turns * 1000}) == local_visited.end()) {
                    local_visited.insert({corner, new_distance + new_turns * 1000});
                    process.push({new_path, new_distance, new_turns});
                }
            }
        }
    }

    // Merge thread-local paths into global paths
    std::lock_guard<std::mutex> lock(paths_mutex);
    global_paths.insert(global_paths.end(), local_paths.begin(), local_paths.end());
}

void shortest_path_multithreaded(
    const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>& all_options,
    std::vector<std::vector<std::pair<int, int>>>& paths,
    const std::pair<int, int>& start,
    const std::pair<int, int>& end,
    const int MAX,
    int num_threads = 24 // Default thread count
) {
    const auto& options = all_options.at(start);
    size_t total_options = options.size();
    size_t options_per_thread = (total_options + num_threads - 1) / num_threads;

    std::vector<std::thread> threads;

    // Launch threads to process divided chunks
    for (int i = 0; i < num_threads; ++i) {
        auto start_iter = options.begin() + i * options_per_thread;
        auto end_iter = options.begin() + std::min<size_t>((i + 1) * options_per_thread, total_options);

        // Ensure we don't go out of bounds
        if (start_iter >= options.end()) break;

        // Convert iterator range to a vector
        std::vector<std::pair<int, int>> chunk(start_iter, end_iter);

        threads.emplace_back(
            process_thread,
            std::cref(all_options),
            std::ref(paths),
            std::cref(start),
            std::cref(end),
            MAX,
            std::move(chunk) // Pass the chunk to the thread
        );
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }
}



int main() {
    int MAX = 12; // 7+1 for test, 11+1 for test2, 80+1 for input
    std::ifstream file("test2.txt");
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
    // shortest_path_multithreaded(all_options, paths, {start_y, start_x}, {end_y, end_x}, MAX);

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