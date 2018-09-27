from collections import deque

__natural_major__ = [2, 2, 1, 2, 2, 2, 1]


def __round_the_table__(steps):
    natural_major = deque(steps)
    modi = []

    for i in range(len(steps)):
        natural_major.rotate(1)
        modi.append(list(natural_major))

    return reversed(modi)
