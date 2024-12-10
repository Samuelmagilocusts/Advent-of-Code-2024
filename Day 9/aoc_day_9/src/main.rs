use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Index;
use std::result;
use std::time::Instant;


fn main() -> io::Result<()> {
    let path = "input.txt";

    // Open the file
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);
    let mut total: i64 = 0;
    let mut total_p2: i64 = 0;

    // Parse numbers from file
    let numbers: Vec<i32> = reader
        .lines() // Read lines from the file
        .filter_map(|line| line.ok()) // Handle potential IO errors
        .flat_map(|line| line.chars().collect::<Vec<char>>()) // Collect characters into Vec<char>
        .filter_map(|c| c.to_digit(10)) // Convert characters to digits
        .map(|digit| digit as i32) // Convert digits to i32
        .collect();

    let mut sorted_line: Vec<i32> = Vec::new();

    let mut t: i32 = 0;
    for i in 0..numbers.len() {
        let mut update: bool = false;
        for _ in 0..numbers[i] {
            if i % 2 == 0 {
                sorted_line.push(t as i32);
                update = true;
                
            } else {
                sorted_line.push(-1);
            }
        }
        if update {
            t += 1;
        }
    }

    let mut organ: Vec<i32> = Vec::new(); 
    let number_of_dots = sorted_line.iter().filter(|&&x| x == -1).count();

    let mut last: usize = sorted_line.len()-1;
    for i in 0..sorted_line.len()-number_of_dots {
        if sorted_line[i] == -1 {
            let mut my_new_num = 0;
            loop {
                if sorted_line[last] == -1 {
                    last -= 1;
                } else {
                    my_new_num = sorted_line[last];
                    last -= 1;
                    break;
                }
            }
            organ.push(my_new_num);
        } else {
            organ.push(sorted_line[i]);
        }
    }

    
    for i in 0..organ.len() {
        total += (organ[i] as i64) * (i as i64);
    }

    let start = Instant::now();
    

    let mut current_group: Vec<i32> = Vec::new();
    let mut groups: Vec<Vec<i32>> = Vec::new();

    for (index, &value) in sorted_line.iter().enumerate() {
        // If the current group is empty or matches the last value, add it to the group
        if current_group.is_empty() || current_group.last() == Some(&value) {
            current_group.push(value);
        } else {
            // Otherwise, save the group and start a new one
            groups.push(current_group.clone());
            current_group.clear();
            current_group.push(value);
        }
    }
    groups.push(current_group.clone());

    let mut arranged_groups: Vec<Vec<i32>> = groups.clone();
    groups.reverse();
    // println!("{:?}",groups);

    for group in groups {
        // println!("{:?} : gourp: {:?}",arranged_groups, group);
        if !group.contains(&-1) {
            // find first iteration that is big enough for pattern
            let mut dot_location: usize = 100000;
            let mut num_location: usize = 0;
            let pattern = vec![-1; group.len()];
            for (pos, block) in arranged_groups.iter().enumerate() {
                if block.contains(&-1) {
                    if block.len() >= group.len() {
                        dot_location = pos;
                        break;
                    }
                }
            }

            if dot_location == 100000 {
                continue;
            }

            for (pos, block) in arranged_groups.iter().enumerate() {
                if block == &group {
                    num_location = pos;
                    break;
                }
            }

            if dot_location < num_location {
                if arranged_groups[dot_location].len() > group.len() {
                    let new_chuck = vec![-1; arranged_groups[dot_location].len()-group.len()];
                    arranged_groups[dot_location] = group;
                    arranged_groups[num_location] = pattern;
                    arranged_groups.insert(dot_location+1, new_chuck);
                } else {
                    arranged_groups[dot_location] = group;
                    arranged_groups[num_location] = pattern;
                }
            } 
        }
    }

    // println!("{:?}",arranged_groups);

    let result_vec: Vec<i32> = arranged_groups.into_iter().flat_map(|row| row).collect();
    for (pos, num) in result_vec.iter().enumerate() {
        if *num != -1 {
            total_p2 += (pos as i32 * num) as i64;
        } 
    }
    


    let duration = start.elapsed(); // Measure the elapsed time
    println!("Time elapsed: {:?}", duration);
    // println!("{:?}", organ);
    println!("AOC Day 9 Part 1 Total: {}",total);
    println!("AOC Day 9 Part 2 Total: {}",total_p2);

    Ok(())
}
