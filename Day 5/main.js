const fs = require('fs');
const file_op = fs.readFileSync("C:/Users/Matthew/OneDrive - Schoolhouse Electronics LLC/Desktop/Extra/Coding projects/AOC 2024/Advent-of-Code-2024/Day 5/input_operations.txt", "utf8");
const file_pages = fs.readFileSync("C:/Users/Matthew/OneDrive - Schoolhouse Electronics LLC/Desktop/Extra/Coding projects/AOC 2024/Advent-of-Code-2024/Day 5/input_pages.txt", "utf8");
// console.log(file_pages);

const array_op_string = file_op.split('\r\n');
const array_pages_string = file_pages.split('\r\n');
var operations = [];
var pages_2darray = [];

array_op_string.forEach(line => {
    let pair = [];
    let [left, right] = line.split(':');
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
        if (page.contains(operation_pair[0]) && page.contains(operation_pair[1])) {
            commands.push(operation_pair);
        }
    });
    return commands;
}
function validate_page(commands, page) {
    commands.forEach(command => {
        if (page.indexof(command[0]) >= page.indexof(command[1])) {
            return false;
        }
    });
    return true;
}


function main() {
    var validated_pages = [];
    var total = 0;

    pages_2darray.forEach(page => {
        let commands = get_constraints(page);
        if (validate_page(commands, page)) {
            validated_pages.push(page);
        }
    })

    validated_pages.forEach(page => {
        total += page[page.length/2];
    })

    console.log("AOC Day5 Part 1 Total:",total);
}

main();