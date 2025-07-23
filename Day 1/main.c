#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>



int convert_chars_to_int(char *table) {
    int temp = atoi(table);
    return temp;
}

void heap_sort(int *sorted, int *unsorted) {
    int index[1000];
    for (int i = 0; i < 1000; i++) {
        index[i] = 0;
    }

    int least = 1000000;
    int final_index = 0;
    for (int s = 0; s < 1000; s++) {
        least = 1000000;
        
        for (int i = 0; i < 1000; i++) {
            if (least > unsorted[i] && index[i] == 0) {
                least = unsorted[i];
                final_index = i;
            }
        }
        sorted[s] = least;
        index[final_index] = 1;
        least = 1000000;
    }

}

int compare(int one, int two) {
    if (one >= two) {
        return one-two;
    } else if (two > one) {
        return two-one;
    } 
}

uint64_t find_douplicates(int *array, int value_to_check) {
    int amount_of_douplicates = 0;
    for (int i = 0; i < 1000; i++) {
        if (array[i] == value_to_check) {
            amount_of_douplicates++;
        } 
        if (array[i] > value_to_check) {
            break;
        }
    }
    return amount_of_douplicates*value_to_check;
}


int main() {
    FILE *file = fopen("input.txt", "r");

    int unsorted_left_table[1000];
    int unsorted_right_table[1000];
    int sorted_left_table[1000];
    int sorted_right_table[1000];

    char charactor;
    int row = 0, column = 0, l_pos = 0, r_pos = 0;
    bool switch_table = false;
    char *left_number = malloc(5 * sizeof(char));
    char *right_number = malloc(sizeof(char) * 5);

    while ((charactor = fgetc(file)) != EOF) {
        if (charactor == ' ') {
            switch_table = true;

        } else if (charactor == '\n') {
            // left_number[l_pos] = '\0';
            // right_number[r_pos] = '\0';
            l_pos = 0;
            r_pos = 0;
            switch_table = false;
            unsorted_left_table[column] = convert_chars_to_int(left_number);
            unsorted_right_table[column++] = convert_chars_to_int(right_number);

        } else {
            if (switch_table) {
                right_number[r_pos++] = charactor;
            } else {
                left_number[l_pos++] = charactor;
            }

        }
    }

    // for (int i = 0; i < 1000; i++) {
    //     printf("Combined array: Left: %d, Right: %d\n", unsorted_left_table[i], unsorted_right_table[i]);
    // }


    heap_sort(sorted_left_table, unsorted_left_table);
    heap_sort(sorted_right_table, unsorted_right_table);

    // for (int i = 0; i < 1000; i++) {
    //     printf("Sorted left: %d, right: %d\n",sorted_left_table[i],sorted_right_table[i]);
    // }

    uint64_t total = 0;
    for (int i = 0; i < 1000; i++) {
        total += compare(sorted_left_table[i], sorted_right_table[i]);
    }

    printf("Total 1: %u\n",total);

    uint64_t douplicates = 0;
    for (int i = 0; i < 1000; i++) {
        douplicates += find_douplicates(sorted_right_table, sorted_left_table[i]);
    }
    

    printf("Total 2: %u\n",douplicates);
}