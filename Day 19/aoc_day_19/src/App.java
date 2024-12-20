import java.util.Scanner;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class App {
    public static void main(String[] args) throws Exception {
        
        File file = new File("input.txt");
        Scanner reader = new Scanner(file);

        ArrayList<String> towels = new ArrayList<>();
        ArrayList<String> patterns = new ArrayList<>();

        while (reader.hasNextLine()) {
            String line = reader.nextLine().trim();

            if (line.length() > 0) {
                if (line.contains(",")) {
                    String[] temp = line.split(", ");
                    towels.addAll(Arrays.asList(temp));
                } else {
                    patterns.add(line);
                }
            }
        }

        reader.close();

        int total = 0;
        long start_p1 = System.currentTimeMillis();

        for (String pattern : patterns) {
            if (isPossible(towels, pattern, new HashMap<>())) { 
                total++; 
            };
        }

        System.out.printf("AOC Day 19 Part 1 Total: %d\n",total);
        long end_p1 = System.currentTimeMillis();
        System.out.println("Time p1 ms: " + (end_p1-start_p1));
        
        long start_p2 = System.currentTimeMillis();
        int total_p2 = 0;
        for (String pattern : patterns) {
            total_p2 += number_of_combos(towels, pattern, new HashMap<>());
            // System.out.println("out total up");
        }

        System.out.printf("AOC Day 19 part 2 Total: %d\n",total_p2);
        long end_p2 = System.currentTimeMillis();
        System.out.println("Time p2 ms: " + (end_p2-start_p2));
        
    }

    public static int number_of_combos(ArrayList<String> towels, String pattern, HashMap<String, Integer> map) {
        if (pattern.isEmpty()) {
            return 1;
        };

        if (map.containsKey(pattern)) {
            return map.get(pattern);
        }

        int max = 0;

        for (String towel : towels) {
            if (pattern.startsWith(towel)) {
                int current_max = number_of_combos(towels, pattern.substring(towel.length()), map);
                max += current_max;
            }
        }
        map.put(pattern, max);
        return max;
    }

    public static boolean isPossible(ArrayList<String> towels, String pattern, HashMap<String, Boolean> map) {

        if (pattern.isEmpty()) return true;

        if (map.containsKey(pattern)) {
            return map.get(pattern);
        }

        for (String towel : towels) {
            if (pattern.startsWith(towel)) {
                if (isPossible(towels, pattern.substring(towel.length()), map)) {
                    map.put(pattern, true);
                    return true;
                }
            }
        }
        map.put(pattern, false);
        return false;
    }
}
