#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <array>
#include <chrono>
#include <map>
#include <queue>

struct Visited {
    bool fork;
    bool left;
    bool right;
    bool up;
    bool down;
};

struct Cell
{
    int x;
    int y;
};


bool run = true;

std::map<std::pair<int, int>, struct Visited*> branches;

bool direction_chagned(char last_dir, char next_direction) {
    if (last_dir == next_direction) {
        return false;
    } else {
        return true;
    }
}

bool doesnot_contain(const std::vector<std::pair<int, int>> &visited, const std::pair<int, int> &thing) {
    for (size_t i = 0; i < visited.size(); i++) {
        if (visited[i] == thing) {
            return false; // Found in visited
        }
    }
    return true; // Not found
}


void print_grid(std::vector<std::vector<char>> &grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int k = 0; k < grid[0].size(); k++) {
            std::cout << grid[i][k];
            if (grid[i][k] == 'S') {
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void recurse(std::vector<std::vector<char>> &grid, std::vector<int> &totals,  std::vector<std::pair<int, int>> &visited, int x = 0, int y = 0,  int total = 0, char last_dir = 's') {
    bool enable[4] = {false, false, false, false};
    int temp_total[4] = {0,0,0,0};
 
    if (grid[y][x] == 'E') {
        totals.push_back(total);
        return;
    } 

    if (branches[{x,y}]->fork) {
        if (grid[y][x-1] != '#' && branches[{x-1,y}]->left) {
            enable[0] = true;
        } 
        
        if (grid[y][x+1] != '#' && branches[{x+1,y}]->right) {
            enable[1] = true;
        } 
        
        if (grid[y-1][x] != '#' && branches[{x,y-1}]->up) {
            enable[2] = true;
        } 
        
        if (grid[y+1][x] != '#' && branches[{x,y+1}]->down) {
            enable[3] = true;
        }

    } else {
        if (grid[y][x-1] != '#' && doesnot_contain(visited, {x-1,y})) {
            enable[0] = true;
        } 
        
        if (grid[y][x+1] != '#' && doesnot_contain(visited, {x+1,y})) {
            enable[1] = true;
        } 
        
        if (grid[y-1][x] != '#' && doesnot_contain(visited, {x,y-1})) {
            enable[2] = true;
        } 
        
        if (grid[y+1][x] != '#' && doesnot_contain(visited, {x,y+1})) {
            enable[3] = true;
        }
    }

    int amount_enabled = 0;
    for (int i = 0; i < 4; i++) {
        if (enable[i]) {
            amount_enabled++;
        }
    }

    bool test = false;
    

    if (amount_enabled > 1) {
        if (branches[{x,y}]->fork == false) {
            branches[{x,y}]->fork = true;
        }
        if (enable[0]) {
            branches[{x-1,y}]->left = true;
        }
        if (enable[1]) {
            branches[{x+1,y}]->right = true;
        }
        if (enable[2]) {
            branches[{x,y-1}]->up = true;
        }
        if (enable[3]) {
            branches[{x,y+1}]->down = true;
        }
    }

    if (amount_enabled == 0) {
        return;
    } else {
        if (doesnot_contain(visited, {x,y})) {
            visited.push_back({x,y});
            test = true;
        }
        
        grid[y][x] = 'O';
        if (enable[0]) {
            if (direction_chagned(last_dir, 'l')) {
                temp_total[0] += 1000; 
            }
            recurse(grid, totals, visited, x-1, y, total+1+temp_total[0], 'l');
        }
        
        if (enable[1]) {
            if (direction_chagned(last_dir, 'r')) {
                temp_total[1] += 1000; 
            }
            recurse(grid, totals, visited, x+1, y, total+1+temp_total[1], 'r');
        }

        if (enable[2]) {
            if (direction_chagned(last_dir, 'u')) {
                temp_total[2] += 1000; 
            }
            recurse(grid, totals, visited, x, y-1, total+1+temp_total[2], 'u');
        }

        if (enable[3]) {
            if (direction_chagned(last_dir, 'd')) {
                temp_total[3] += 1000; 
            }
            recurse(grid, totals, visited, x, y+1, total+1+temp_total[3], 'd');
        }
    }

    if (!visited.empty() && test == true) {
        if (branches[{x,y}]->fork == true) {
            if (enable[0]) {
                branches[{x-1,y}]->left = false;
            }
            if (enable[1]) {
                branches[{x+1,y}]->right = false;
            }
            if (enable[2]) {
                branches[{x,y-1}]->up = false;
            }
            if (enable[3]) {
                branches[{x,y+1}]->down = false;
            }
        }
        visited.pop_back();
    }

}

void recurse(std::vector<std::vector<char>> &grid, int new_x = 0, int new_y = 0) {
    std::queue<Cell*> process;

    


}

int main() {
    int total = 0;
    std::ifstream file("test.txt");
    std::vector<std::vector<char>> grid;
    std::vector<int> totals;
    std::vector<std::pair<int, int>> visited;
    auto start = std::chrono::high_resolution_clock::now();

    if (file.is_open()) {
        std::string line;
        std::vector<char> temp_char_line;
        while(std::getline(file, line)) {
            for (char charactor : line) {
                temp_char_line.push_back(charactor);
            }
            grid.push_back(temp_char_line);
            temp_char_line.clear();
        }

        file.close();
    }

    int x = 0, y = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int k = 0; k < grid[0].size(); k++) {
            if (grid[i][k] == 'S') {
                x = k;
                y = i;
            }
            branches[{k,i}] = new Visited{false,false,false,false,false};
        }
    }


    
    recurse(grid, totals, visited, x, y);
    std::sort(totals.begin(), totals.end());

    total = totals[0];
    

    std::cout << "AOC Day 16 Part 1 Total: " << total << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms\n";
}