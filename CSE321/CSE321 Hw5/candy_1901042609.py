def cut(prices, n):
    vals = [0 for x in range(n + 1)]

    for i in range(1, n + 1):
        max_val = -99999
        for j in range(i):
            max_val = max(max_val, prices[j] + vals[i - j - 1])
        vals[i] = max_val

    return vals[n]


array = [1, 5, 8, 9, 10, 17, 17, 20]
print("maximum obtainable value in {} is".format(array), cut(array, len(array)))
