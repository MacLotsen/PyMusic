from collections import deque


def __round_the_table__():
    natural_major = deque([2, 2, 1, 2, 2, 2, 1])
    modi = []

    for i in range(7):
        natural_major.rotate(1)
        modi.append(list(natural_major))

    return reversed(modi)


ionian, dorian, phrygian, lydian, mixolydian, aeolian, locrian = __round_the_table__()
