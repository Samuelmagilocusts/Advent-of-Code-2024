use std::fs::File;
use std::io::{self, BufRead};
use std::collections::HashSet;
use std::time::Instant;

struct Point {
    lu: bool,
    ru: bool,
    lb: bool,
    rb: bool
}

fn build_full_param(lines: &Vec<Vec<char>>, character: char, (y, x): (usize, usize), point_data: &mut Vec<Vec<Point>>) -> usize {
    let mut amount_corners = 0;
    let mut four_corners: [bool; 4] = [false, false, false, false];
    let mut diagonals: [bool; 4] = [false, false, false, false];

    if x > 0 { // l
        if lines[y][x-1] != character {
            four_corners[0] = true;
        }
    } else {
        four_corners[0] = true;
    }

    if y > 0 { // u
        if lines[y-1][x] != character {
            four_corners[1] = true;
        }
    } else {
        four_corners[1] = true;
    }

    if x < lines.len()-1 { // r
        if lines[y][x+1] != character {
            four_corners[2] = true;
        }
    } else {
        four_corners[2] = true;
    }

    if y < lines.len()-1 { // d
        if lines[y+1][x] != character {
            four_corners[3] = true;
        }
    } else {
        four_corners[3] = true;
    }

    if x > 0 && y > 0 { // left up
        if lines[y-1][x-1] == character{
            diagonals[0] = true;
        }
    } 

    if x < lines.len()-1 && y > 0 { // right up
        if lines[y-1][x+1] == character{
            diagonals[1] = true;
        }
    }

    if x < lines.len()-1 && y < lines.len()-1 { // right down
        if lines[y+1][x+1] == character{
            diagonals[2] = true;
        }
    }

    if x > 0 && y < lines.len()-1 { // left down
        if lines[y+1][x-1] == character{
            diagonals[3] = true;
        }
    }

    if (four_corners[0] == true && four_corners[1] == true) { // left upper
        amount_corners += 1;
        point_data[y][x].lu = true;
    }
    
     // left top corner left & top not the same
    if (four_corners[0] == false && four_corners[1] == true && diagonals[0] == true) ||
        (four_corners[0] == true && four_corners[1] == false && diagonals[0] == true)
    {
        if y != 0 && x != 0 {
            if point_data[y-1][x-1].rb == false {
                amount_corners += 1;
                point_data[y][x].lu = true;
            } 
        }
    }

    if four_corners[1] == true && four_corners[2] == true { // right upper
        amount_corners += 1;
        point_data[y][x].ru = true;
    }
    
     // right top corner top & right not the same
    if    (four_corners[1] == false && four_corners[2] == true && diagonals[1] == true) ||
        (four_corners[1] == true && four_corners[2] == false && diagonals[1] == true)
    {
        if y != 0 && x != lines.len()-1 {
            if point_data[y-1][x+1].lb == false {
                amount_corners += 1;
                point_data[y][x].ru = true;
            } 
        }
    }

    if four_corners[2] == true && four_corners[3] == true { // right bottom
        amount_corners += 1;
        point_data[y][x].rb = true;
    } 
    if // right bottom corner rigth & bottom not the same
        (four_corners[2] == false && four_corners[3] == true && diagonals[2] == true) ||
        (four_corners[2] == true && four_corners[3] == false && diagonals[2] == true)
    {
        if y != lines.len()-1 && x != lines.len()-1 {
            if point_data[y+1][x+1].lu == false {
                amount_corners += 1;
                point_data[y][x].rb = true;
            } 
        }
    }

    if four_corners[3] == true && four_corners[0] == true { // left bottom
        amount_corners += 1;
        point_data[y][x].lb = true;
    } // left bottom corner bottom & left not the same
    if    (four_corners[3] == false && four_corners[0] == true && diagonals[3] == true) ||
        (four_corners[3] == true && four_corners[0] == false && diagonals[3] == true)
    {
        if y != lines.len()-1 && x != 0 {
            if point_data[y+1][x-1].ru == false {
                amount_corners += 1;
                point_data[y][x].lb = true;
            } 
        }
    }

    amount_corners
}

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

fn recurse(grid: &Vec<Vec<char>>, character: char, (y, x): (usize, usize), visited: &mut HashSet<(usize, usize)>, saved: &mut Vec<[usize;2]>, p2: bool, point_data: &mut Vec<Vec<Point>>) -> (usize, usize) {
    let mut area: usize = 0;
    let mut param: usize = 0;

    if grid[y][x] == character {
        if !visited.contains(&(y,x)) {
            area += 1;
            if p2 {
                param += build_full_param(grid, character, (y,x), point_data);
            } else {
                param += build_param(grid, character, (y,x));
            }
            visited.insert((y,x));
            saved.push([y,x]);
        }
    }

    if x > 0 { // l
        if grid[y][x-1] == character {
            if !visited.contains(&(y,x-1)) {
                let (a, p) = recurse(&grid, character, (y,x-1), visited, saved, p2, point_data);
                area += a;
                param += p;
            }
        }
    } 

    if y > 0 { // u
        if grid[y-1][x] == character {
            if !visited.contains(&(y-1,x)) {
                let (a, p) = recurse(&grid, character, (y-1,x), visited, saved, p2, point_data);
                area += a;
                param += p;
            }
        }
    } 

    if x < grid.len()-1 { // r
        if grid[y][x+1] == character {
            if !visited.contains(&(y,x+1)) {
                let (a, p) = recurse(&grid, character, (y,x+1), visited, saved, p2, point_data);
                area += a;
                param += p;
            }
        }
    } 

    if y < grid.len()-1 { // d
        if grid[y+1][x] == character {
            if !visited.contains(&(y+1,x)) {
                let (a, p) = recurse(&grid, character, (y+1,x), visited, saved, p2, point_data);
                area += a;
                param += p;
            }
        }
    } 
    
    (area, param)
}

fn main() -> io::Result<()> {
    let path = "input.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: usize = 0;
    let grid: Vec<Vec<char>> = reader
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();

    let mut visited: HashSet<(usize, usize)> = HashSet::new();
    let p2 = true;
    let start = Instant::now();
    let mut point_data: Vec<Vec<Point>> = Vec::new();

    if p2 {
        for _ in 0..grid.len() {
            let mut temp: Vec<Point> = Vec::new();
            for _ in 0..grid.len() {
                let point = Point {lu: false, ru: false, lb: false, rb: false};
                temp.push(point);
            }
            point_data.push(temp);
        } 
    }

    for y in 0..grid.len() {
        for x in 0..grid.len() {
            let mut coordinates: Vec<[usize; 2]> = Vec::new();
            if !visited.contains(&(y, x)) {
                let (a, p) = recurse(&grid, grid[y][x], (y, x), &mut visited, &mut coordinates, p2, &mut point_data);
                println!("{}",grid[y][x]);
                total += a * p;
                // println!("{:?}",coordinates);
            }
        }
    }
    println!("Time: {:?}",start.elapsed());

    if p2 {
        println!("AOC Day 12 P2 Total: {}",total);
    } else {
        println!("AOC Day 12 P1 Total: {}",total);
    }

    Ok(())
}
