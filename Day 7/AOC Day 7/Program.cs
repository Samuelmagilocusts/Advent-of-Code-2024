// AOC DAY 7

using System;
using System.Diagnostics;
using System.Security.Cryptography.X509Certificates;

class AOC7 {

    static UInt64 multiply_entire_array(List<int> array) {
        UInt64 output = 0;
        foreach (int number in array) {
            output *= (UInt64)number;
        }
        return output;
    }

    static bool try_match(UInt64 equation, List<int> operators, List<string> combos) {
        UInt64 total = 0;
        foreach (string combo in combos) {
            for (int i = 0; i < combo.Length; i++) {
                if (i == 0) {
                    if (combo[i] == '*') {
                        total += (UInt64)(operators[i] * operators[i+1]);
                    } else if (combo[i] == 'o') {
                        total += UInt64.Parse(operators[i].ToString() + operators[i+1].ToString());
                        
                    } else {
                        total += (UInt64)(operators[i] + operators[i+1]);
                    } 
                } else {
                    if (combo[i] == '*') {
                        total *= (UInt64)operators[i+1];
                    } else if (combo[i] == 'o') {
                        total = UInt64.Parse(total.ToString() + operators[i+1].ToString());
                    } else {
                        total += (UInt64)operators[i+1];
                    } 
                }
            }

            if (total == equation) {
                return true;
            } else {
                total = 0;
            }
        }
        return false;
    }

    static void generate_combinations(List<string> combinations, int number_of_combos, string Current, char[] ops) {
        if (Current.Length == number_of_combos) {
            combinations.Add(Current);
            return;
        } 

        foreach (char op in ops) {
            generate_combinations(combinations, number_of_combos, Current+op, ops);
        }
    }

    static UInt64 calculate_match(UInt64 equation, List<int> operators, char[] gen) {
        if ((UInt64)operators.Sum() == equation || multiply_entire_array(operators) == equation) {
            return equation;
        } else {
            List<string> combos = new List<string>();
            generate_combinations(combos, operators.Count - 1, "", gen);
            if (try_match(equation, operators, combos) == true) {
                return equation;
            } else {
                return 0;
            }
        }
    }

    static void Main() {
        string[] file = File.ReadAllLines("C:/Users/Starfox64/Desktop/Projects/Advent-of-Code-2024/Day 7/AOC Day 7/input.txt");

        List<UInt64> equations  = new List<UInt64>();
        List<List<int>> operators_all = new List<List<int>>();
        UInt64 total = 0, total_p2 = 0;
        Stopwatch stopwatch = new Stopwatch();
        stopwatch.Start();

        foreach (string line in file) {
            equations.Add(UInt64.Parse(line.Split(':')[0]));
            string operation_string = (line.Split(": ")[1]).Trim('\r','\n');
            string[] new_string_array_operations = operation_string.Split(' ');
            List<int> new_list = new_string_array_operations.Select(int.Parse).ToList();
            operators_all.Add(new_list);
        }

        for (int i = 0; i < equations.Count; i++) {
            total += calculate_match(equations[i], operators_all[i], ['*','+']);
        }

        stopwatch.Stop();
        Console.WriteLine($"AOC Day 7 Part 1: {total}");
        Console.WriteLine($"Time taken: {stopwatch.ElapsedMilliseconds} milliseconds");
        stopwatch.Start();

        for (int i = 0; i < equations.Count; i++) {
            total_p2 += calculate_match(equations[i], operators_all[i], ['*','+','o']);
        }

        stopwatch.Stop();
        Console.WriteLine($"AOC Day 7 Part 2: {total_p2}");
        Console.WriteLine($"Time taken: {stopwatch.ElapsedMilliseconds} milliseconds");
    }
}