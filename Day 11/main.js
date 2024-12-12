const fs = require('fs');
const file = fs.readFileSync("input.txt", "utf8");

function remove_leading_zeros(string) {
    while (string[0] === "0" && string.length > 1) {
        string = string.substring(1);
    }
    return string;
}

function process_line(line) {
    newLine = [];

    // console.log(line);

    line.forEach(element => {
        // console.log(element.length,element.length%2)
        if (parseInt(element) == 0) {
            newLine.push("1");
        } else if (element.length % 2 == 0) {
            let middle = Math.floor(element.length / 2);
            let left = element.substring(0, middle);
            let right = element.substring(middle);
            // console.log("left",left,"right",right);
            newLine.push(remove_leading_zeros(left));
            newLine.push(remove_leading_zeros(right));
        } else {
            newLine.push((parseInt(element)*2024).toString());
        }
    });

    return newLine;
}

function main() {
    let currentLine = file.split(' ');

    for (let i = 0; i < 75; i++) {
        let time = Date.now();
        currentLine = process_line(currentLine);
        // console.log(i,currentLine);///////////;
        console.log(i+1,currentLine.length);
        console.log(Date.now() - time);
    }

    console.log("AOC Day 11 p1 total:",currentLine.length);

}

main();
