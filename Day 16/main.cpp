#include <iostream>
#include <vector>
#include <fstream>
#include <string>

bool direction_chagned(char last_dir, char next_direction) {
    if (last_dir == next_direction) {
        return false;
    } else {
        return true;
    }
}

int recurse(std::vector<std::vector<char>> &grid, int x = 0, int y = 0, int total = 0, char last_dir = 'u') {
    if (grid[y][x] == '.') {
        if (grid[y][x-1] == '.') {
            if (direction_chagned(last_dir, 'l')) {
                total += 1000;
            }
            recurse(grid, x-1, y, total, 'l');
        } else if (grid[y][x+1] == '.') {
            if (direction_chagned(last_dir, 'r')) {
                total += 1000;
            }
            recurse(grid, x+1, y, total, 'r');
        } else if (grid[y-1][x] == '.') {
            if (direction_chagned(last_dir, 'u')) {
                total += 1000;
            }
            recurse(grid, x, y-1, total, 'u');
        } else if (grid[y+1][x] == '.') {
            if (direction_chagned(last_dir, 'd')) {
                total += 1000;
            }
            recurse(grid, x, y+1, total, 'd');
        }
    } else if (grid[y][x] == 'S') {
        
    } else if (grid[y][x] == 'E') {
        return total;
    } else {
        std::cerr << "Messed up\n";
    }
}

int main() {
    int total = 0;
    std::ifstream file("input.txt");
    std::vector<std::vector<char>> grid;

    if (file.is_open()) {
        std::string line;
        std::vector<char> temp_char_line;
        while(std::getline(file, line)) {
            for (char charactor : line) {
                temp_char_line.push_back(charactor);
            }
            grid.push_back(temp_char_line);
        }

        file.close();
    }

    total = recurse(grid, 1, 13);

    std::cout << "AOC Day 16 Part 1 Total: " << total << std::endl;
}