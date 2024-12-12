use std::fs::File;
use std::io::{self, BufRead};
use std::collections::{HashMap, HashSet};
use std::time::Instant;

fn push_per(per: &mut HashMap<char, i32>, character: char) {
    per.entry(character)
        .and_modify(|num_of_per| *num_of_per += 1)
        .or_insert(1);
}

fn main() -> io::Result<()> {
    let path = "test1.txt";
    let alphabet = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'];
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: i32 = 0;
    let mut lines: Vec<Vec<char>> = reader
        .lines()
        .filter_map(Result::ok)
        .map(|line| line.chars().collect())
        .collect();
    let mut area: HashMap<char, i32> = HashMap::new();
    let mut per: HashMap<char, i32> = HashMap::new();

    for (y, line) in lines.iter().enumerate() {
        for (x, character) in line.iter().enumerate() {
            area.entry(*character)
                        .and_modify(|num_of_area| *num_of_area += 1)
                        .or_insert(1);
            
            if x > 0 { // l
                if lines[y][x-1] != *character {
                    push_per(&mut per, *character);
                }
            } else {
                push_per(&mut per, *character);
            }

            if y > 0 { // u
                if lines[y-1][x] != *character {
                    push_per(&mut per, *character);
                }
            } else {
                push_per(&mut per, *character);
            }

            if x < line.len()-1 { // r
                if lines[y][x+1] != *character {
                    push_per(&mut per, *character);
                }
            } else {
                push_per(&mut per, *character);
            }

            if y < line.len()-1 { // d
                if lines[y+1][x] != *character {
                    push_per(&mut per, *character);
                }
            } else {
                push_per(&mut per, *character);
            }
        }
    }

    println!("{:?}",area);
    println!("{:?}",per);

    for c in alphabet {
        if let Some(a) = area.get(&c) {
            if let Some(p) = per.get(&c) {
                total += *a * *p;
            }
        }
    }

    println!("AOC Day 12 P1 Total: {}",total);

    Ok(())
}
