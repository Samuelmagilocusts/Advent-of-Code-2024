use std::fs::File;
use std::io::{self, BufRead};
use std::time::Instant;


fn algebra2(x1: f64, x2: f64, y1: f64, y2: f64, x_check: f64, y_check: f64) -> [u64; 2] {
    let b: f64 = ((y_check * x1 - x_check * y1) / ((x2 * x1 + x2 * y1)-(x1 * x2 + x1 * y2))).abs();
    let a: f64 = (x_check - x2 * b) / x1;


    if a.fract() == 0.0 && b.fract() == 0.0 {
        [a as u64, b as u64]
    } else {
        [0,0]
    }
}


fn main() -> io::Result<()> {

    let path = "input.txt";
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: u64 = 0;
    let instructions: Vec<String> = reader.lines().filter_map(Result::ok).collect();
    let mut x1: f64 = 0.0;
    let mut x2: f64 = 0.0;
    let mut y1: f64 = 0.0;
    let mut y2: f64 = 0.0;
    let mut x_check: f64 = 0.0;
    let mut y_check: f64 = 0.0;
    let p2 = true;


    let mut i = 0;
    let time = Instant::now();
    for line in instructions {
        if i == 2 { 
            let x_temp = line.split("X=").nth(1).unwrap().split(',').nth(0).unwrap().trim();
            let y_temp = line.split("Y=").nth(1).unwrap().trim();

            let x = x_temp.parse::<f64>().unwrap();
            let y = y_temp.parse::<f64>().unwrap();

            if p2 {
                x_check = x+10000000000000.0;
                y_check = y+10000000000000.0;
            } else {
                x_check = x;
                y_check = y;
            }
            // println!("Label: {}, X: {}, Y: {}", label, x_check, y_check);  
            
            let result: [u64; 2] = algebra2(x1, x2, y1, y2, x_check, y_check);
            if result[0] != 0 && result[1] != 0 {
                total += result[0] * 3 + result[1] * 1;
            } 
            i += 1;
            
        } else if i < 2 {
            let x_temp = line.split("X+").nth(1).unwrap().split(',').nth(0).unwrap().trim();
            let y_temp = line.split("Y+").nth(1).unwrap().trim();

            let x = x_temp.parse::<f64>().unwrap();
            let y = y_temp.parse::<f64>().unwrap();

            
            if i == 0 {
                x1 = x;
                y1 = y;
            } else if i == 1 {
                x2 = x;
                y2 = y;
            }

            i += 1;
            // println!("Label: {}, X: {}, Y: {}", label, x, y);  
        } else {
            i = 0;
        }
    }
    println!("Time: {:?}", time.elapsed());

    if p2 {
        println!("Total AOC Day 13 Part 2: {:?}",total);
    } else {
        println!("Total AOC Day 13 Part 1: {:?}",total);
    }

    Ok(())
}
