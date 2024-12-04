using System;
using System.Diagnostics.Contracts;
using System.Numerics;

class Program {
    public const int MaxSize = 140;
    public string[] horizontal = new string[MaxSize];
    public string[] vertical = new string[MaxSize];
    public List<string> diag_tl = new List<string>();
    public List<string> diag_br = new List<string>();
    public int total = 0;
    public int total_p2 = 0;

    static bool ContainsWord(string line, string word) {
        if ((line != null) && line.Contains(word)) {
            return true;
        } else {
            return false;
        }
    }

    static int CountWord(string line, string word) {
        string[] amount = line.Split(word);
        return amount.Length-1;
    }


    static void Main() {
        Program program = new Program();

        string contents = File.ReadAllText("C:/Users/Starfox64/Desktop/Projects/Advent-of-Code-2024/Day 4/AOC Day 4/input.txt");
        program.horizontal = contents.Split("\r\n");

        // build verticle
        for (int v = 0; v < program.horizontal[0].Length; v++){
            string new_line = "";
            for (int h = 0; h < program.horizontal.Length; h++) {
                new_line += program.horizontal[h][v];
            }
            program.vertical[v] = new_line;
            
        }

        // diag top left to bottom right
        for (int i = 0; i < program.horizontal.Length; i++) {
            string new_line = "";
            for (int t = 0; t < program.horizontal[i].Length; t++) {
                if (t+i < Program.MaxSize)

                new_line += program.horizontal[t][t+i];
            }
            program.diag_tl.Add(new_line);
        }
        program.diag_tl.RemoveAt(0);
        for (int i = 0; i < program.horizontal.Length; i++) {
            string new_line = "";
            for (int t = 0; t < program.horizontal[i].Length; t++) {
                if (t+i < Program.MaxSize)

                new_line += program.horizontal[t+i][t];
            }
            program.diag_tl.Insert(0, new_line);
        }

        // diag bottom right to top left
        // diag top left to bottom right
        for (int i = program.horizontal.Length-1; i > 0; i--) {
            string new_line = "";
            for (int t = 0; t < program.horizontal[i].Length; t++) {
                if (i-t >= 0)
                new_line += program.horizontal[i-t][t];
            }
            program.diag_br.Add(new_line);
        }
        program.diag_br.RemoveAt(0);
        for (int i = 0; i < program.horizontal.Length; i++) {
            string new_line = "";
            int k = i;
            for (int t = program.horizontal.Length-1; t >= 0; t--) {
                if (t-i >= 0)
                new_line += program.horizontal[t][k++];
            }
            program.diag_br.Insert(0, new_line);
        }

        string word = "XMAS";
        foreach (string line in program.horizontal) {
            Console.WriteLine(line);
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        foreach (string line in program.vertical) {
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        foreach (string line in program.diag_tl) {
            Console.WriteLine(line);
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        foreach (string line in program.diag_br) {
            Console.WriteLine(line);
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        word = "SAMX";
        foreach (string line in program.horizontal) {
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        foreach (string line in program.vertical) {
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        foreach (string line in program.diag_tl) {
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }
        foreach (string line in program.diag_br) {
            if (ContainsWord(line,word)) {
                program.total += CountWord(line,word);
            }
        }

        Console.WriteLine($"AOC Day 4 p1:{program.total}");

        for (int ver = 0; ver < Program.MaxSize; ver++) {
            for (int hor = 0; hor < Program.MaxSize; hor++) {
                if (ver > 0 && ver < Program.MaxSize-1 && hor > 0 && hor < Program.MaxSize-1) {
                    if (program.horizontal[ver][hor] == 'A') {
                        bool X1 = false, X2 = false;
                        if ((program.horizontal[ver-1][hor-1] == 'M' && program.horizontal[ver+1][hor+1] == 'S') || (program.horizontal[ver-1][hor-1] == 'S' && program.horizontal[ver+1][hor+1] == 'M')) {
                            X1 = true;
                        }
                        if ((program.horizontal[ver-1][hor+1] == 'M' && program.horizontal[ver+1][hor-1] == 'S') || (program.horizontal[ver-1][hor+1] == 'S' && program.horizontal[ver+1][hor-1] == 'M')) {
                            X2 = true;
                        }
                        if (X1 && X2) {
                            program.total_p2++;
                        }
                    }
                }
            }
        }

        Console.WriteLine($"AOC Day 4 p2:{program.total_p2}");
    }
}