import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
// import java.util.LinkedList;
// import javax.naming.AuthenticationException;
// import java.util.HashMap;
// import java.util.HashSet;
// import java.util.Iterator;
// import java.util.function.Consumer;
import java.io.File;
// import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;


public class App {


    public static void main(String[] args) throws Exception {
        File file = new File("test2.txt");
        Scanner reader = new Scanner(file);
        
        ArrayList<ArrayList<Character>> initial_grid = new ArrayList<ArrayList<Character>>();
        ArrayList<ArrayList<Character>> grid = new ArrayList<ArrayList<Character>>();
        ArrayList<ArrayList<Character>> start_frame = new ArrayList<>();

        while (reader.hasNextLine()) {
            String line = reader.nextLine();
            ArrayList<Character> char_line = new ArrayList<Character>();
            for (int i = 0; i < line.length(); i++) {
                char_line.add(line.charAt(i));
            }
            initial_grid.add(char_line);
        }

        reader.close();

        File commands_file = new File("test_commands2.txt");
        Scanner commands_reader = new Scanner(commands_file);
        StringBuilder commands = new StringBuilder();

        while (commands_reader.hasNextLine()) {
            String line = commands_reader.nextLine();
            commands.append(line.strip()); // Strip unwanted characters and append to StringBuilder
        }
        commands_reader.close();


        for (ArrayList<Character> line : initial_grid) {
            ArrayList<Character> dif_line = new ArrayList<Character>(); 
            for (char thing : line) {
                switch (thing) {
                    case '.':
                        dif_line.add('.');
                        dif_line.add('.');
                        break;
                    case '#':
                        dif_line.add('#');
                        dif_line.add('#');
                        break;
                    case 'O':
                        dif_line.add('[');
                        dif_line.add(']');
                        break;
                    case '@':
                        dif_line.add('@');
                        dif_line.add('.');
                        break;
                }
            }
            grid.add(dif_line);
        }

        // Deep copy
        for (ArrayList<Character> row : grid) {
            start_frame.add(new ArrayList<>(row));
        }

        print_grid(grid);

        int[] xy = {0,0};
        for (int i = 0; i < grid.size(); i++) {
            for (int t = 0; t < grid.get(i).size(); t++) {
                if (grid.get(i).get(t) == '@') {
                    xy[0] = t;
                    xy[1] = i;
                }
            }
        }

        int iter = 0;
        for (char command : commands.toString().toCharArray()) {
            move(grid, command, xy);
            // if (iter > 8173) {
            //     System.out.println(iter);
            //     print_grid(grid);
            //     System.out.println();
            // }

 
            // for (int i = 0; i < grid.size(); i++) {
            //     for (int t = 0; t < grid.get(0).size(); t++) {
            //         if (i < grid.size()-1) {
            //             if (grid.get(i).get(t) == '#' && start_frame.get(i).get(t) != '#') {
            //                 System.out.printf("# %d:%d\n",i,t);
            //             }
            //         }
            //     }
            // }
        

            if (malformmed(grid)) {
                System.out.println(iter);
                print_grid(grid);
                
                System.out.println("");
            }
            // print_grid(grid);
                
            // System.out.println("");
            iter++;
        }
        System.out.println("\nLast");
        print_grid(grid);

        System.out.printf("AOC Day 15 P2 Total: %d",get_total(grid));
    }

    private static int get_total(ArrayList<ArrayList<Character>> grid) {
        int total = 0;
        for (int i = 0; i < grid.size(); i++) {
            for (int t = 0; t < grid.get(0).size(); t++) {
                switch (grid.get(i).get(t)) {
                    case '[':
                        total += 100 * i + t;
                        break;
                    default:
                        break;
                }
            }
        }
        return total;
    }

    private static void print_grid(ArrayList<ArrayList<Character>> grid) {
        for (ArrayList<Character> line : grid) { 
            System.out.println(line);
        }
    }

    private static boolean malformmed(ArrayList<ArrayList<Character>> grid) {
        for (int i = 0; i < grid.size(); i++) {
            for (int t = 0; t < grid.get(0).size(); t++) {
                if (i < grid.size()-1) {
                    if (grid.get(i).get(t) == '[' && grid.get(i).get(t+1) != ']') {
                        System.out.printf("[] %d:%d\n",i,t);
                        return true;
                    }
                }
            }
        }
        
        return false;
    }

    private static void get_coords_that_mov(ArrayList<ArrayList<Character>> grid, char direction, int x, int y, ArrayList<int[]> to_move) {

        int state = 1;
        if (grid.get(y).get(x) == '[') {
            state = 3;
        } else if (grid.get(y).get(x) == ']') {
            state = 2;
        } else if (grid.get(y).get(x) == '.') {
            state = 0;
        } else if (grid.get(y).get(x) == '#') {
            state = -1;
        }

        switch (state) {
            case -1:
                int[] coord_kill = {-1, -1};
                if (!containsCoord(to_move, coord_kill)) {
                    to_move.add(Arrays.copyOf(coord_kill, coord_kill.length));
                }
                return;
            case 0:
                // int[] coord_default = {x, y};
                // if (!containsCoord(to_move, coord_default)) {
                //     to_move.add(Arrays.copyOf(coord_default, coord_default.length));
                // }
                return;
            default:
                int[] coord = {x, y};
                if (!containsCoord(to_move, coord)) {
                    to_move.add(Arrays.copyOf(coord, coord.length));
                }

                switch (direction) {
                    case '<':
                        get_coords_that_mov(grid, direction, x - 1, y, to_move);
                        break;
                    case '>':
                        get_coords_that_mov(grid, direction, x + 1, y, to_move);
                        break;
                    case '^':
                        if (state == 2) {
                            int[] leftCoord = {x - 1, y};
                            if (!containsCoord(to_move, leftCoord)) {
                                to_move.add(Arrays.copyOf(leftCoord, leftCoord.length));
                            }
                            get_coords_that_mov(grid, direction, x - 1, y - 1, to_move); // left
                            get_coords_that_mov(grid, direction, x, y - 1, to_move);     // straight
                        } else {
                            int[] rightCoord = {x + 1, y};
                            if (!containsCoord(to_move, rightCoord)) {
                                to_move.add(Arrays.copyOf(rightCoord, rightCoord.length));
                            }
                            get_coords_that_mov(grid, direction, x + 1, y - 1, to_move); // right
                            get_coords_that_mov(grid, direction, x, y - 1, to_move);     // straight
                        }
                        break;
                    case 'v':
                        if (state == 2) {
                            int[] leftCoord = {x - 1, y};
                            if (!containsCoord(to_move, leftCoord)) {
                                to_move.add(Arrays.copyOf(leftCoord, leftCoord.length));
                            }
                            get_coords_that_mov(grid, direction, x - 1, y + 1, to_move); // left
                            get_coords_that_mov(grid, direction, x, y + 1, to_move);     // straight
                        } else {
                            int[] rightCoord = {x + 1, y};
                            if (!containsCoord(to_move, rightCoord)) {
                                to_move.add(Arrays.copyOf(rightCoord, rightCoord.length));
                            }
                            get_coords_that_mov(grid, direction, x + 1, y + 1, to_move); // right
                            get_coords_that_mov(grid, direction, x, y + 1, to_move);     // straight
                        }
                        break;
                }
                break;
        }

        
    }

    private static boolean containsCoord(ArrayList<int[]> list, int[] target) {
        for (int[] coord : list) {
            if (Arrays.equals(coord, target)) {
                return true;
            }
        }
        return false;
    }
    

    private static void move(ArrayList<ArrayList<Character>> grid, char direction, int[] point) {
        ArrayList<int[]> move_list = new ArrayList<int[]>();
        int x = point[0];
        int y = point[1];
        int[] kill_code = {-1,-1};
        switch (direction) {
            case '<':
                if (x > 1) {
                    if (grid.get(y).get(x-1) == '#') {

                    } else if (grid.get(y).get(x-1) == '.') {
                        grid.get(y).set(x, '.');
                        grid.get(y).set(x-1, '@');
                        point[0]--;
                    } else if (grid.get(y).get(x-1) == '[' || grid.get(y).get(x-1) == ']') {
                        get_coords_that_mov(grid, direction, x-1, y, move_list);
                        if (move_list.size() > 0 && !containsCoord(move_list, kill_code)) {
                            Collections.reverse(move_list);
                            for (int[] coords : move_list) {
                                grid.get(coords[1]).set(coords[0]-1, grid.get(coords[1]).get(coords[0]));
                            }
                            grid.get(y).set(x, '.');
                            grid.get(y).set(x-1, '@');
                            point[0]--;
                        }
                    }
                }
                break;
            case '>':
                if (x < grid.size()-2) {
                    if (grid.get(y).get(x+1) == '#') {

                    } else if (grid.get(y).get(x+1) == '.') {
                        grid.get(y).set(x, '.');
                        grid.get(y).set(x+1, '@');
                        point[0]++;
                    } else if (grid.get(y).get(x+1) == '[' || grid.get(y).get(x+1) == ']') {
                        get_coords_that_mov(grid, direction, x+1, y, move_list);
                        if (move_list.size() > 0 && !containsCoord(move_list, kill_code)) {
                            Collections.reverse(move_list);
                            for (int[] coords : move_list) {
                                grid.get(coords[1]).set(coords[0]+1, grid.get(coords[1]).get(coords[0]));
                            }
                            grid.get(y).set(x, '.');
                            grid.get(y).set(x+1, '@');
                            point[0]++;
                        }
                    }
                }
                break;
            case '^':
                if (y > 1) {
                    if (grid.get(y-1).get(x) == '#') {

                    } else if (grid.get(y-1).get(x) == '.') {
                        grid.get(y).set(x, '.');
                        grid.get(y-1).set(x, '@');
                        point[1]--;
                    } else if (grid.get(y-1).get(x) == '[' || grid.get(y-1).get(x) == ']') {
                        get_coords_that_mov(grid, direction, x, y-1, move_list);
                        if (move_list.size() > 0 && !containsCoord(move_list, kill_code)) {
                            move_list.sort(Comparator.comparingInt(a -> a[1]));
                            for (int[] coords : move_list) {
                                grid.get(coords[1]-1).set(coords[0], grid.get(coords[1]).get(coords[0]));
                                grid.get(coords[1]).set(coords[0], '.');
                                
                            }
                            grid.get(y).set(x, '.');
                            grid.get(y-1).set(x, '@');
                            point[1]--;
                        }
                    }
                }
                break;
            case 'v':
                if (y < grid.size()-2) {
                    if (grid.get(y+1).get(x) == '#') {

                    } else if (grid.get(y+1).get(x) == '.') {
                        grid.get(y).set(x, '.');
                        grid.get(y+1).set(x, '@');
                        point[1]++;
                    } else if (grid.get(y+1).get(x) == '[' || grid.get(y+1).get(x) == ']') {
                        get_coords_that_mov(grid, direction, x, y+1, move_list);
                        if (move_list.size() > 0 && !containsCoord(move_list, kill_code)) {
                            move_list.sort(Comparator.comparingInt(a -> a[1]));
                            Collections.reverse(move_list);
                            for (int[] coords : move_list) {
                                grid.get(coords[1]+1).set(coords[0], grid.get(coords[1]).get(coords[0]));
                                grid.get(coords[1]).set(coords[0], '.');
                                
                            }
                            grid.get(y).set(x, '.');
                            grid.get(y+1).set(x, '@');
                            point[1]++;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}


