
fn algebra(x1: i32, x2: i32, y1: i32, y2: i32, x_check: i32, y_check: i32) -> Vec<[i32;2]> {
    let mut results: Vec<[i32; 2]> = Vec::new();
    for i in 0..1000 {
        for t in 0..1000 {
            if i * x1 + t * x2 == x_check && i * y1 + t * y2 == y_check {
                results.push([i,t]);
            }
        }
    }
    results
}

fn main() {
    println!("Hello, world!");
}
