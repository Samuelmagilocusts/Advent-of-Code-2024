#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <cstdint>
#include <bitset>



int main() {
    std::fstream file("testp2.txt");
    std::fstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::string line;
    std::map<std::string, int> regs;
    std::queue<Gate*> gates;

    // Read the input file
    while (std::getline(file, line)) {
        if (line.find(':') != std::string::npos) {
            size_t split_pos = line.find(": ");
            std::string key = line.substr(0, split_pos);
            int value = std::stoi(line.substr(split_pos + 2));
            regs[key] = value;
        } else if (line.find("->") != std::string::npos) {
            size_t split_pos = line.find(" -> ");
            std::string process = line.substr(0, split_pos);
            std::string result = line.substr(split_pos + 4);

            size_t first_space = process.find_first_of(" ");
            size_t second_space = process.find_last_of(" ");
            std::string first = process.substr(0, first_space);
            std::string op = process.substr(first_space + 1, second_space - first_space - 1);
            std::string second = process.substr(second_space + 1);

            gates.push(new Gate(first, second, op, result));
        } else {
            std::cout << "Line does not contain ':' or '->': " << line << std::endl;
        }
    }

    file.close();

}