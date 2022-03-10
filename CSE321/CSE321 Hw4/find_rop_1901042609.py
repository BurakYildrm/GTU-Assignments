def combine_results(sequence, mid, low, high):
    counter = 0
    i = low
    j = mid + 1
    k = 0
    ordered = [0 for _ in range(high - low + 1)]

    while i < mid + 1 and j < high + 1:
        if sequence[i] < sequence[j]:
            ordered[k] = sequence[i]
            i += 1
            k += 1
        else:
            ordered[k] = sequence[j]
            j += 1
            k += 1
            counter += mid - i + 1

    while i < mid + 1:
        ordered[k] = sequence[i]
        i += 1
        k += 1

    while j < high + 1:
        ordered[k] = sequence[j]
        j += 1
        k += 1

    k = 0
    for index in range(low, high + 1):
        sequence[index] = ordered[k]
        k += 1

    return counter


def find_rop(sequence, low, high):
    if high <= low:
        return 0
    else:
        mid = (low + high) // 2
        count = find_rop(sequence, low, mid)
        count += find_rop(sequence, mid + 1, high)
        count += combine_results(sequence, mid, low, high)
        return count


array = [76, 8, 66, 73, 30, 49, 67, 62, 81, 3]
print("number of reversed ordered pairs in {} is".format(array), find_rop(array, 0, len(array) - 1))
