#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    std::map<std::string, std::string> options;

    options["A<"] = "v<<";
    options["Av"] = ">v";
    options["A>"] = "v";
    options["A^"] = "<";
    options["AA"] = "A";

    options["<<"] = "A";
    options["<v"] = ">";
    options["<>"] = ">>";
    options["<^"] = ">^";
    options["<A"] = ">>^";

    options["v<"] = "<";
    options["vv"] = "A";
    options["v>"] = ">";
    options["v^"] = "^";
    options["vA"] = ">^";

    options["><"] = "<<";
    options[">v"] = "<";
    options[">>"] = "A";
    options[">^"] = "^<";
    options[">A"] = "^";

    options["^<"] = "v<";
    options["^v"] = "v";
    options["^>"] = "v>";
    options["^^"] = "A";
    options["^A"] = ">";


    char dir_pad[2][3] = {
		{'n', '^', 'A'},
		{'<', 'v', '>'},
	};

    char num_pad[4][3] = {
		{'7', '8', '9'},
		{'4', '5', '6'},
		{'1', '2', '3'},
		{'n', '0', 'A'},
	};


    const int amount_of_dir_pad_bots = 3;
    int bot_num = 0;
    int total = 0;

    // for (int t = )

    std::string path = "<A^A>^^AvvvA";

    while (bot_num < amount_of_dir_pad_bots) {
        std::string next;
        char last_char = 'A';
        for (int i = 0; i < path.size(); i++) {
            std::string start_end;
            start_end += last_char; 
            start_end += path[i];
            if (options.find(start_end) != options.end()) {
                next+=options[start_end];
            } else {
                std::cout << "yoyoyo";
            }
            last_char = path[i];
        }
        
        path = next;
        std::cout << bot_num << " : " << path << "\n";
        bot_num++;
    }

    std::cout << "AOC Day 20 Part 2 Total: " << total << "\n";


    return 0;
}