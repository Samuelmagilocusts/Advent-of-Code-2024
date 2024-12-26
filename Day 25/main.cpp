#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>


void store_grid(std::vector<char> &c1, std::vector<char> &c2, std::vector<char> &c3, std::vector<char> &c4, std::vector<char> &c5, std::vector<std::vector<std::vector<char>>> &keys, std::vector<std::vector<std::vector<char>>> &locks) {
    std::vector<std::vector<char>> new_grid;
    new_grid.push_back(c1);
    new_grid.push_back(c2);
    new_grid.push_back(c3);
    new_grid.push_back(c4);
    new_grid.push_back(c5);

    if (c1[0] == '#' && c2[0] == '#' && c3[0] == '#' && c4[0] == '#' && c5[0] == '#') {
        locks.push_back(new_grid);
    } else {
        keys.push_back(new_grid);
    }
    
    c1.clear();
    c2.clear();
    c3.clear();
    c4.clear();
    c5.clear();
}

int count_hashtags(std::vector<char> &array) {
    int total = 0;

    for (char &item : array) {
        if (item == '#') total++;
    }

    return total-1;
}

int main() {

    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::vector<std::vector<char>>> keys;
    std::vector<std::vector<std::vector<char>>> locks;
    std::vector<char> c1, c2, c3, c4, c5;
    int total = 0;

    while (std::getline(file, line)) {
        
        if (line.size() == 0) {
            store_grid(c1, c2, c3, c4, c5, keys, locks);
        } else {
            c1.push_back(line[0]);
            c2.push_back(line[1]);
            c3.push_back(line[2]);
            c4.push_back(line[3]);
            c5.push_back(line[4]);
        }
    }
    store_grid(c1, c2, c3, c4, c5, keys, locks);
    file.close();

    for (int key_i = 0; key_i < keys.size(); key_i++) {
        for (int lock_i = 0; lock_i < locks.size(); lock_i++) {
            bool fits = true;
            for (int col = 0; col < 6; col++) {
                int key_size = count_hashtags(keys[key_i][col]);
                int lock_size = count_hashtags(locks[lock_i][col]);
                if (key_size+lock_size > 5) {
                    fits = false;
                }
            }
            if (fits) total++;
        }
    }

    std::cout << "AOC Day 25 Part 1 Total: " << total << std::endl;

}