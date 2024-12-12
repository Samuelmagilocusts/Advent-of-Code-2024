use std::default;
use std::io;
use std::collections::HashSet;
use std::collections::HashMap;
use std::time::Instant;

use rayon::collections::hash_map;

fn remove_leading_zeros(s: &str) -> &str { 
    let trimmed = s.trim_start_matches('0'); 
    if trimmed.is_empty() { 
        "0"
     } else { 
        trimmed 
    } 
}

fn is_even(number: u64) -> bool {
    if number < 10 {
        false
    } else if number < 100 {
        true
    } else if number < 1000 {
        false
    } else if number < 10000 {
        true
    } else if number < 100000 {
        false
    } else if number < 1000000 {
        true
    } else if number < 10000000 {
        false
    } else if number < 100000000 {
        true
    } else if number < 1000000000 {
        false
    } else if number < 10000000000 {
        true
    } else if number < 100000000000 {
        false
    } else if number < 1000000000000 {
        true
    } else if number < 10000000000000 {
        false
    } else if number < 100000000000000 {
        true
    } else if number < 1000000000000000 {
        false
    } else if number < 10000000000000000 {
        true
    } else if number < 100000000000000000 {
        false
    } else if number < 1000000000000000000 {
        true
    } else if number < 10000000000000000000 {
        false
    } else {
        println!();
        false
    }
}

// fn process(stones: Vec<u64>) {
//     let mut amount_ones: i64 = 0;
//     let mut amount_mul: i64 = 0;
//     let mut aditional: i64 = 0;
//     for stone in stones {
//         if stone == 0 {
//             amount_ones += 1;
//         } else if is_even(stone) {
//             aditional += 1;
//         } else {
//             stone * 2024;
//         }
//     }
// }


fn process(stones: Vec<u64>) -> Vec<u64> {
    let mut new_vec: Vec<u64> = Vec::new();
    for stone in stones {
        if stone == 0 {
            new_vec.push(1);
        } else if is_even(stone) == true {
            let string_stone = stone.to_string();
            let (left, right) = string_stone.split_at(string_stone.len() / 2);
            let left_num = remove_leading_zeros(left).parse::<u64>().expect("Failed to parse left part");
            let right_num = remove_leading_zeros(right).parse::<u64>().expect("Failed to parse right part");
            new_vec.push(left_num); 
            new_vec.push(right_num);
        } else {
            new_vec.push(stone * 2024);
        }
    }
    new_vec
}

fn main() -> io::Result<()> {
    let mut numbers: Vec<u64> = [112, 1110, 163902, 0, 7656027, 83039, 9, 74].to_vec();
    // let mut numbers: Vec<u64> = [125, 17].to_vec();
    let mut total: u64 = 0;
    let mut workspace: Vec<u64> = Vec::new();
    // let mut hash_map: HashMap<>

    for (t, num) in numbers.iter().enumerate() {
        workspace.push(*num);
        let startMain = Instant::now(); 

        for i in 0..75 {
            let start = Instant::now(); 
            workspace = process(workspace);
            // println!("For {}, Time taken: {:?}, Size: {}",i,duration,numbers.len());
            let duration = start.elapsed();
            println!("Curent section: {}. Current blink: {}. Time elapsed: {:?}",t,i,duration);
        }

        total += workspace.len() as u64;
        let durationMain = startMain.elapsed();
        println!("Time taken: {:?} on section: {}",durationMain,t);
        workspace.clear();
    }

    println!("AOC Day 11 Part 2 Total: {}", total); 

    Ok(())
}
