#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>




int main() {

    std::ifstream file("test.txt");
    std::string line;
    std::vector<std::vector<std::string>> keys;
    std::vector<std::vector<std::string>> locks;

    int lock_i = 0, key_i = 0, current_grid = 0;
    while (std::getline(file, line)) {
        if (line.size() == 0) {

        } else {
            
        }
    }



}