use std::fs::File;
use std::io::{self, BufRead};
use std::collections::{HashMap, HashSet};
use std::time::Instant;

use std::fs::OpenOptions;
use std::io::{Write};

fn append_grid_to_file(grid: &Vec<Vec<char>>, iteration: usize, filename: &str) -> io::Result<()> {
    // Open the file in append mode
    let mut file = OpenOptions::new()
        .create(true)
        .append(true)
        .open(filename)?;

    // Write the iteration number
    writeln!(file, "Iteration: {}\n", iteration)?;

    // Write the grid row by row
    for row in grid {
        let line: String = row.iter().collect(); // Convert Vec<char> to String
        writeln!(file, "{}", line)?; // Write to file
    }

    // Add some blank lines for spacing
    writeln!(file, "\n\n")?;

    Ok(())
}

fn process_robot(start: [i32; 2], move_d: [i32; 2], limit: i32, max_x: i32, max_y: i32) -> [usize; 2] {
    let mut x = (start[0] + move_d[0] * limit) % max_x;
    let mut y = (start[1] + move_d[1] * limit) % max_y;

    if x < 0 {
        x += max_x;
    }
    if y < 0 {
        y += max_y;
    }

    [x as usize, y as usize]
}


fn extract_pair(pair: &str) -> (i32, i32) {
    let parts: Vec<i32> = pair
        .split(',')
        .map(|num| num.parse::<i32>().unwrap())
        .collect();

    (parts[0], parts[1])
}

fn parse_coordinates(input: &str) -> [i32; 4] {
    let mut p_coords = (0, 0);
    let mut v_coords = (0, 0);

    for segment in input.split_whitespace() {
        if segment.starts_with("p=") {
            p_coords = extract_pair(&segment[2..]);
        } else if segment.starts_with("v=") {
            v_coords = extract_pair(&segment[2..]);
        }
    }

    let (p_x, p_y) = p_coords;
    let (v_x, v_y) = v_coords;

    [p_x, p_y, v_x, v_y]
}

fn main() -> io::Result<()> {
    let path = "input.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: i32 = 0;
    let mut total_p2: i32 = 0;
    let mut instructions: Vec<String> = reader
        .lines()
        .filter_map(Result::ok)
        .collect();
    let mut instructions_copy = instructions.clone();
    let mut grid: Vec<Vec<i32>> = Vec::new();

    let max_x = 101;
    let max_y = 103;

    // let max_x = 11;
    // let max_y = 7;

    let limit = 100;

    let start = Instant::now();
    for _ in 0..max_y {
        let mut temp = Vec::new();
        for _ in 0..max_x {
            temp.push(0);
        }
        grid.push(temp);
    }

    for instruction in instructions {
        let [p_x, p_y, v_x, v_y] = parse_coordinates(&instruction);
        let [x, y] = process_robot([p_x, p_y], [v_x, v_y], limit, max_x, max_y);
        // println!("{}:{}",x,y);

        grid[y][x] += 1;
    }

    let forbidden_x = max_x/2;
    let fordidden_y = max_y/2;
    let mut lu = 0;
    let mut ru = 0;
    let mut ld = 0;
    let mut rd = 0;
    for y in 0..max_y {
        for x in 0..max_x {
            if y < fordidden_y && x < forbidden_x { // upper left
                lu += grid[y as usize][x as usize];
            } else if y < fordidden_y && x > forbidden_x { // upper right
                ru += grid[y as usize][x as usize];
            } else if y > fordidden_y && x > forbidden_x { // lower right
                rd += grid[y as usize][x as usize];
            } else if y > fordidden_y && x < forbidden_x { // lower left
                ld += grid[y as usize][x as usize];
            }
        }
        // println!("{:?}",grid[y as usize]);
    }
    total = lu * ru * ld * rd;

    
    


    println!("AOC Day 14 Part 1 Total: {}",total);
    println!("Time: {:?}",start.elapsed());


    let p2 = true;
    if p2 {
        let start_2 = Instant::now();
        for i in 3000..10000 {
            let mut ni = instructions_copy.clone();
            let mut grid_p2: Vec<Vec<char>> = Vec::new();
            for _ in 0..max_y {
                let mut temp = Vec::new();
                for _ in 0..max_x {
                    temp.push('.');
                }
                grid_p2.push(temp);
            }
            for instruction in ni {
                
                let [p_x, p_y, v_x, v_y] = parse_coordinates(&instruction);
                let [x, y] = process_robot([p_x, p_y], [v_x, v_y], i, max_x, max_y);
                // println!("{}:{}",x,y);
                
                if grid_p2[y][x] == '.' {
                    grid_p2[y][x] = '1';
                } else {
                    grid_p2[y][x] = std::char::from_u32((grid_p2[y][x] as u32) + 1).unwrap();
                }                
            }
            // println!("{}",i);
            // if (i > 60 && i < 65) || (i > 80 && i < 84) {
            //     for line in grid_p2 {
            //         println!("{:?}",line);
            //     }
            //     println!()
            // }
            if grid_p2[33].iter().filter(|&&c| c != '.').count() > 15 {
                append_grid_to_file(&grid_p2, i as usize, "display.txt")?;
            }
            
        }
        println!("Time: {:?}",start_2.elapsed());
    }

    

    Ok(()) 
}