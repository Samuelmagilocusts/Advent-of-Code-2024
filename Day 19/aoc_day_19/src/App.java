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

        ArrayList<ArrayList<String>> all_pos = new ArrayList<>();

        
        for (int i = 0; i < towels.size(); i++) {
            ArrayList<String> temp = new ArrayList<>();
            for (int s = towels.size()-1; s > (towels.size() - 1) - i; s--) {
                temp.add(towels.get(s));
            }
            for (int e = 0; e < towels.size()-i; e++) {
                temp.add(towels.get(e));
            }
            all_pos.add(temp);
        }

        int total_p2 = 0;
        
        for (String pattern : valid_patterns) {
             
            for (ArrayList<String> new_towels : all_pos) {
                ArrayList<String> result = new ArrayList<>();
                if (number_of_combos(new_towels, pattern, new HashMap<>(), result)) {
                    //
                }
            }
        }

        total_p2 = all_results.size();

        System.out.printf("AOC Day 19 part 2 Total: %d\n",total_p2);
        
    }

    static ArrayList<ArrayList<String>> all_results = new ArrayList<>();

    public static boolean number_of_combos(ArrayList<String> towels, String pattern, HashMap<String, Boolean> map, ArrayList<String> result) {
        if (pattern.isEmpty()) {
            if (!all_results.contains(result)) {
                all_results.add(result);
            }
        };

        if (map.containsKey(pattern)) {
            return map.get(pattern);
        }

        for (String towel : towels) {
            if (pattern.startsWith(towel)) {
                result.add(towel);
                if (number_of_combos(towels, pattern.substring(towel.length()), map, result)) {
                    map.put(pattern, true);
                    if (!all_results.contains(result)) {
                        all_results.add(result);
                    }
                }
                result.remove(result.size()-1);
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
