import math


def min_cuts(n):
    if n <= 1:
        return 0
    else:
        cuts = min_cuts(math.ceil(n / 2))
        return cuts + 1


long = 100
print("minimum number of cuts needed for {} meter long wire:".format(long), min_cuts(long))
