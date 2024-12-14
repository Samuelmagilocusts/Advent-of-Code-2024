use std::fs::File;
use std::io::{self, BufRead};
use std::collections::{HashMap, HashSet};
use std::time::Instant;


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

    Ok(()) 
}