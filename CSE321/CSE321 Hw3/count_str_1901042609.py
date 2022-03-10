def count_substrings(text, start, end):
    results = []
    for i in range(len(text)):
        indexes = []
        if text[i] == start and i + 1 < len(text):
            for j in range(i + 1, len(text)):
                if text[j] == end:
                    indexes.append(j)
            for index in indexes:
                results.append(text[i:(index + 1)])
    return len(results)


print(count_substrings("TXZXXJZWX", "X", "Z"))
