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
// import java.util.Collections;
// import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.io.File;


public class App {
    public static void main(String[] args) throws Exception {
        // Read file
        File file = new File("input.txt");
        Scanner reader = new Scanner(file);

        // Build input string from file content
        StringBuilder inputBuilder = new StringBuilder();
        while (reader.hasNextLine()) {
            inputBuilder.append(reader.nextLine()).append("\n");
        }
        reader.close();

        String input = inputBuilder.toString();

        // Parse the registers
        Map<String, Integer> registers = new HashMap<>();
        Pattern registerPattern = Pattern.compile("(Register \\w): (\\d+)");
        Matcher registerMatcher = registerPattern.matcher(input);

        while (registerMatcher.find()) {
            String registerName = registerMatcher.group(1); // "Register A"
            int registerValue = Integer.parseInt(registerMatcher.group(2)); // 729
            registers.put(registerName, registerValue);
        }

        // Parse the program
        List<Integer> program = new ArrayList<>();
        Pattern programPattern = Pattern.compile("Program: ([\\d,]+)");
        Matcher programMatcher = programPattern.matcher(input);

        if (programMatcher.find()) {
            String[] programValues = programMatcher.group(1).split(","); // "0,1,5,4,3,0"
            for (String value : programValues) {
                program.add(Integer.parseInt(value)); // Convert to integers
            }
        }

        for (Map.Entry<String, Integer> entry : registers.entrySet()) {
            System.out.println(entry.getKey() + " = " + entry.getValue());
        }

        A = registers.get("Register A");
        B = registers.get("Register B");
        C = registers.get("Register C");
        combo_op.put(0L,0L);
        combo_op.put(1L,1L);
        combo_op.put(2L,2L);
        combo_op.put(3L,3L);
        combo_op.put(4L, A);
        combo_op.put(5L, B);
        combo_op.put(6L, C);

        System.out.println("\nProgram:");
        System.out.println(program);
        long total = 0;
        for (long t = 100000000000000L; t < 1000000000000000L; t++) {

            A = t;
            B = 0;
            C = 0;
            out.clear();
            ip = 0;
            combo_op.put(0L,0L);
            combo_op.put(1L,1L);
            combo_op.put(2L,2L);
            combo_op.put(3L,3L);
            combo_op.put(4L, A);
            combo_op.put(5L, B);
            combo_op.put(6L, C);
            // System.out.println(t);
            boolean run = true;
            while (ip < program.size() && run == true) {
                assembly(program.get(ip),program.get(ip+1));
                if (out.size() > 0 ) {
                    // System.out.printf("%d:%d\n",out.size(), program.size());
                    if (out.size() > program.size()) {
                        
                        System.out.println("too long");
                        run = false;
                        break;
                    }
                    for (int i = 0; i < out.size(); i++) {
                        if (out.get(i) != program.get(i)) {
                            run = false;
                            break;
                        }
                    }
                }
                
            }
            if (out.equals(program) == true) {
                total = t;
                break;
            }
        }

        System.out.print("Out: ");
        System.out.println(out);
        System.out.printf("A: %d, B: %d C: %d\n",A,B,C);

        for (long num : out) {
            System.out.printf(",%d",num);
        }

        System.out.printf("\nAOC Day 17 Part 2 Total: %d\n",total);
    }

    static long A = 0;
    static long B = 0;
    static long C = 0;
    static Map<Long, Long> combo_op = new HashMap<>();
    static int ip = 0;
    static List<Integer> out = new ArrayList<Integer>();

    private static void assembly(int rf, int lo) {
        boolean jmp = false;
        
        switch (rf) {
            case 0: // adv
                A = Math.floorDiv(A , (int)Math.pow(2, combo_op.get((long)lo)));
                combo_op.put(4L, A);
                break;
            case 1: // bxl
                B = lo ^ B;
                combo_op.put(5L, B);
                break;
            case 2: // bst
                B = combo_op.get((long)lo) % 8;
                combo_op.put(5L, B);
                break;
            case 3: // jnz
                if (A > 0) {
                    jmp = true;
                    ip = lo;
                }
                break;
            case 4: // bxc
                B = B ^ C;
                combo_op.put(5L, B);
                break;
            case 5: // out
                out.add((int)(combo_op.get((long)lo) % 8));
                break;
            case 6: // bdv
                B = Math.floorDiv(A , (long)Math.pow(2, combo_op.get((long)lo)));
                combo_op.put(5L, B);
                break;
            case 7: // cdv
                C = Math.floorDiv(A , (long)Math.pow(2, combo_op.get((long)lo)));
                combo_op.put(6L, C);  
                break;      
            default:
                break;
        }

        if (jmp == false) {
            ip+=2;
        }
    }
}
