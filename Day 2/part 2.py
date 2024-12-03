    
def trace_array_journey(array, last_result = '', result = 0, path = []):
    if len(array) <= 1:
        return result, path 
    value1 = array[0]
    value2 = array[1]
    if value1 < value2:
        if last_result == 'a':
            return trace_array_journey(array[1:], 'a', result, path + ['a'])
        elif last_result == '':
            return trace_array_journey(array[1:], 'a', result, path + ['a'])
        else:
            return trace_array_journey(array[1:], 'a', result+1, path + ['a'])
    elif value1 > value2:
        if last_result == 'd':
            return trace_array_journey(array[1:], 'd', result, path + ['d'])
        elif last_result == '':
            return trace_array_journey(array[1:], 'd', result, path + ['d'])
        else:
            return trace_array_journey(array[1:], 'd', result+1, path + ['d'])
    elif value1 == value2:
        if last_result == 's':
            return trace_array_journey(array[1:], 's', result, path + ['s'])
        elif last_result == '':
            return trace_array_journey(array[1:], 's', result, path + ['s'])
        else:
            return trace_array_journey(array[1:], 's', result+1, path + ['s'])
        
def most_frequent(List):
    mf = max(set(List), key=List.count)
    return mf

def compliable_report(array, direction):
    if len(array) <= 1:
        return True
    else:
        if direction == 'a':
            dif = array[1] - array[0]
            if dif >= 1 and dif <= 3:
                return compliable_report(array[1:], direction)
            else:
                return False
        elif direction == 'd':
            dif = array[0] - array[1]
            if dif >= 1 and dif <= 3:
                return compliable_report(array[1:], direction)
            else:
                return False
        else:
            return False


def is_report_salvagable(array):
    strikes, journey = trace_array_journey(array)

    if strikes > 2:
        return False, ''
    else:
        return True, most_frequent(journey)
    

def try_every_config(Original_Array, direction, array_atempt = [], i = 0):
    if len(array_atempt) > 0:
        if compliable_report(array_atempt, direction) is True:
            return True
    
    if i == len(Original_Array) - 1:
        return try_every_config(Original_Array, direction, Original_Array[:i], i + 1)
    
    
    if i == len(Original_Array):
        return False

    return try_every_config(Original_Array, direction, Original_Array[:i]+Original_Array[i+1:], i + 1)
    

chack = []

def main():
    filearray = []
    data = []
    total = 0
    with open("Day 2/input.txt") as file:
        filearray = file.readlines()

    for line in filearray:
        str_array = line.strip('\n').split(' ')
        int_array = []
        for string in str_array:
            int_array.append(int(string))
        data.append(int_array)


    for report in data:
        useable, direction = is_report_salvagable(report)
        if useable is True:
            if compliable_report(report, direction) is True:
                total += 1
            else:
                if try_every_config(report, direction) is True:
                    total += 1
                else:
                    chack.append(report) 
                pass
        else:
            # chack.append(report)
            pass
        

    print("Day 2 p2 Total:",total)

    # for report, pos in enumerate(chack):
    #     print(pos, "report", report)


main()
