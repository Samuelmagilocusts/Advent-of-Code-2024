#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <cstdint>
#include <bitset>

struct Gate {
    std::string first;
    std::string second;
    std::string op;
    std::string result;

    Gate(std::string f, std::string s, std::string o, std::string r)
        : first(f), second(s), op(o), result(r) {}
};

int flip(int first, const std::string &op, int second) {
    if (op == "XOR") {
        return first ^ second;
    } else if (op == "OR") {
        return first | second;
    } else if (op == "AND") {
        return first & second;
    } else {
        throw std::invalid_argument("Invalid operation: " + op);
    }
}


bool validate_pairs(std::vector<int> pairs) {
    // if (pairs) {

    // }
}

void gen_combo(std::vector<std::vector<int>> results, int size_of_options, int num_of_swaps) {

}

std::vector<std::vector<int>> generate_all_combos(int size_of_options, int num_of_swaps) {
    std::vector<int> model;
    std::vector<std::vector<int>> result;
    // std::map<> completed_swaps;
    for (int i = 0; i < size_of_options; i++) {
        model.push_back(i);
    }
    gen_combo(result, size_of_options, num_of_swaps);

    return result;
}


int main() {
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



    // Process the gates
    while (!gates.empty()) {
        Gate *gate = gates.front();
        gates.pop();

        if (regs.find(gate->first) != regs.end() && regs.find(gate->second) != regs.end()) {
            regs[gate->result] = flip(regs[gate->first], gate->op, regs[gate->second]);
            delete gate;
        } else {
            gates.push(gate);
        }
    }

    uint64_t total = 0, x_t = 0, y_t = 0;
    std::vector<int> values;
    std::vector<int> x_values;
    std::vector<int> y_values;

    for (auto &[key, value] : regs) {
        if (key[0] == 'z') {
            values.push_back(value);
        } else if (key[0] == 'x') {
            x_values.push_back(value);
        } else if (key[0] == 'y') {
            y_values.push_back(value);
        }
    }

    for (int i = values.size()-1; i >= 0; i--) {
        total = (total << 1) | values[i]; 
    }

    for (int i = x_values.size()-1; i >= 0; i--) {
        x_t = (x_t << 1) | values[i]; 
    }

    for (int i = y_values.size()-1; i >= 0; i--) {
        y_t = (y_t << 1) | values[i]; 
    }

    std::bitset<64> x_binary(x_t); // Convert to 32-bit binary
    std::cout << "Binary representation x: " << x_binary << std::endl;
    std::bitset<64> y_binary(y_t); // Convert to 32-bit binary
    std::cout << "Binary representation y: " << y_binary << std::endl;
    std::bitset<64> z_binary(total); // Convert to 32-bit binary
    std::cout << "Binary representation z: " << z_binary << std::endl;
    std::bitset<64> combined_binary(x_t+y_t); // Convert to 32-bit binary
    std::cout << "Binary representation c: " << combined_binary << std::endl;
    std::cout << "Decimal representation c: " << x_t+y_t << std::endl;

    std::cout << "AOC Day 24 Total: " << total << std::endl;

    return 0;
}
