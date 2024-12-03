using System;
using System.Diagnostics;
using System.IO;

class Program
{

    static List<int> convert_string_to_int_array(string pair) {
        string left = "", right = "";
        bool change = false;

        foreach (char charactor in pair) {
            if (char.IsDigit(charactor)) {
                if (change) {
                    right += charactor;
                } else {
                    left += charactor;
                }
            } else if (charactor == ',') {
                change = true;
            }
        }

        return new List<int> {int.Parse(left), int.Parse(right)};
    }

    static bool Validate_Pair(string pair) {
        if (pair[0] == '(' && pair[pair.Length - 1] == ')' && pair.Length >= 5 && pair.Length <= 9 && pair.Contains(',')) {
            return true;
        } else {
            return false;
        }
    }

    static bool Throw_Flag(char charactor, string mul_string) {
        if (mul_string.Length > 0 &&  mul_string[mul_string.Length-1] == ')' && mul_string[0] == '(') {
            return false;
        }
        if (char.IsDigit(charactor) || charactor == ',' || charactor == '(' || charactor == ')') {
            return true;
        } else {
            return false;
        }
    } 
    static void Main(string[] args)
    {
        
        

        FileStream stream = File.Open("C:/Users/Starfox64/Desktop/Projects/Advent-of-Code-2024/Day 3/AOC Day 3/input.txt",FileMode.Open,FileAccess.Read);

        using (StreamReader reader = new StreamReader(stream))
        {
            double total = 0;

            string content = reader.ReadToEnd();
            // Console.WriteLine(content);
            bool check_mul = false;
            string mul_string = "";
            bool enabled = true;
            for (int i = 0; i < content.Length; i++) {
                char charactor = content[i];
                if (i > 6){
                    if (content[i-6] == 'd' && content[i-5] == 'o' && content[i-4] == 'n' && content[i-3] == '\'' && content[i-2] == 't' && content[i-1] == '(' && content[i] == ')') {
                        enabled = false;
                    }
                }
                if (i > 3){
                    if (content[i-3] == 'd' && content[i-2] == 'o' && content[i-1] == '(' && content[i] == ')') {
                        enabled = true;
                    }

                    if (enabled) {
                        if (content[i-3] == 'm' && content[i-2] == 'u' && content[i-1] == 'l' && content[i] == '(') {
                            check_mul = true;
                        }
                        if (check_mul) {
                            if (Throw_Flag(charactor, mul_string)) {
                                mul_string += charactor;
                            } else {
                                check_mul = false;
                                if (Validate_Pair(mul_string)) {
                                    List<int> list = convert_string_to_int_array(mul_string);
                                    // Console.WriteLine($"{list[0]},{list[1]}");
                                    total += list[0] * list[1];
                                } else {
                                    Console.Write("");
                                }
                                mul_string = "";
                            }
                        }
                    }
                }
            }

            Console.WriteLine($"AOC Day 3 p1: {total}");
        }

        

        
    }
}

class Car
{
    public string color = "red"; // Instance field
}
