class Cheese:
    def __init__(self, weight, price):
        self.weight = weight
        self.price = price
        self.cost = price // weight

    def __lt__(self, other):
        return self.cost < other.cost


def max_price(wt, prc, cap):
    chs_vals = []

    for i in range(len(wt)):
        chs_vals.append(Cheese(wt[i], prc[i]))

    chs_vals.sort(reverse=True)
    total = 0

    for i in chs_vals:
        cur_wt = int(i.weight)
        cur_prc = int(i.price)
        if cap - cur_wt >= 0:
            cap -= cur_wt
            total += cur_prc
        else:
            frac = cap / cur_wt
            total += cur_prc * frac
            break

    return total


weights = [10, 40, 20, 30, 5]
prices = [30, 70, 100, 120, 80]
capacity = 50

max_val = max_price(weights, prices, capacity)
print("Maximum price is", max_val)
