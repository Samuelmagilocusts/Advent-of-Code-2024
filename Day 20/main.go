package main

import (
	"bufio"
	"fmt"
	"os"
	"time"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

var results = make(map[int]int)

type Pair struct {
	First  int
	Second int
}

func main() {
	file, err := os.Open("input.txt")
	check(err)
	defer file.Close()

	reader := bufio.NewScanner(file)

	var grid [][]rune

	for reader.Scan() {
		line := reader.Text()
		var newLine []rune

		for _, char := range line {
			newLine = append(newLine, char)
		}

		grid = append(grid, newLine)
	}

	if err := reader.Err(); err != nil {
		panic(err)
	}

	var start_x int
	var start_y int
	for i, row := range grid {
		for e, _ := range row {
			if grid[i][e] == 'S' {
				start_x = e
				start_y = i
			}
		}
	}

	startTime := time.Now()

	visited := make([][]bool, len(grid))
	set_false(visited, len(grid))
	recurse(grid, start_x, start_y, visited, 0)
	var og_total int = 0
	for key, _ := range results {
		og_total = key
		break
	}

	for i := range grid {
		for f := range grid {
			if grid[i][f] == '#' {
				if i > 0 && i < len(grid)-1 && grid[i-1][f] != '#' && grid[i+1][f] != '#' {
					set_false(visited, len(grid))
					grid[i][f] = '.'
					recurse(grid, start_x, start_y, visited, 0)
					grid[i][f] = '#'
				}
				if f > 0 && f < len(grid)-1 && grid[i][f-1] != '#' && grid[i][f+1] != '#' {
					set_false(visited, len(grid))
					grid[i][f] = '.'
					recurse(grid, start_x, start_y, visited, 0)
					grid[i][f] = '#'
				}
			}
		}
	}

	var total int = 0
	for key, value := range results {
		if og_total-key >= 100 {
			total += value
		}
	}

	fmt.Printf("AOC Day 20 Part 1 Total: %d\n", total)
	duration := time.Since(startTime)

	fmt.Printf("Time taken: %.3f ms\n", duration.Seconds()*1000)
}

func make_grid(grid [][]rune, x int, y int, visited [][]bool, path []Pair) {

}

func set_false(visited [][]bool, size int) {
	for i := range visited {
		visited[i] = make([]bool, size)
	}
}

func recurse(grid [][]rune, x int, y int, visited [][]bool, total int) {

	if grid[y][x] == 'E' {
		results[total] += 1
		return
	}

	visited[y][x] = true

	if x > 0 && !visited[y][x-1] && grid[y][x-1] != '#' {
		recurse(grid, x-1, y, visited, total+1)
	}

	if x < len(grid)-1 && !visited[y][x+1] && grid[y][x+1] != '#' {
		recurse(grid, x+1, y, visited, total+1)
	}

	if y > 0 && !visited[y-1][x] && grid[y-1][x] != '#' {
		recurse(grid, x, y-1, visited, total+1)
	}

	if y < len(grid)-1 && !visited[y+1][x] && grid[y+1][x] != '#' {
		recurse(grid, x, y+1, visited, total+1)
	}

	visited[y][x] = false

}

func printGrid(grid [][]rune) {
	for _, row := range grid {
		fmt.Println(string(row))
	}
}
