using System;
using System.Security.Cryptography.X509Certificates;

class AOCD6 {
    static void reset_grid(List<List<char>> array1, List<List<char>> array2) {
        array2.Clear();
        foreach (var innerList in array1) {
            array2.Add(new List<char>(innerList));
        }
    }

    static void print_grid(List<List<char>> array) {
        foreach (List<char> line in array) {
            foreach (char charactor in line) {
                Console.Write(charactor);
            }
            Console.Write("\n");
        }
        Console.WriteLine("\n\n\n\n");
    }

    static bool check_ahead(List<List<char>> array_2d, int vertical, int horizontal) {
        if (array_2d[vertical][horizontal] == '#' || array_2d[vertical][horizontal] == 'O') {
            return false;
        } else {
            return true;
        }
    }

    int[] get_starting_position(List<List<char>> array) {
        int h = 0, v = 0;
        foreach (List<char> line in array) {
            if (line.Contains('^')) {
                h = line.IndexOf('^');
                v = array.IndexOf(line);
            }
        }
        return [v,h];
    }

    static int[] update_directions(int vertical, int horizontal, char direction) {
        switch (direction) { // undo
            case 'u':
                vertical--;
                break;
            case 'r':
                horizontal++;
                break;
            case 'd':
                vertical++;
                break;
            case 'l':
                horizontal--;
                break;
        }
        return [vertical, horizontal];
    }

    static char get_apropriate_char(char current_char, char direction) {
        char new_char = 't';
        // if (corner) {
        //     return '+';
        // }
        switch (current_char) {
            case '.':
                if (direction == 'u' || direction == 'd') {
                    new_char = '|';
                } else {
                    new_char = '-';
                }
                break;
            case '|':
                new_char = '+';
                break;
            case '-':
                new_char = '+';
                break;
            case '+':
                return '1';
            default:
                new_char = current_char;
                break;
        }
        return new_char;
    }

    bool find_path(List<List<char>> array_2d, int vertical, int horizontal, List<List<char>> array_result, List<char> looping, char direction = 'u', int i = 0) {
        int[] new_dirctions = update_directions(vertical, horizontal, direction);
        vertical = new_dirctions[0];
        horizontal = new_dirctions[1];

        // if (looping.Count >= 3) {
        //     return true;
        // }

        if (i > 7100) { 
            return true; 
        }

        while (true) {
            if (vertical < 0 || horizontal < 0 || vertical >= array_2d.Count || horizontal >= array_2d.Count) {
                return false;
            } else {
                if (check_ahead(array_2d, vertical, horizontal)) {
                    break;
                } else {
                    switch (direction) { // undo
                        case 'u':
                            vertical++;
                            break;
                        case 'r':
                            horizontal--;
                            break;
                        case 'd':
                            vertical--;
                            break;
                        case 'l':
                            horizontal++;
                            break;
                    }

                    switch (direction) {
                        case 'u':
                            direction = 'r';
                            break;
                        case 'r':
                            direction = 'd';
                            break;
                        case 'd':
                            direction = 'l';
                            break;
                        case 'l':
                            direction = 'u';
                            break;
                    }

                    new_dirctions = update_directions(vertical, horizontal, direction);
                    vertical = new_dirctions[0];
                    horizontal = new_dirctions[1];
                }
            }
        }
        // Console.WriteLine(array_2d[vertical][horizontal]);
        char new_char = get_apropriate_char(array_result[vertical][horizontal], direction);
        if (new_char == '1') looping.Add(new_char);
        array_result[vertical][horizontal] = new_char;
        // print_grid(array_result);
        
        return find_path(array_2d, vertical, horizontal, array_result, looping, direction, i+1);
    }

    static void Main() {
        AOCD6 Aocd6 = new AOCD6();
        string[] file = File.ReadAllLines("C:/Users/Starfox64/Desktop/Projects/Advent-of-Code-2024/Day 6/input.txt");
        List<List<char>> array_2d = new List<List<char>>();
        List<List<char>> array_result = new List<List<char>>();
        List<List<char>> test_list = new List<List<char>>();
        List<char> looping = new List<char>();

        foreach (string line in file) {
            string cleaned = line.Trim('\r','\n');
            List<char> chars = [.. cleaned];
            array_2d.Add(chars);
            array_result.Add(new List<char>(chars));
            test_list.Add(new List<char>(chars));
        }

        int[] starting_position = Aocd6.get_starting_position(array_2d);

        Aocd6.find_path(array_2d, starting_position[0], starting_position[1], array_result, looping);

        int total = 0, total_p2 = 0;

        foreach (List<char> line in array_result) {
            total += line.Count(c => c != '.' && c != '#');
        }

        Console.WriteLine($"AOC Day 6 Part 1 Total: {total}");
        
        for (int y = 0; y < array_2d.Count; y++) {
            for (int x = 0; x < array_2d[y].Count; x++) {
                reset_grid(array_2d, array_result);
                if (array_2d[y][x] == '.') {
                    test_list[y][x] = 'O';
                    if (Aocd6.find_path(test_list, starting_position[0], starting_position[1], array_result, looping) == true) {
                        // print_grid(test_list);
                        total_p2++;
                    }
                    test_list[y][x] = '.';
                }
            }
        }



        // print_grid(array_result);

        
        Console.WriteLine($"AOC Day 6 Part 2 Total: {total_p2}");


    }
}
