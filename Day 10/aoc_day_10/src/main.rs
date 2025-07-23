use std::fs::File;
use std::io::{self, BufRead};
use std::collections::{HashMap, HashSet};
use std::time::Instant;

fn get_unique_path_quantity(paths: &Vec<Vec<[usize; 2]>>) -> i32 {
    let mut unique = HashMap::new();
    for path in paths {
        let mut line = String::new();
        for pair in path {
            line += &pair[0].to_string();
            line += &pair[1].to_string();
        }
        // Insert the key only if it doesn't exist already
        unique.entry(line).or_insert(0);
    }
    // println!("{:?}", unique);
    unique.len() as i32
}

fn convert_vec_to_i32(input: Vec<Vec<char>>) -> Vec<Vec<i32>> {
    input
        .into_iter() // Iterate over the outer vector
        .map(|inner| {
            inner
                .into_iter() // Iterate over the inner vector
                .map(|c| {
                    if c.is_digit(10) {
                        // Convert digit characters to their numeric value
                        c.to_digit(10).unwrap() as i32
                    } else {
                        // Handle non-digit characters, e.g., '.', '#'
                        -1 // Assign a placeholder value, adjust as needed
                    }
                })
                .collect::<Vec<i32>>() // Collect into a new inner Vec<i32>
        })
        .collect() // Collect into the new outer Vec<Vec<i32>>
}

fn fallowtrail(
    grid: &Vec<Vec<i32>>, 
    x: usize, 
    y: usize, 
    results: &mut Vec<Vec<[usize; 2]>>, 
    mut current: Vec<[usize; 2]>
) {
    // // If this position has already been visited, return early to avoid revisiting
    // if visited.contains(&(x, y)) {
    //     return;
    // }
    // visited.insert((x, y));  // Mark the current position as visited

    current.push([x, y]);  // Add the current position to the path

    // If we've reached the value 9, this is a valid path, so add it to results
    if grid[y][x] == 9 {
        results.push(current.clone());
        return;
    }

    // Right
    if x + 1 < grid[0].len() && grid[y][x + 1] == grid[y][x] + 1 {
        fallowtrail(grid, x + 1, y, results, current.clone());
    }
    // Left
    if x > 0 && grid[y][x - 1] == grid[y][x] + 1 {
        fallowtrail(grid, x - 1, y, results, current.clone());
    }
    // Down
    if y + 1 < grid.len() && grid[y + 1][x] == grid[y][x] + 1 {
        fallowtrail(grid, x, y + 1, results, current.clone());
    }
    // Up
    if y > 0 && grid[y - 1][x] == grid[y][x] + 1 {
        fallowtrail(grid, x, y - 1, results, current.clone());
    }

    // visited.remove(&(x, y));  // Unmark the current position as visited after all recursive calls
}


fn test_results(paths: &Vec<Vec<[usize; 2]>>, grid: &Vec<Vec<i32>>) {
    for path in paths {
        for pair in path {
            print!("{}",grid[pair[1]][pair[0]]);
        }
        println!();
    }
}



fn main() -> io::Result<()> {
    let path = "input.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: i32 = 0;
    let mut lines: Vec<Vec<char>> = reader
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();

    let grid = convert_vec_to_i32(lines);
    

    // println!("{:?}",grid);
    
    for (l_pos, line) in grid.iter().enumerate() {
        for (pos, num) in line.iter().enumerate() {
            let mut results: Vec<Vec<[usize; 2]>> = Vec::new();
            let mut visited: HashSet<(usize, usize)> = HashSet::new();
            if *num == 0 {
                fallowtrail(&grid, pos, l_pos, &mut results, Vec::new());
            }
            
            total += get_unique_path_quantity(&results);

            // for result in results {
            //     println!("{:?}",result);
            // }
            // test_results(&results, &grid);
        }
    }

    println!("AOC Day 10 p1 total: {}",total);

    

    Ok(())
}


