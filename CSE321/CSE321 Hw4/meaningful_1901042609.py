def kth_smallest(array, k):
    low = min(array)
    high = max(array)

    while low <= high:
        mid = low + (high - low) // 2
        less = 0
        equal = 0

        for i in range(len(array)):
            if array[i] < mid:
                less += 1

            elif array[i] == mid:
                equal += 1

        if less < k <= less + equal:
            return mid

        elif less >= k:
            high = mid - 1

        elif less < k and less + equal < k:
            low = mid + 1


results = [76, 8, 66, 73, 30, 49, 67, 62, 81, 3]
kth = 3
print("{}{} smallest element in {} is"
      .format(kth, "st" if kth % 10 == 1 else "nd" if kth % 10 == 2 else "rd" if kth % 10 == 3 else "th", results),
      kth_smallest(results, kth))
