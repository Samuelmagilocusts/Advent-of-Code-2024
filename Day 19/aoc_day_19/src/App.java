import java.util.Scanner;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class App {
    public static void main(String[] args) throws Exception {

        File file = new File("test.txt");
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
        ArrayList<String> valid_patterns = new ArrayList<>();

        for (String pattern : patterns) {
            if (isPossible(towels, pattern, new HashMap<>())) { 
                total++; 
                // System.out.printf("increased total: %d\n",total);
                valid_patterns.add(pattern);
            };
            // System.out.println("Nothing");
        }

        System.out.printf("AOC Day 19 Part 1 Total: %d\n",total);

        
        for (String pattern : valid_patterns) {
            number_of_combos(towels, pattern, new HashMap<>());
        }

        System.out.printf("AOC Day 19 part 2 Total: %d\n",total_p2);
        
    }

    static int total_p2 = 0;

    public static boolean number_of_combos(ArrayList<String> towels, String pattern, HashMap<String, Boolean> map) {
        if (pattern.isEmpty()) {
            total_p2++;
            // return true;
        };

        if (map.containsKey(pattern)) {
            return map.get(pattern);
        }

        for (String towel : towels) {
            if (pattern.startsWith(towel)) {
                if (isPossible(towels, pattern.substring(towel.length()), map)) {
                    map.put(pattern, true);
                    total_p2++;
                    // return true;
                }
            }
        }
        map.put(pattern, false);
        return false;
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
