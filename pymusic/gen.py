import random


def gen(possibilities, r=random):
    z = r.randint(1, sum([w for w, v in possibilities]))-1
    for weight, val in possibilities:
        if z < weight:
            return val
        z -= weight


