package main

// part 1 works runs in about 5 seconds. part 2 not so much... didnt even finish building part 2

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"time"
)

var all_paths [][]rune

var best_routes map[[]rune][]rune

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
	for u, target := range targets {
		var starting []rune
		var robot [][]rune
		var subtotal = 0

		for _, char := range target {
			starting = append(starting, char)
		}

		var temp_robot = get_commands(numpad, starting)

		for _, array := range temp_robot {
			if len(robot) == 0 {
				robot = array
			} else {
				robot = die(robot, array)
			}
		}

		for i := range 3 - 1 {
			start := time.Now()
			source_robot := make([][]rune, len(robot))
			for i := range robot {
				source_robot[i] = make([]rune, len(robot[i]))
				copy(source_robot[i], robot[i])
			}
			robot = [][]rune{}
			for _, r := range source_robot {
				temp_robot = get_commands(dirpad, r)
				var temp_new_robot [][]rune
				for _, array := range temp_robot {
					if len(temp_new_robot) == 0 {
						temp_new_robot = array
					} else {
						temp_new_robot = die(temp_new_robot, array)
					}
				}
				robot = append(robot, remove_douplicates(temp_new_robot)...)
			}

			robot = remove_douplicates(robot)
			robot = find_shortest(robot)
			elapsed := time.Since(start)
			fmt.Printf("%d, %d Time Elapsed: %d ms\n", u, i+1, elapsed.Milliseconds())
		}

		for _, r := range robot {
			if subtotal == 0 || len(r) < subtotal {
				subtotal = len(r)
			}
		}

		num, _ := strconv.Atoi(string([]rune(target[:len(target)-1])))
		total += subtotal * num
	}

	fmt.Printf("AOC Day 21 Part 1 Total: %d", total)
}

func remove_douplicates(array [][]rune) [][]rune {
	vis := make(map[string]bool)
	result := [][]rune{}
	for _, line := range array {
		str := string(line)

		if !vis[str] {
			vis[str] = true
			result = append(result, line)
		}
	}
	return result
}

func die(one [][]rune, two [][]rune) [][]rune {
	var new_2d_array [][]rune
	for _, seg_one := range one {
		for _, seg_two := range two {
			new_1d_array := make([]rune, len(seg_one))
			copy(new_1d_array, seg_one)
			new_1d_array = append(new_1d_array, seg_two...)
			new_2d_array = append(new_2d_array, new_1d_array)
		}
	}

	return find_shortest(new_2d_array)
}

func find_shortest(big_array [][]rune) [][]rune {
	var shortest = 0
	var one []int
	var check_array [][]rune

	for _, array := range big_array {
		if shortest == 0 || shortest > len(array) {
			shortest = len(array)
		}
	}

	for i, array := range big_array {
		if shortest == len(array) {
			one = append(one, i)
			check_array = append(check_array, array)
		}
	}

	return check_array
}

func get_commands(pad [][]rune, targets []rune) [][][]rune {
	var capture [][][]rune

	x, y := get_start_pos(pad, 'A')

	for _, target := range targets {
		vis := set_false(4)
		all_paths = [][]rune{}
		get_shortest_path(pad, x, y, vis, target, []rune{})
		validate_paths(pad, target, x, y)

		var choices = find_shortest(all_paths)
		var big_array [][]rune
		for _, choice := range choices {
			temp := make([]rune, len(choice))
			copy(temp, choice)
			temp = append(temp, 'A')
			big_array = append(big_array, temp)
		}
		x, y = get_start_pos(pad, target)
		capture = append(capture, big_array)
	}

	return capture
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

func validate_paths(pad [][]rune, target rune, start_x int, start_y int) {
	var valid_paths [][]rune

	for _, array := range all_paths {
		if validate(pad, target, array, start_x, start_y) {
			valid_paths = append(valid_paths, array)
		}
	}

	all_paths = valid_paths
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

	if x >= 0 && y >= 0 && x < len(pad[0]) && y < len(pad) && pad[y][x] == target {
		return true
	}

	return false
}
