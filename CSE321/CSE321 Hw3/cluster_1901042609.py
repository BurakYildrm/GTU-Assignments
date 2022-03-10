import math


def find_max_profit(cluster, low, high):
    if low == high:
        return cluster[low]
    middle = math.floor((high + low) / 2)
    right_s = find_max_profit(cluster, middle + 1, high)
    left_s = find_max_profit(cluster, low, middle)
    temp = 0
    temp_left = -99999999999
    for i in range(middle, low - 1, -1):
        temp += cluster[i]
        if temp > temp_left:
            temp_left = temp
    temp = 0
    temp_right = -99999999999
    for i in range(middle + 1, high + 1):
        temp += cluster[i]
        if temp > temp_right:
            temp_right = temp
    middle_s = max(temp_right + temp_left, temp_right, temp_left)
    return max(middle_s, left_s, right_s)


print(find_max_profit([2, 11, -8, 9], 0, 3))
