const fs = require('fs');
const file_op = fs.readFileSync("input_operations.txt", "utf8");
const file_pages = fs.readFileSync("input_pages.txt", "utf8");
// const file_op = fs.readFileSync("test1.txt", "utf8");
// const file_pages = fs.readFileSync("test2.txt", "utf8");

const array_op_string = file_op.split('\r\n');
const array_pages_string = file_pages.split('\r\n');
var operations = [];
var pages_2darray = [];

array_op_string.forEach(line => {
    let pair = [];
    let [left, right] = line.split('|');
    pair.push(parseInt(left));
    pair.push(parseInt(right));
    operations.push(pair);
});

array_pages_string.forEach(line => {
    let int_pages = [];
    const pages = line.split(',');
    pages.forEach(page_number => {
        int_pages.push(parseInt(page_number));
    })
    pages_2darray.push(int_pages);
});

function get_constraints(page) {
    let commands = [];
    operations.forEach(operation_pair => {
        if (page.includes(operation_pair[0]) && page.includes(operation_pair[1])) {
            commands.push(operation_pair);
        } 
    });
    return commands;
}

function validate_page(commands, page) {
    let answer = true;
    commands.forEach(command => {
        let left = page.indexOf(command[0]);
        let right = page.indexOf(command[1]);
        if (left >= right) {
            answer = false;
        }
    });
    return answer;
}

function fix_page(pages, commands) {
    let fixed_pages = structuredClone(pages); 
    let fixing = true;
    while (fixing) {
        fixing = false;
        commands.forEach(command => {
            let left_index = fixed_pages.indexOf(command[0]);
            let right_index = fixed_pages.indexOf(command[1]);
            if (left_index > right_index) {
                fixing = true;
                const [pull] = fixed_pages.splice(left_index, 1);
                if (right_index > 0) {
                    fixed_pages.splice(right_index-1,0,pull);
                } else {
                    fixed_pages.unshift(pull);
                }
            }
        })
    }
    return fixed_pages;
}


function main() {
    var validated_pages = [];
    var incorrectly_ordered = [];
    var total = 0;
    var total_2 = 0;

    pages_2darray.forEach(page => {
        let commands = get_constraints(page);
        let valid = validate_page(commands, page);
        if (valid) {
            validated_pages.push(page);
        } else {
            incorrectly_ordered.push(fix_page(page, commands));
        }
    })

    validated_pages.forEach(page => {
        let index = Math.round(page.length/2)-1;
        total += page[index];
    })

    incorrectly_ordered.forEach(page => {
        let index = Math.round(page.length/2)-1;
        total_2 += page[index];
    })

    console.log("AOC Day5 Part 1 Total:",total);
    console.log("AOC Day 5 Part 2 Totla:",total_2);
}

main();