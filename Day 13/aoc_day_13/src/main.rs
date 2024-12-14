use std::fs::File;
use std::io::{self, BufRead};
use std::collections::{HashMap, HashSet};
use std::time::Instant;
use regex::Regex;


fn algebra2(x1: i64, x2: i64, y1: i64, y2: i64, x_check: i64, y_check: i64) -> [i64; 2] {
    let mut results: [i64; 2] = [1000000000000000, 1000000000000000];

    

    for i in 0..1000 {
        for t in 0..1000 {
            if i * x1 + t * x2 == x_check && i * y1 + t * y2 == y_check {
                if i + t < results[0] + results[1] {
                    results[0] = i;
                    results[1] = t;
                }
            }
        }
    }
    results
}

fn algebra(x1: i64, x2: i64, y1: i64, y2: i64, x_check: i64, y_check: i64) -> [i64; 2] {
    let mut results: [i64; 2] = [1000000000000000, 1000000000000000];
    for i in 0..1000 {
        for t in 0..1000 {
            if i * x1 + t * x2 == x_check && i * y1 + t * y2 == y_check {
                if i + t < results[0] + results[1] {
                    results[0] = i;
                    results[1] = t;
                }
            }
        }
    }
    results
}

fn main() -> io::Result<()> {

    let path = "test.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: i64 = 0;
    let mut total_p2: i64 = 0;
    let mut instructions: Vec<String> = reader.lines().filter_map(Result::ok).collect();
    let mut x1: i64 = 0;
    let mut x2: i64 = 0;
    let mut y1: i64 = 0;
    let mut y2: i64 = 0;
    let mut x_check: i64 = 0;
    let mut y_check: i64 = 0;


    let mut i = 0;
    for line in instructions {
        let re = Regex::new(r"(Button \w+|Prize): X[+=](\d+), Y[+=](\d+)").unwrap();
        if let Some(captures) = re.captures(&line) {
            let label = captures.get(1).unwrap().as_str();
            let x = captures.get(2).unwrap().as_str().parse::<i64>().unwrap();
            let y = captures.get(3).unwrap().as_str().parse::<i64>().unwrap();

            if line.contains("Prize") { 
                i = 0;
                x_check = x;
                y_check = y;
                let mut result: [i64; 2] = algebra(x1, x2, y1, y2, x_check, y_check);
                if result[0] != 1000000000000000 && result[1] != 1000000000000000 {
                    total += result[0] * 3 + result[1] * 1;
                    // println!();
                } 
                
            } else {
                i += 1;
                if i == 1 {
                    x1 = x;
                    y1 = y;
                } else if i == 2 {
                    x2 = x;
                    y2 = y;
                }
            }
            println!("Label: {}, X: {}, Y: {}", label, x, y);  
        }
    }

    println!("Total AOC Day 13 Part 1: {:?}",total);

    Ok(())
}
