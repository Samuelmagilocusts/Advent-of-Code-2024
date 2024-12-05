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
    let [left, right] = line.split('|');
    // console.log("left",left,"right",right)
    pair.push(parseInt(left));
    pair.push(parseInt(right));
    operations.push(pair);
});
// console.log(operations);

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
        // console.log("31",operation_pair,"page",page);
        if (page.includes(operation_pair[0]) && page.includes(operation_pair[1])) {
            commands.push(operation_pair);
        }
    });
    return commands;
}
function validate_page(commands, page) {
    commands.forEach(command => {
        // console.log(command)
        if (command[0] < command[1]) {
            if (page.indexOf(command[0]) > page.indexOf(command[1])) {
                return false;
            }
        } else {
            if (page.indexOf(command[0]) < page.indexOf(command[1])) {
                return false;
            }
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
        total += page[Math.round(page.length/2)];
    })

    console.log("AOC Day5 Part 1 Total:",total);
}

main();