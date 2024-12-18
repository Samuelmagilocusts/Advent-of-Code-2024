// import java.util.LinkedList;
// import javax.naming.AuthenticationException;
// import java.util.HashMap;
// import java.util.HashSet;
// import java.util.Iterator;
// import java.util.function.Consumer;
// import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.ArrayList;
import java.util.Collections;
// import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.io.File;

import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;

public class App {
    public static void main(String[] args) throws Exception {
        File file = new File("test.txt");
        Scanner reader = new Scanner(file);

        ArrayList<ArrayList<Integer>> coords = new ArrayList<>();

        // Parse coordinates from file
        while (reader.hasNextLine()) {
            String line = reader.nextLine().trim(); // Read a single line and trim whitespace
            if (!line.isEmpty()) { // Ensure the line is not empty
                String[] parts = line.split(","); // Split by comma
                ArrayList<Integer> temp = new ArrayList<>();
                temp.add(Integer.parseInt(parts[0].trim())); // Add the first coordinate
                temp.add(Integer.parseInt(parts[1].trim())); // Add the second coordinate
                coords.add(temp);
            }
        }
        reader.close();

        // Initialize the grid
         // Set grid size (7x7 in this case)
        ArrayList<Integer> totals = new ArrayList<Integer>();

        // Place '#' in the grid based on coordinates
        // for (int i = 0; i < coords.size(); i++) {
            ArrayList<ArrayList<Character>> grid = new ArrayList<>();
            ArrayList<ArrayList<Boolean>> visited = new ArrayList<ArrayList<Boolean>>();
            ArrayList<Integer> subtotals = new ArrayList<Integer>();
            preload(grid, gridSize);
            preloadBool(visited, gridSize);
            int i = 12;
            int t = 0;
            for (ArrayList<Integer> coord : coords) {
                if (t < i) {
                    int x = coord.get(0);
                    int y = coord.get(1);
                    grid.get(y).set(x, '#');
                }
                t++;
            }
            d = 5;
            recurse(grid, 0, 0, 0, visited, subtotals);
            if (subtotals.size() > 0) {
                for (int num : subtotals) {
                    if (num > 0) {
                        totals.add(num);
                    }
                }
            }
            
            print_grid(grid);
            int thing = 0;
            
        // }

        // Print the grid to verify
        // for (ArrayList<Character> row : grid) {
        //     for (Character cell : row) {
        //         System.out.print(cell + " ");
        //     }
        //     System.out.println();
        // }
        Collections.sort(totals);
        System.out.printf("AOC Day 18 Part 1: %d ", totals.get(0));
    }

    public static void preload(ArrayList<ArrayList<Character>> grid, int gridSize) {
        for (int i = 0; i < gridSize; i++) {
            ArrayList<Character> temp = new ArrayList<>();
            for (int k = 0; k < gridSize; k++) {
                temp.add('.'); // Fill with '.' initially
            }
            grid.add(temp);
        }
    }

    public static void preloadBool(ArrayList<ArrayList<Boolean>> grid, int gridSize) {
        for (int i = 0; i < gridSize; i++) {
            ArrayList<Boolean> temp = new ArrayList<>();
            for (int k = 0; k < gridSize; k++) {
                temp.add(false); // Fill with '.' initially
            }
            grid.add(temp);
        }
    }

    public static void print_grid(ArrayList<ArrayList<Character>> grid) {
        for (ArrayList<Character> line : grid) {
            System.out.println(line);
        }
        System.out.println();
    }

    static int max = 7 * 7;
    static int d = 0;
    static int gridSize = 7;

    public static void recurse(ArrayList<ArrayList<Character>> grid, int x, int y, int total, ArrayList<ArrayList<Boolean>> visited, ArrayList<Integer> totals) {
        // if (d >= max) {
        //     return;
        // }
        // d++;

        // print_grid(grid);
        
        if (x == grid.size()-1 && y == grid.size()-1) {
            totals.add(total);
            return;
        }
        
        visited.get(y).set(x, true);
        grid.get(y).set(x, 'O');
        if (x > 0) {
            if (grid.get(y).get(x-1) == '.' && visited.get(y).get(x-1) == false) {
                recurse(grid, x-1, y, total + 1, visited, totals);
            }
        }

        if (x < grid.size()-1) {
            if (grid.get(y).get(x+1) == '.' && visited.get(y).get(x+1) == false) {
                recurse(grid, x+1, y, total + 1, visited, totals);
            }
        }

        if (y > 0) {
            if (grid.get(y-1).get(x) == '.' && visited.get(y-1).get(x) == false) {
                recurse(grid, x, y-1, total + 1, visited, totals);
            }
        }

        if (y < grid.size()-1) {
            if (grid.get(y+1).get(x) == '.' && visited.get(y+1).get(x) == false) {
                recurse(grid, x, y+1, total + 1, visited, totals);
            }
        }

        visited.get(y).set(x, false);
    }
}
