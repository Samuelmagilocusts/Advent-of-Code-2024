#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <set>
#include <unordered_map>
#include <unordered_set>

// took 45 min to run input part 1 the first time. now takes 5 seconds. now to 3 seconds by cuttening iteration in half..
// part 2 takes at least 3 minutes if you start at the highest number. 

int check = 0;
void get_lan(const std::unordered_map<std::string, std::vector<std::string>> &connected, std::set<std::vector<std::string>> &results_set, const std::string selected, const int max, std::vector<std::string> &result, const int pos = 0) {
    if (pos >= max) {
        results_set.insert(result);
        // if (check % 10000 == 0) std::cout << "check " << check << std::endl;
        // check++;
        return;
    }

    for (const std::string &item : connected.at(selected)) {
        if (result.empty() || item > result.back()) {
            bool connects_to_all = true;
            std::unordered_set<std::string> connected_set(connected.at(item).begin(), connected.at(item).end());
            for (int i = 0; i < pos; ++i) {
                if (connected_set.find(result[i]) == connected_set.end()) {
                    connects_to_all = false;
                    break;
                }
            }

            if (connects_to_all) {
                result.push_back(item);
                get_lan(connected, results_set, item, max, result, pos + 1);
                result.pop_back(); 
            }
        }
    }
}

int count_ts(std::set<std::vector<std::string>> &results_set, bool print = false) {
    int total = 0;
    for (const auto& vec : results_set) {
        bool contains_t = false;
        for (const auto& item : vec) {
            if (item[0] == 't') {
                contains_t = true;
            }
            if (print) std::cout << item << ",";
        }
        if (print) std::cout << "\n";
        if (contains_t) {
            total++;
        }
    }
    return total;
}

int main() {

    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::vector<std::string>> pairs;
    std::unordered_map<std::string, std::vector<std::string>> connected;
    std::set<std::vector<std::string>> results_set;
    std::set<std::vector<std::string>> last_results_set;
    std::vector<std::string> alpha;
    std::vector<std::string> result;
    int total = 0, total_p2 = 0;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> all_totals;
    bool p2 = true;
    
    while (std::getline(file, line)) {
        std::string left = line.substr(0, 2);
        std::string right = line.substr(3);
        std::vector<std::string> pair;
        if (std::find(alpha.begin(), alpha.end(), left) == alpha.end()) {
            alpha.push_back(left);
        }

        if (std::find(alpha.begin(), alpha.end(), right) == alpha.end()) {
            alpha.push_back(right);
        }
        
        pair.push_back(left);
        pair.push_back(right);
        pairs.push_back(pair);
    }

    line.clear();
    file.close();

    for (const std::string &lan : alpha) {
        for (const std::vector<std::string> pair : pairs) {
            std::string key = pair[0];
            std::string value = pair[1];

            if (lan == value) {
                if (std::find(connected[lan].begin(), connected[lan].end(), key) == connected[lan].end()) {
                    connected[lan].push_back(key);
                }
            } else if (lan == key) {
                if (std::find(connected[lan].begin(), connected[lan].end(), value) == connected[lan].end()) {
                    connected[lan].push_back(value);
                }
            }
        }
    }

    // part 1
    if (p2) {
        int size = 3;
        while (true) {
            auto start_p2 = std::chrono::high_resolution_clock::now();
            std::cout << "Trying Size: " << size << "\n";
            
            for (const std::string &lan : alpha) {
                get_lan(connected, results_set, lan, size, result);
            }
            
            std::cout << "Results size: " << results_set.size() << "\n" ;
            if (results_set.empty()) break; 
            all_totals.push_back(size);
            last_results_set = results_set;
            results_set.clear();
            result.clear();
            size++;
            auto end_p2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_p2 - start_p2);
            std::cout << "Time elapsed: " << duration.count() << " milliseconds\n";
        }

        total_p2 = all_totals.back();
        std::vector<std::string> answer = *last_results_set.begin();
        std::sort(answer.begin(),answer.end());
        std::cout << "Answer: ";
        for (int i = 0; i < answer.size(); i++) {
            std::cout << answer[i];
            if (i != answer.size()-1) {
                std::cout << ",";
            }
        }
        std::cout << "\n";
        
    } else {
        for (const std::string &lan : alpha) {
            get_lan(connected, results_set, lan, 3, result);
        }

        total = count_ts(results_set);
    }

    

    if (p2) {
        std::cout << "AOC Day 23 Part 2 Group Size: " << total_p2 << "\n";
    } else {
        std::cout << "AOC Day 23 Part 1 Total: " << total << "\n";
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time elapsed: " << duration.count() << " milliseconds\n";

    return 0;
}