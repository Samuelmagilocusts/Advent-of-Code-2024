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

fn get_new_positions(new_antenna: [i32; 2], origin: [i32; 2], mode: &mut i32) -> (i32, i32, i32, i32) {
    let mut na_y: i32 = 0;
    let mut na_x: i32 = 0;
    let mut o_y: i32 = 0;
    let mut o_x: i32 = 0;

    if *mode == 1 { // normal
        na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
        na_y = (new_antenna[0] - origin[0]) + new_antenna[0];

        o_x = (origin[1] - new_antenna[1]) + origin[1];
        o_y = (origin[0] - new_antenna[0]) + origin[0];

    } else if *mode == 2 { // left
        na_x = (new_antenna[1] - origin[1]) + new_antenna[1];
        na_y = (new_antenna[0] - origin[0]) + new_antenna[0];
        
        o_x = (new_antenna[1] - origin[1]) + origin[1];
        o_y = (new_antenna[0] - origin[0]) + origin[0];

    } else if *mode == 3 { // right
        na_x = (origin[1] - new_antenna[1]) + new_antenna[1];
        na_y = (origin[0] - new_antenna[0]) + new_antenna[0];

        o_x = (origin[1] - new_antenna[1]) + origin[1];
        o_y = (origin[0] - new_antenna[0]) + origin[0];

    }

    (na_y, na_x, o_y, o_x)
}

fn flip_direction(values: [i32; 4], min: i32, max: i32, check_direction_one: &mut bool, check_direction_two: &mut bool, mode: &mut i32) {
    for (pos, coord) in values.iter().enumerate() {
        if *coord <= min || *coord >= max {
            if *mode == 3 {
                *check_direction_two = false;
                *mode = 2;
                break;
            } else if *mode == 2 {
                *check_direction_one = false;
                *mode = 3;
                break;
            } else {
                if pos == 0 || pos == 1 {
                    *check_direction_one = false;
                    *mode = 3;
                } else {
                    *check_direction_two = false;
                    *mode = 2;
                }
                break;
            }
            
        } 
    }
}

fn place_antinodes(grid: &mut Vec<Vec<char>>, new_antenna: [i32; 2], origin: [i32; 2], data: &mut HashMap<String, [i32; 2]>, check_direction_one: &mut bool, check_direction_two: &mut bool, mode: &mut i32, run_p2: bool) -> Vec<String> {
    let mut extra: Vec<String> = Vec::new();
    let mut runonce = true;

    let (na_y, na_x, o_y, o_x) = get_new_positions(new_antenna, origin, mode);

    if run_p2 {
        if grid[new_antenna[0] as usize][new_antenna[1] as usize] == '.' {
            grid[new_antenna[0] as usize][new_antenna[1] as usize] = '#';
        } else if grid[new_antenna[0] as usize][new_antenna[1] as usize] != '#' && grid[new_antenna[0] as usize][new_antenna[1] as usize] != '.'{
            extra.push(format!("{}:{}", new_antenna[0], new_antenna[1]));
        }
        if grid[origin[0] as usize][origin[1] as usize] == '.' {
            grid[origin[0] as usize][origin[1] as usize] = '#';
        } else if grid[origin[0] as usize][origin[1] as usize] != '#' && grid[origin[0] as usize][origin[1] as usize] != '.'{
            extra.push(format!("{}:{}", origin[0], origin[1]));
        }
    }

    if na_x >= 0 && na_y >= 0 && (na_x as usize) < grid[0].len() && (na_y as usize) < grid.len() {
        if grid[na_y as usize][na_x as usize] == '.' {
            grid[na_y as usize][na_x as usize] = '#';
        } else if grid[na_y as usize][na_x as usize] != '#' && grid[na_y as usize][na_x as usize] != '.'{
            extra.push(format!("{}:{}", na_y, na_x));
        }
        data.insert("na".to_string(), [na_y, na_x]);
    } else {
        
        data.insert("na".to_string(), [0,0]);
        if runonce {
            flip_direction([na_y, na_x, o_y, o_x], 0, grid.len() as i32 - 1, check_direction_one, check_direction_two, mode);
            runonce = false;
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
        if runonce {
            flip_direction([na_y, na_x, o_y, o_x], 0, grid.len() as i32 - 1, check_direction_one, check_direction_two, mode);
            runonce = false;
        }
    }

    if na_y == 0 || na_x == 0 || o_y == 0 || o_x == 0 {
        if na_y == 0 || na_x == 0 {
            data.insert("na".to_string(), [0,0]);
        } else if o_y == 0 || o_x == 0 {
            data.insert("o".to_string(), [0,0]);
        }
        if runonce {
            flip_direction([na_y, na_x, o_y, o_x], 0, grid.len() as i32 - 1, check_direction_one, check_direction_two, mode);
            runonce = false;
        }
    }
    
    extra

}

fn recur(grid: &mut Vec<Vec<char>>, new_antenna: [i32; 2], origin: [i32; 2], data: &mut HashMap<String, [i32; 2]>, extra: &mut Vec<String>, run_p2: bool, check_direction_one: &mut bool, check_direction_two: &mut bool, mode: &mut i32, flipped: bool) {

    if (*check_direction_one && *mode == 2) || (*check_direction_two && *mode == 3) || *mode == 1 {
        extra.extend(place_antinodes(grid, new_antenna, origin, data, check_direction_one, check_direction_two, mode, run_p2));
        // print_grid(grid);
        // println!("{:?}",data);
    }

    if run_p2 {
        if let Some(o_coords) = data.get("o") {
            if let Some(na_coords) = data.get("na") {
                let mut new_coords_1: [i32;2] = [0,0];
                let mut new_coords_2: [i32;2] = [0,0];

                if *mode == 1 {
                    *mode = 2;
                }

                if *mode == 2 {
                    if (na_coords[0] == 0 && na_coords[1] == 0) || (o_coords[0] == 0 && o_coords[1] == 0) {
                        new_coords_1 = new_antenna.clone();
                        new_coords_2 = origin.clone();
                    } else {
                        new_coords_1 = *na_coords;
                        new_coords_2 = new_antenna.clone();
                    }
                    

                } else if *mode == 3 {
                    if (na_coords[0] == 0 && na_coords[1] == 0) || (o_coords[0] == 0 && o_coords[1] == 0) {
                        new_coords_1 = new_antenna.clone();
                        new_coords_2 = origin.clone();
                    } else {
                        new_coords_1 = origin.clone();
                        new_coords_2 = *o_coords;
                    }
                    
                }

                if new_coords_1[0] == 0 && new_coords_1[1] == 0 {
                    println!();
                }

                if new_coords_2[0] == 0 && new_coords_2[1] == 0 {
                    println!();
                }

                if *check_direction_one || *check_direction_two {
                    recur(grid, new_coords_1, new_coords_2, data, extra, run_p2, check_direction_one, check_direction_two, mode, flipped);
                }
            }
        }
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
                // println!("{} {} {}",sel_char,i,k);
                mode = 1;
                let flipped = if origin[1] < new_antenna[1] {
                    true
                } else {
                    false
                };
                recur(grid, new_antenna, origin, &mut data, &mut extra, run_p2, &mut check_direction_one, &mut check_direction_two, &mut mode, flipped);
            }
        }
    }
    extra
}

fn main() -> io::Result<()> {
    let path = "input.txt";
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
            // println!("check...{}:{}, {}",i,k,lines_copy[i][k]);
            if character != '.' && character != '#' {
                extra.extend(find_nodes(&mut lines, [i as i32, k as i32], character, part_two));
            }
        }
    }

    print_grid(&mut lines);

    let unique_vec: Vec<_> = extra.into_iter().collect::<HashSet<_>>().into_iter().collect();

    total += unique_vec.len() as i32;
    // for u in unique_vec {
    //     let parts: Vec<&str> = u.split(':').collect();
    //     if parts.len() == 2 {
    //         let left = parts[0];
    //         let right = parts[1];
    //         let left_idx = left.parse::<usize>().expect("Invalid left index");
    //         let right_idx = right.parse::<usize>().expect("Invalid right index");
    //         println!("{} {} {}", lines[left_idx][right_idx], left_idx, right_idx);
    //     } else {
    //         eprintln!("Invalid format in unique_vec: {}", u);
    //     }
    // }
    
    // println!("{:?}",unique_vec);
    total += lines.iter().flat_map(|line| line.iter()).filter(|&&c| c == '#').count() as i32;


    if part_two {
        println!("AOC Day 8 p2 Total: {}", total);
    } else {
        println!("AOC Day 8 p1 Total: {}", total);
    }
    
    Ok(())
}
