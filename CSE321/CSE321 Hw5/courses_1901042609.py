def max_courses(start, finish):
    n = len(start)
    counter = 0
    last_course = 0
    counter += 1

    for i in range(n):
        if start[i] >= finish[last_course]:
            last_course = i
            counter += 1

    return counter


s = [1, 3, 0, 5, 8, 5]
f = [2, 4, 6, 7, 9, 9]
print("maximum number of courses a student can attend is", max_courses(s, f))
