use std::io;
use std::collections::HashSet;
use std::collections::HashMap;
use std::time::Instant;

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


fn process(stones: &mut HashMap<u64, u64>, cashe: &mut HashMap<u64, [u64; 2]>) -> HashMap<u64, u64> {
    let mut new_map: HashMap<u64, u64> = HashMap::new();
    for (&stone, quantity) in stones {
        if stone == 0 {
            new_map.entry(1).and_modify(|f|*f+=*quantity).or_insert(*quantity);
        } else if is_even(stone) == true {
            if let Some([left, right]) = cashe.get(&stone) {
                new_map.entry(*left).and_modify(|f|*f+=*quantity).or_insert(*quantity);
                new_map.entry(*right).and_modify(|f|*f+=*quantity).or_insert(*quantity);
            } else {
                let string_stone = stone.to_string();
                let (left, right) = string_stone.split_at(string_stone.len() / 2);
                let left_num = remove_leading_zeros(left).parse::<u64>().expect("Failed to parse left part");
                let right_num = remove_leading_zeros(right).parse::<u64>().expect("Failed to parse right part");
                new_map.entry(left_num).and_modify(|f|*f+=*quantity).or_insert(*quantity);
                new_map.entry(right_num).and_modify(|f|*f+=*quantity).or_insert(*quantity);
                cashe.entry(stone).insert_entry([left_num,right_num]);
            }
        } else {
            let temp_mul: u64 = stone*2024;
            new_map.entry(temp_mul).and_modify(|f|*f+=*quantity).or_insert(*quantity);
        }
    }
    new_map
}

fn main() -> io::Result<()> {
    let mut numbers: Vec<u64> = [112, 1110, 163902, 0, 7656027, 83039, 9, 74].to_vec();
    // let mut numbers: Vec<u64> = [125, 17].to_vec();
    let mut total: u64 = 0;
    let mut hash_map: HashMap<u64, u64> = HashMap::new();
    let mut cashe: HashMap<u64, [u64; 2]> = HashMap::new();

    for num in numbers {
        hash_map.insert(num, 1);
    }

    let startMain = Instant::now(); 
    for i in 0..75 {
        let start = Instant::now(); 
        hash_map = process(&mut hash_map,  &mut cashe);
        
        let duration = start.elapsed();
        // println!("{:?}",hash_map);
        // println!("Curent section: {}. Current blink: {}. Time elapsed: {:?}",t,i,duration);
        println!("For {}, Time taken: {:?}",i,duration);
    }

    // println!("{:?}",hash_map);

    for value in hash_map.values() {
        total += value;
    }
    println!();

    let durationMain = startMain.elapsed();
    println!("Time taken: {:?}",durationMain);
    

    println!("AOC Day 11 Part 2 Total: {}", total); 

    Ok(())
}
