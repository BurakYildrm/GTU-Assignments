results = [76, 8, 66, 73, 30, 49, 67, 62, 81, 3]


def best(x, y, array):
    if y - x <= 1:
        return max(array[x], array[y])
    else:
        best1 = best(x, (x + y) // 2, array)
        best2 = best((x + y) // 2 + 1, y, array)
        return max(best1, best2)


def worst(x, y, array):
    if y - x <= 1:
        return min(array[x], array[y])
    else:
        worst1 = worst(x, (x + y) // 2, array)
        worst2 = worst((x + y) // 2 + 1, y, array)
        return min(worst1, worst2)


print("best: {}, worst: {}".format(best(0, 9, results), worst(0, 9, results)))
