def brute_force_exp(base, power):
    res = 1
    for i in range(power):
        res *= base
    return res


def divide_and_conquer_exp(base, power):
    if power <= 1:
        return base
    else:
        low = divide_and_conquer_exp(base, power // 2)
        high = divide_and_conquer_exp(base, power - power // 2)
        return low * high


print("2^5 (brute force): \t\t\t", brute_force_exp(5, 3))
print("2^5 (divide and conquer): \t", divide_and_conquer_exp(5, 3))
