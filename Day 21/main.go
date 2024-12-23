package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

var all_paths [][]rune

func main() {

	file, _ := os.Open("test.txt")
	defer file.Close()
	reader := bufio.NewScanner(file)
	var targets []string

	for reader.Scan() {
		line := reader.Text()
		targets = append(targets, line)
	}

	var numpad [][]rune = [][]rune{
		{'7', '8', '9'},
		{'4', '5', '6'},
		{'1', '2', '3'},
		{'n', '0', 'A'},
	}

	var dirpad [][]rune = [][]rune{
		{'n', '^', 'A'},
		{'<', 'v', '>'},
	}

	var total int = 0
	for _, target := range targets {
		x, y := get_start_pos(numpad, 'A')
		var robot_one_req []rune
		for _, char := range target {
			vis := set_false(4)
			all_paths = [][]rune{}
			get_shortest_path(numpad, x, y, vis, char, []rune{})
			validate_paths(numpad, char, x, y)
			temp := find_shortest(dirpad, x, y)
			robot_one_req = append(robot_one_req, temp...)
			robot_one_req = append(robot_one_req, 'A')
			x, y = get_start_pos(numpad, char)
		}

		robot_two_req := get_commands(dirpad, robot_one_req)
		robot_three_req := get_commands(dirpad, robot_two_req)
		num, _ := strconv.Atoi(string([]rune(target[:len(target)-1])))
		total += len(robot_three_req) * num

	}

	fmt.Printf("AOC Day 21 Part 1 Total: %d", total)

}

func validate_paths(pad [][]rune, target rune, start_x int, start_y int) {
	// Create a new slice to store valid paths
	var valid_paths [][]rune

	for _, array := range all_paths {
		if validate(pad, target, array, start_x, start_y) {
			valid_paths = append(valid_paths, array) // Keep valid paths
		}
	}

	// Replace all_paths with only the valid paths
	all_paths = valid_paths
}

func findMostConsecutive(grid [][]rune) []rune {
	var result []rune
	maxConsecutive := 0

	for _, arr := range grid {
		currentChar := rune(0)
		currentCount := 0
		maxCountForArr := 0

		for _, char := range arr {
			if char == currentChar {
				currentCount++
			} else {
				currentChar = char
				currentCount = 1
			}
			if currentCount > maxCountForArr {
				maxCountForArr = currentCount
			}
		}

		if maxCountForArr > maxConsecutive {
			maxConsecutive = maxCountForArr
			result = arr
		}
	}

	return result
}

func find_shortest(dirpad [][]rune, x int, y int) []rune {
	var shortest = 10000
	var selected = 0
	var one []int
	var check_array [][]rune

	for i, array := range all_paths {
		if shortest > len(array) {
			shortest = len(array)
			selected = i
		}
	}

	for i, array := range all_paths {
		if shortest == len(array) {
			one = append(one, i)
			check_array = append(check_array, array)
		}
	}

	var temp []rune
	if len(one) > 1 {
		temp = findMostConsecutive(check_array)
	} else {
		temp = all_paths[selected]
	}
	return temp
}

func get_commands(dirpad [][]rune, targets []rune) []rune {
	var result []rune

	x, y := get_start_pos(dirpad, 'A')

	for _, target := range targets {
		vis := set_false(3)
		all_paths = [][]rune{}
		get_shortest_path(dirpad, x, y, vis, target, []rune{})
		validate_paths(dirpad, target, x, y)
		temp := find_shortest(dirpad, x, y)
		result = append(result, temp...)
		result = append(result, 'A')
		x, y = get_start_pos(dirpad, target)
	}

	return result
}

func set_false(size int) [][]bool {
	visited := make([][]bool, size)
	for i := range visited {
		visited[i] = make([]bool, size)
	}
	return visited
}

func get_start_pos(grid [][]rune, target rune) (int, int) {
	for i, line := range grid {
		for d, char := range line {
			if char == target {
				return d, i
			}
		}
	}

	return 0, 0
}

func get_shortest_path(grid [][]rune, x int, y int, vis [][]bool, end rune, path []rune) {
	if grid[y][x] == end {
		// Create a copy of the path before appending
		pathCopy := make([]rune, len(path))
		copy(pathCopy, path)
		all_paths = append(all_paths, pathCopy)
		return
	}

	vis[y][x] = true
	directions := []struct {
		dx, dy int
		move   rune
	}{
		{-1, 0, '<'}, {1, 0, '>'}, {0, -1, '^'}, {0, 1, 'v'},
	}

	for _, dir := range directions {
		newX, newY := x+dir.dx, y+dir.dy
		if newY >= 0 && newY < len(grid) &&
			newX >= 0 && newX < len(grid[0]) &&
			grid[newY][newX] != 'n' && !vis[newY][newX] {

			new_path := append(path, dir.move)
			get_shortest_path(grid, newX, newY, vis, end, new_path)
		}
	}

	vis[y][x] = false
}

func validate(pad [][]rune, target rune, path []rune, start_x int, start_y int) bool {
	var x = start_x
	var y = start_y

	for _, char := range path {
		if char == '<' {
			x -= 1
		} else if char == '>' {
			x += 1
		} else if char == '^' {
			y -= 1
		} else if char == 'v' {
			y += 1
		}
	}

	// println(pad[y][x])
	if x >= 0 && y >= 0 && x < len(pad[0]) && y < len(pad) && pad[y][x] == target {
		return true
	}

	return false
}
