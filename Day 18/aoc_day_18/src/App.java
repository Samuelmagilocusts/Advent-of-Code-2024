// import java.util.LinkedList;
// import javax.naming.AuthenticationException;
// import java.util.HashMap;
// import java.util.HashSet;
// import java.util.Iterator;
// import java.util.function.Consumer;
// import java.io.FileNotFoundException;
import java.util.Scanner;

// import org.w3c.dom.Node;

// import java.util.regex.Matcher;
// import java.util.regex.Pattern;
import java.util.ArrayList;
// import java.util.Collections;
import java.util.LinkedList;
import java.util.Queue;
// import java.util.Comparator;
// import java.util.HashMap;
// import java.util.List;
// import java.util.Map;
import java.io.File;

// import java.util.ArrayList;
// import java.util.Scanner;
// import java.io.File;

public class App {
    public static void main(String[] args) throws Exception {
        File file = new File("input.txt");
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

        // Place '#' in the grid based on coordinates
        for (int i = 0; i < coords.size(); i++) {
            ArrayList<ArrayList<Character>> grid = new ArrayList<>();
            ArrayList<ArrayList<Boolean>> visited = new ArrayList<>();
            preload(grid, gridSize);
            preloadBool(visited, gridSize);
            // int i = 1024;
            int t = 0;
            for (ArrayList<Integer> coord : coords) {
                if (t < i) {
                    int x = coord.get(0);
                    int y = coord.get(1);
                    grid.get(y).set(x, '#');
                }
                t++;
            }
            int result = bfs(grid, 0 ,0);
            if (result == -1) {
                System.out.printf("AOC Day 18 Part 2: %d:%d\n", coords.get(i-1).get(0),coords.get(i-1).get(1));
                break;
            }   
        }
        // System.out.printf("AOC Day 18 Part 1: %d\n", total);
        
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

    static int gridSize = 71;

    static class Node {
        int x, y, steps;

        Node(int x, int y, int steps) {
            this.x = x;
            this.y = y;
            this.steps = steps;
        }
    }

    public static int bfs(ArrayList<ArrayList<Character>> grid, int startX, int startY) {
        int n = grid.size();
        boolean[][] visited = new boolean[n][n];
        Queue<Node> queue = new LinkedList<>();
        int[][] directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // Right, Down, Left, Up

        // Start BFS from the top-left corner
        queue.add(new Node(startX, startY, 0));
        visited[startY][startX] = true;

        while (!queue.isEmpty()) {
            Node current = queue.poll();

            // If we reach the bottom-right corner, return the steps
            if (current.x == n - 1 && current.y == n - 1) {
                return current.steps;
            }

            // Explore all possible directions
            for (int[] dir : directions) {
                int newX = current.x + dir[0];
                int newY = current.y + dir[1];

                // Check if the new position is within bounds, not visited, and not blocked
                if (newX >= 0 && newY >= 0 && newX < n && newY < n &&
                        grid.get(newY).get(newX) == '.' && !visited[newY][newX]) {
                    visited[newY][newX] = true;
                    queue.add(new Node(newX, newY, current.steps + 1));
                }
            }
        }

        // If no path is found, return -1
        return -1;
    }
    
}
