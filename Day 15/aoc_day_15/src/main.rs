use std::fs::File;
use std::io::{self, BufRead};
use std::collections::HashSet;
use std::process::{Command, Output};
use std::time::Instant;

fn shift_line(grid: &mut Vec<Vec<char>>, direction: char, [x, y]: [usize; 2]) {
    let mut index = 0;
    match direction {
        '<' => {
            
            for i in (0..=x).rev() {
                if grid[y][i] == '.' {
                    index = i;
                    break;
                }
            }            

            for i in index..x {
                grid[y][i] = grid[y][i+1];
            }
        },
        '>' => {
            for i in x..grid.len() {
                if grid[y][i] == '.' {
                    index = i;
                    break;
                }
            }
            for i in (x..=index).rev() {
                grid[y][i] = grid[y][i-1];
            }
        },
        '^' => {
            for i in (0..=y).rev() {
                if grid[i][x] == '.' {
                    index = i;
                    break;
                }
            }

            for i in index..y {
                grid[i][x] = grid[i+1][x];
            }
        },
        'v' => {
            for i in y..grid.len() {
                if grid[i][x] == '.' {
                    index = i;
                    break;
                }
            }

            for i in (y..=index).rev() {
                grid[i][x] = grid[i-1][x];
            }
        },
        _ => {

        }
    }
    grid[y][x] = '.'
}

fn grid_range_contains(grid: &mut Vec<Vec<char>>, direction: char, [x, y]: [usize; 2]) -> bool {
    let mut output = false;
    let mut first_hash: i32 = -1;
    let mut first_dot: i32 = -1;
    match direction {
        '<' => {
            for i in (1..=x).rev() {
                if grid[y][i] == '.' {
                    first_dot = i as i32;
                    break;
                }
            }
            for i in (1..=x).rev() {
                if grid[y][i] == '#' {
                    first_hash = i as i32;
                    break;
                }
            }
            if first_dot == -1 {
                output = false;
            } else {
                if first_hash == -1 {
                    output = true;
                } else {
                    if first_dot > first_hash {
                        output = true;
                    } else {
                        output = false;
                    }
                }
            }
        },
        '>' => {
            for i in x..grid.len()-1 {
                if grid[y][i] == '.' {
                    first_dot = i as i32;
                    break;
                } 
            }

            for i in x..grid.len()-1 {
                if grid[y][i] == '#' {
                    first_hash = i as i32;
                    break;
                } 
            }

            if first_dot == -1 {
                output = false;
            } else {
                if first_hash == -1 {
                    output = true;
                } else {
                    if first_dot < first_hash {
                        output = true;
                    } else {
                        output = false;
                    }
                }
            }
        },
        '^' => {
            for i in (1..=y).rev() {
                if grid[i][x] == '.' {
                    first_dot = i as i32;
                    break;
                }
            }
            for i in (1..=y).rev() {
                if grid[i][x] == '#' {
                    first_hash = i as i32;
                    break;
                }
            }
            if first_dot == -1 {
                output = false;
            } else {
                if first_hash == -1 {
                    output = true;
                } else {
                    if first_dot > first_hash {
                        output = true;
                    } else {
                        output = false;
                    }
                }
            }
        },
        'v' => {
            for i in y..grid.len()-1 {
                if grid[i][x] == '.' {
                   first_dot = i as i32;
                } 
            }
            for i in y..grid.len()-1 {
                if grid[i][x] == '#' {
                   first_hash = i as i32;
                } 
            }
            if first_dot == -1 {
                output = false;
            } else {
                if first_hash == -1 {
                    output = true;
                } else {
                    if first_dot < first_hash {
                        output = true;
                    } else {
                        output = false;
                    }
                }
            }
        }, 
        _ => {

        }
    }
    
    output
}

fn build_frame(grid: &mut Vec<Vec<char>>, direction: char, [x, y]: &mut [usize; 2]) {
    
    match direction {
        '<' => {
            if *x > 0 {
                match grid[*y][*x-1] {
                    '.' => {
                        grid[*y][*x] = '.';
                        grid[*y][*x-1] = '@';
                        *x -= 1;
                    },
                    'O' => {
                        if grid_range_contains(grid, direction, [x.clone(),y.clone()]) {
                            shift_line(grid, direction, [*x,*y]);
                            *x -= 1;
                        }
                    },
                    '#' => {

                    },
                    _ => {

                    }
                }
            }
        },
        '>' => {
            if *x < grid.len() {
                match grid[*y][*x+1] {
                    '.' => {
                        grid[*y][*x] = '.';
                        grid[*y][*x+1] = '@';
                        *x += 1;
                    },
                    'O' => {
                        if grid_range_contains(grid, direction, [x.clone(),y.clone()]) {
                            shift_line(grid, direction, [*x,*y]);
                            *x += 1;
                        }
                    },
                    '#' => {

                    },
                    _ => {

                    }
                }
            }

        },
        '^' => {
            if *y > 0 {
                match grid[*y-1][*x] {
                    '.' => {
                        grid[*y][*x] = '.';
                        grid[*y-1][*x] = '@';
                        *y -= 1;
                    },
                    'O' => {
                        if grid_range_contains(grid, direction, [x.clone(),y.clone()]) {
                            shift_line(grid, direction, [*x,*y]);
                            *y -= 1;
                        }
                    },
                    '#' => {

                    },
                    _ => {

                    }
                }
            }
        },
        'v' => {
            if *y < grid.len() {
                match grid[*y+1][*x] {
                    '.' => {
                        grid[*y][*x] = '.';
                        grid[*y+1][*x] = '@';
                        *y += 1;
                    },
                    'O' => {
                        if grid_range_contains(grid, direction, [x.clone(),y.clone()]) {
                            shift_line(grid, direction, [*x,*y]);
                            *y += 1;
                        }
                    },
                    '#' => {

                    },
                    _ => {

                    }
                }
            }
        },
        _ => {

        }
    }
}

fn print_grid(grid: &mut Vec<Vec<char>>) {
    for line in grid {
        println!("{:?}",line);
    }
}

fn main() -> io::Result<()> {
    let path = "input.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: usize = 0;
    let mut grid: Vec<Vec<char>> = reader
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();

    let testgrid = grid.clone();

    let path_c = "commands.txt";
    let file_c = File::open(path_c)?;
    let reader_c = io::BufReader::new(file_c);
    let mut total: usize = 0;
    let mut commands: Vec<Vec<char>> = reader_c
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();
    
    
    let start_time = Instant::now();
    let mut start = [4,4];
    for i in 0..grid.len() {
        for t in 0..grid.len() {
            if grid[i][t] == '@' {
                start = [t,i];
                break;
            }
        }
    }

    // print_grid(&mut grid);

    for line in commands {
        for com in line {
            if com != '\n' && com != '\r' {
                build_frame(&mut grid, com, &mut start);
                // print_grid(&mut grid);
                // println!();
            }
        }
    }

    // print_grid(&mut grid);

    for i in 0..grid.len() {
        for t in 0..grid.len() {
            if testgrid[i][t] == '#' && grid[i][t] != testgrid[i][t] {
               println!("Moved #. ({},{})",t,i);
            }
        }
    } 

    for i in 0..grid.len() {
        for t in 0..grid.len() {
            if grid[i][t] == 'O' {
                total += i*100 + t;
            }
        }
    }

    println!("AOC Day 15 Part 1 Total: {}",total);
    println!("Time: {:?}",start_time.elapsed());

    Ok(())
}
