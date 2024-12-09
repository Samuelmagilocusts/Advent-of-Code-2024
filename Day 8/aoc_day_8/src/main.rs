use std::fs::File;
use std::io::{self, BufRead};
use std::collections::HashSet;
use std::collections::HashMap;


fn print_grid(grid: &mut Vec<Vec<char>>) {
    for i in 0..grid.len() {
        for k in 0..grid.len() {
            print!("{}",grid[i][k]);
        }
        println!();
    }
    println!("\n\n");
}

fn get_new_positions(new_antenna: [i32; 2], origin: [i32; 2], directions: [char; 4], mode: &mut i32) -> (i32, i32, i32, i32) {
    let mut na_y: i32 = 0;
    let mut na_x: i32 = 0;
    let mut o_y: i32 = 0;
    let mut o_x: i32 = 0;

    // l & u: has to be negitive
    // r & d: has to be positive
        
    if *mode == 1 { // normal
        if directions[0] == 'l' && directions[2] == 'u' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        } else if directions[0] == 'l' && directions[2] == 'd' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        } else if directions[0] == 'r' && directions[2] == 'u' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        } else if directions[0] == 'r' && directions[2] == 'd' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        }

        if directions[1] == 'l' && directions[3] == 'u' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        } else if directions[1] == 'l' && directions[3] == 'd' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        } else if directions[1] == 'r' && directions[3] == 'u' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        } else if directions[1] == 'r' && directions[3] == 'd' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        }
    } else if *mode == 2 { // left
        if directions[0] == 'l' && directions[2] == 'u' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        } else if directions[0] == 'l' && directions[2] == 'd' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        } else if directions[0] == 'r' && directions[2] == 'u' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        } else if directions[0] == 'r' && directions[2] == 'd' {
            na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
            na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        }

        if directions[1] == 'l' && directions[3] == 'u' {
            o_x = (new_antenna[1] - origin[1]) + origin[1];
            o_y = (new_antenna[0] - origin[0]) + origin[0];

        } else if directions[1] == 'l' && directions[3] == 'd' {
            o_x = (new_antenna[1] - origin[1]) + origin[1];
            o_y = (new_antenna[0] - origin[0]) + origin[0];

        } else if directions[1] == 'r' && directions[3] == 'u' {
            o_x = (new_antenna[1] - origin[1]) + origin[1];
            o_y = (new_antenna[0] - origin[0]) + origin[0];

        } else if directions[1] == 'r' && directions[3] == 'd' {
            o_x = (new_antenna[1] - origin[1]) + origin[1];
            o_y = (new_antenna[0] - origin[0]) + origin[0];

        }
    } else if *mode == 3 { // right
        if directions[0] == 'l' && directions[2] == 'u' {
            na_x = (origin[1] - new_antenna[1]) + new_antenna[1];
            na_y = (origin[0] - new_antenna[0]) + new_antenna[0];

        } else if directions[0] == 'l' && directions[2] == 'd' {
            na_x = (origin[1] - new_antenna[1]) + new_antenna[1];
            na_y = (origin[0] - new_antenna[0]) + new_antenna[0];

        } else if directions[0] == 'r' && directions[2] == 'u' {
            na_x = (origin[1] - new_antenna[1]) + new_antenna[1];
            na_y = (origin[0] - new_antenna[0]) + new_antenna[0];

        } else if directions[0] == 'r' && directions[2] == 'd' {
            na_x = (origin[1] - new_antenna[1]) + new_antenna[1];
            na_y = (origin[0] - new_antenna[0]) + new_antenna[0];

        }

        if directions[1] == 'l' && directions[3] == 'u' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        } else if directions[1] == 'l' && directions[3] == 'd' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        } else if directions[1] == 'r' && directions[3] == 'u' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        } else if directions[1] == 'r' && directions[3] == 'd' {
            o_x = (origin[1] - new_antenna[1]) + origin[1];
            o_y = (origin[0] - new_antenna[0]) + origin[0];

        }
    }

    (na_y, na_x, o_y, o_x)
}

fn find_direction(new_antenna: [i32; 2], origin: [i32; 2], mode: &mut i32) -> [char; 4] {
    let mut directions: [char; 4] = ['0','0','0','0'];

    if *mode == 1 {
        if new_antenna[1] < origin[1] {
            directions[0] = 'l';
            directions[1] = 'r';
        } else {
            directions[0] = 'r';
            directions[1] = 'l';
        }

        if new_antenna[0] < origin[0] {
            directions[2] = 'u';
            directions[3] = 'd';
        } else {
            directions[2] = 'd';
            directions[3] = 'u';
        }

    } else if *mode == 2 {
        directions[0] = 'l';
        directions[1] = 'l';

        if new_antenna[0] < origin[0] {
            directions[2] = 'u';
            directions[3] = 'u';
        } else {
            directions[2] = 'd';
            directions[3] = 'd';
        }

    } else if *mode == 3 {
        directions[0] = 'r';
        directions[1] = 'r';

        if new_antenna[0] < origin[0] {
            directions[2] = 'd';
            directions[3] = 'd';
        } else {
            directions[2] = 'u';
            directions[3] = 'u';
        }
    }
    

    directions
}

fn place_antinodes(grid: &mut Vec<Vec<char>>, new_antenna: [i32; 2], origin: [i32; 2], data: &mut HashMap<String, [i32; 2]>, check_direction_one: &mut bool, check_direction_two: &mut bool, mode: &mut i32) -> Vec<String> {
    let mut extra: Vec<String> = Vec::new();

    let directions = find_direction(new_antenna, origin, mode);

    let (na_y, na_x, o_y, o_x) = get_new_positions(new_antenna, origin, directions, mode);

    if na_x >= 0 && na_y >= 0 && (na_x as usize) < grid[0].len() && (na_y as usize) < grid.len() {
        if grid[na_y as usize][na_x as usize] == '.' {
            grid[na_y as usize][na_x as usize] = '#';
        } else if grid[na_y as usize][na_x as usize] != '#' && grid[na_y as usize][na_x as usize] != '.'{
            extra.push(format!("{}:{}", na_y, na_x));
        }
        data.insert("na".to_string(), [na_y, na_x]);
    } else {
        data.insert("na".to_string(), [0,0]);
        if directions[0] == 'l' {
            *check_direction_one = false;
            *mode = 3; // right
        } else {
            *check_direction_two = false;
            *mode = 2 // left
        }
        
    }

    if o_x >= 0 && o_y >= 0 && (o_x as usize) < grid[0].len() && (o_y as usize) < grid.len() {
        if grid[o_y as usize][o_x as usize] == '.' {
            grid[o_y as usize][o_x as usize] = '#';
        } else if grid[o_y as usize][o_x as usize] != '#' && grid[o_y as usize][o_x as usize] != '.'{
            extra.push(format!("{}:{}",o_y,o_x));
        }
        data.insert("o".to_string(), [o_y, o_x]);
    } else {
        data.insert("o".to_string(), [0,0]);
        if directions[1] == 'l' {
            *check_direction_one = false;
            *mode = 3; // right
        } else {
            *check_direction_two = false;
            *mode = 2 // left
        }
        
    }
    
    extra

}

fn recur(grid: &mut Vec<Vec<char>>, new_antenna: [i32; 2], origin: [i32; 2], data: &mut HashMap<String, [i32; 2]>, extra: &mut Vec<String>, run_p2: bool, check_direction_one: &mut bool, check_direction_two: &mut bool, mode: &mut i32) {

    if (*check_direction_one && *mode == 2) || (*check_direction_two && *mode == 3) || *mode == 1 {
        extra.extend(place_antinodes(grid, new_antenna, origin, data, check_direction_one, check_direction_two, mode));
        // print_grid(grid);
        // println!("{:?}",data);
    }

    if run_p2 {
        if let Some(o_coords) = data.get("o") {
            if let Some(na_coords) = data.get("na") {
                let mut new_coords_1: [i32;2];
                let mut new_coords_2: [i32;2];

                if *mode == 1 {
                    new_coords_1 = new_antenna.clone();
                    new_coords_2 = origin.clone();
                } else {
                    
                    new_coords_1 = *na_coords;
                    new_coords_2 = *o_coords;
                }

                let directions = find_direction(*na_coords, *o_coords, mode);
                if directions[0] == 'l' {
                    *mode = 2;
                    if new_coords_2[0] == 0 && new_coords_2[1] == 0 {
                        new_coords_1 = *o_coords;
                        new_coords_2 = origin.clone();
                    }
                } else {
                    *mode = 3;
                    if new_coords_1[0] == 0 && new_coords_1[1] == 0 {
                        new_coords_1 = *o_coords;
                        new_coords_2 = origin.clone();
                    }
                }

                // if new_coords_2[0] == 0 && new_coords_2[1] == 0 {
                //     new_coords_1 = origin.clone();
                //     new_coords_2 = new_antenna.clone();

                //     if *mode == 2 {
                //         *mode = 3;
                //     } else {
                //         *mode = 2;
                //     }
                // }
                // if new_coords_1[0] == 0 && new_coords_1[1] == 0 {
                //     new_coords_1 = origin.clone();
                //     new_coords_2 = new_antenna.clone();

                //     if *mode == 2 {
                //         *mode = 3;
                //     } else {
                //         *mode = 2;
                //     }
                // }

                if *check_direction_one || *check_direction_two {
                    recur(grid, new_coords_1, new_coords_2, data, extra, run_p2, check_direction_one, check_direction_two, mode);
                }
            }
        }
        
        let thingy = 0;
    }
}

fn find_nodes(grid: &mut Vec<Vec<char>>, pair: [i32; 2], sel_char: char, run_p2: bool) -> Vec<String> {
    let mut extra: Vec<String> = Vec::new();
    let mut data:HashMap<String, [i32; 2]> = HashMap::new();
    let mut new_antenna: [i32; 2];
    let mut origin: [i32; 2];
    let mut check_direction_one: bool = true;
    let mut check_direction_two: bool = true;
    let mut mode: i32 = 1;

    for i in 0..grid.len() {
        for k in 0..grid.len() {
            if grid[i][k].clone() == sel_char && pair[0] != i as i32 && pair[1] != k as i32 {
                check_direction_one = true;
                check_direction_two = true;
                new_antenna = [i as i32, k as i32];
                origin = pair.clone();
                recur(grid, new_antenna, origin, &mut data, &mut extra, run_p2, &mut check_direction_one, &mut check_direction_two, &mut mode);
                if new_antenna[0] == origin[0] {
                    println!("Same Y: {:?}:{:?}",new_antenna, origin);
                }
                if new_antenna[1] == origin[1] {
                    println!("Same X: {:?}:{:?}",new_antenna, origin);
                }
            }
        }
    }
    extra
}

fn main() -> io::Result<()> {
    let path = "test.txt";

    let part_two: bool = true;

    let file = File::open(path)?;

    let reader = io::BufReader::new(file);

    let mut total: i32 = 0;

    let mut lines: Vec<Vec<char>> = reader
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();

    let lines_copy = lines.clone(); // Duplicate lines for immutable borrow in main

    let mut extra: Vec<String> = Vec::new();

    for i in 0..lines_copy.len() {
        for k in 0..lines_copy.len() {
            let character = lines[i][k].clone();
            println!("check...{}:{}, {}",i,k,lines_copy[i][k]);
            if character != '.' && character != '#' {
                extra.extend(find_nodes(&mut lines, [i as i32, k as i32], character, part_two));
            }
        }
    }

    print_grid(&mut lines);

    let unique_vec: Vec<_> = extra.into_iter().collect::<HashSet<_>>().into_iter().collect();

    total += unique_vec.len() as i32;

    total += lines.iter().flat_map(|line| line.iter()).filter(|&&c| c == '#').count() as i32;


    if part_two {
        println!("AOC Day 8 p2 Total: {}", total);
    } else {
        println!("AOC Day 8 p1 Total: {}", total);
    }
    

    Ok(())
}
