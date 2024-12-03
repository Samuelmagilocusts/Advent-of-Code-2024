

def pull_one_out(array, splitter):
    return array[:splitter] + array[splitter+1:]


filelines = []
grid = []
with open("Day 2/test.txt", "r") as file:
    filelines = file.readlines()

for line in filelines:
    # print(line)
    grid.append(line)

total = 0

numbers_grid = []

for line in grid:
    # print(line)
    new_line = line.strip('\n').split(' ')
    numbers_grid.append(new_line)


assending_lines = []
decending_lines = []
problem_lines_a = []
problem_lines_d = []
problem_lines = []
for number_array in numbers_grid:
    checkmark = True
    direction = []

    for i in range(len(number_array)):   
        if i < len(number_array)-1:
            if (int(number_array[i]) > int(number_array[i+1])):
                direction.append("decending")

            elif (int(number_array[i]) < int(number_array[i+1])):
                direction.append("assending")

            elif (int(number_array[i]) == int(number_array[i+1])):
                direction.append("same")

    if direction.count("decending") == len(number_array)-1:
        decending_lines.append(number_array)
    elif direction.count("assending") == len(number_array)-1:
        assending_lines.append(number_array)
    

    elif direction.count("decending") == len(number_array)-2:
        problem_lines_d.append(number_array)
    elif direction.count("assending") == len(number_array)-2:
        problem_lines_a.append(number_array)
    else:
        problem_lines.append(number_array)

for number_array in assending_lines:
    checkmark = True
    for i in range(len(number_array)):   
        if i < len(number_array)-1:
            if (int(number_array[i+1]) - int(number_array[i]) > 3 or int(number_array[i+1]) - int(number_array[i]) < 1):
                checkmark = False
                break

    if checkmark:
        total +=1

for number_array in decending_lines:
    checkmark = True
    for i in range(len(number_array)):   
        if i < len(number_array)-1:
            if (int(number_array[i]) - int(number_array[i+1]) > 3 or int(number_array[i]) - int(number_array[i+1]) < 1):
                checkmark = False
                break

    if checkmark is True:
        total +=1

total_p2 = 0
for number_array_full in problem_lines_d:
    for d in range(len(number_array_full)):
        checkmark = True
        number_array = pull_one_out(number_array_full, d)

        for i in range(len(number_array)):   
            if i < len(number_array)-1:
                dif = int(number_array[i]) - int(number_array[i+1])
                if dif > 3 or dif < 1:
                    checkmark = False
                    break

        if checkmark is True:
            total_p2 +=1
            break

for number_array_full in problem_lines_a:
    for d in range(len(number_array_full)):
        checkmark = True
        number_array = pull_one_out(number_array_full, d)
        for i in range(len(number_array)):   
            if i < len(number_array)-1:
                dif = int(number_array[i+1]) - int(number_array[i])
                if dif > 3 or dif < 1:
                    checkmark = False
                    break

        if checkmark is True:
            total_p2 +=1
            break

# for number_array_full in problem_lines:
    
#     for d in range(len(number_array_full)):
#         checkmark = True
#         number_array = pull_one_out(number_array_full, d)
#         for i in range(len(number_array)):   
#             if i < len(number_array)-1:
                
#                 difa = int(number_array[i+1]) - int(number_array[i])
#                 difd = int(number_array[i]) - int(number_array[i+1])
#                 if difa > 3 or difa < 1:
#                     checkmark = False
#                     break

#         if checkmark is True:
#             total_p2 +=1
#             break

print("total p1",total, "total p2", total+total_p2)


