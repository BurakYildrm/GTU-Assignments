def solve_polynomial(coefficients, x):
    power = len(coefficients) - 1
    result = 0
    for coefficient in coefficients:
        result += coefficient*pow(x, power)
        power -= 1
    return result


array = [1, 2, 3]

print(solve_polynomial(array, 5))
