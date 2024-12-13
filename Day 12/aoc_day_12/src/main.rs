use std::fs::File;
use std::io::{self, BufRead};
use std::collections::{HashMap, HashSet};
use std::time::Instant;


fn build_param(lines: &Vec<Vec<char>>, character: char, (y, x): (usize, usize)) -> usize {
    let mut amount_push = 0;

    if x > 0 { // l
        if lines[y][x-1] != character {
            amount_push += 1;
        }
    } else {
        amount_push += 1;
    }

    if y > 0 { // u
        if lines[y-1][x] != character {
            amount_push += 1;
        }
    } else {
        amount_push += 1;
    }

    if x < lines.len()-1 { // r
        if lines[y][x+1] != character {
            amount_push += 1;
        }
    } else {
        amount_push += 1;
    }

    if y < lines.len()-1 { // d
        if lines[y+1][x] != character {
            amount_push += 1;
        }
    } else {
        amount_push += 1;
    }

    amount_push
}

fn recurse(grid: &Vec<Vec<char>>, character: char, (y, x): (usize, usize), visited: &mut HashSet<(usize, usize)>, saved: &mut Vec<[usize;2]>) -> (usize, usize) {
    let mut area: usize = 0;
    let mut param: usize = 0;

    if grid[y][x] == character {
        if !visited.contains(&(y,x)) {
            area += 1;
            param += build_param(grid, character, (y,x));
            visited.insert((y,x));
            saved.push([y,x]);
        }
    }

    if x > 0 { // l
        if grid[y][x-1] == character {
            if !visited.contains(&(y,x-1)) {
                let (a, p) = recurse(&grid, character, (y,x-1), visited, saved);
                area += a;
                param += p;
            }
        }
    } 

    if y > 0 { // u
        if grid[y-1][x] == character {
            if !visited.contains(&(y-1,x)) {
                let (a, p) = recurse(&grid, character, (y-1,x), visited, saved);
                area += a;
                param += p;
            }
        }
    } 

    if x < grid.len()-1 { // r
        if grid[y][x+1] == character {
            if !visited.contains(&(y,x+1)) {
                let (a, p) = recurse(&grid, character, (y,x+1), visited, saved);
                area += a;
                param += p;
            }
        }
    } 

    if y < grid.len()-1 { // d
        if grid[y+1][x] == character {
            if !visited.contains(&(y+1,x)) {
                let (a, p) = recurse(&grid, character, (y+1,x), visited, saved);
                area += a;
                param += p;
            }
        }
    } 
    
    (area, param)
}

fn main() -> io::Result<()> {
    let path = "test2.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: usize = 0;
    let mut total_p2: usize = 0;
    let mut grid: Vec<Vec<char>> = reader
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();

    let mut visited: HashSet<(usize, usize)> = HashSet::new();
    
    
    for y in 0..grid.len() {
        for x in 0..grid.len() {
            let mut coordinates: Vec<[usize; 2]> = Vec::new();
            if !visited.contains(&(y, x)) {
                let (a, p) = recurse(&grid, grid[y][x], (y, x), &mut visited, &mut coordinates);
                total += a * p;
                println!("{:?}",coordinates);
            }
        }
    }

    println!("AOC Day 12 P1 Total: {}",total);
    println!("AOC Day 12 P2 Total: {}",total_p2);

    Ok(())
}
