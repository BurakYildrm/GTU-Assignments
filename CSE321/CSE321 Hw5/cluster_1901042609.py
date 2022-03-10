def max_profit(array, n):
    total_max = array[0]
    curr_max = array[0]

    for i in range(1, n):
        curr_max = max(array[i], array[i] + curr_max)
        total_max = max(total_max, curr_max)

    return total_max


branches = [3, -5, 2, 11, -8, 9, -5]
print("maximum profit is", max_profit(branches, len(branches)))
