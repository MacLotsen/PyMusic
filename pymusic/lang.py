from collections import deque

from pymusic.util import herz_of_cents

# ==================================================================================================================== #
#                                                 INTERVAL                                                             #
# ==================================================================================================================== #

primus, secundo, tertius, quartus, quinto, sexta, septimo, octava = range(1, 9)

# ==================================================================================================================== #
#                                                   TONE                                                               #
# ==================================================================================================================== #


C = herz_of_cents(-9)
CIS = DES = herz_of_cents(-8)
D = herz_of_cents(-7)
DIS = ES = herz_of_cents(-6)
E = herz_of_cents(-5)
EIS = F = herz_of_cents(-4)
FIS = GES = herz_of_cents(-3)
G = herz_of_cents(-2)
GIS = AS = herz_of_cents(-1)
A = 440.0
AIS = BES = herz_of_cents(1)
B = CES = herz_of_cents(2)


# ==================================================================================================================== #
#                                                  RHYTHM                                                              #
# ==================================================================================================================== #


def __down_hill__(minimum=1 / 32, maximum=8):
    def _down(lst, weight):
        half_weight = weight / 2.0
        lst.append(weight + half_weight)
        lst.append(weight)
        if half_weight >= minimum:
            _down(lst, half_weight)

    sets = []
    _down(sets, maximum)
    return sets


r8d, r8, r4d, r4, \
r2d, r2, r1d, r1, \
r1_2d, r1_2, r1_4d, r1_4, \
r1_8d, r1_8, r1_16d, r1_16, \
r1_32d, r1_32 = __down_hill__()

figures = [
    (r1_4, r1_4),
    (r1_4d, r1_8),
    (r1_16, r1_8, r1_16, r1_8, r1_8)
]

# ==================================================================================================================== #
#                                                   MODI                                                               #
# ==================================================================================================================== #


def __round_the_table__(steps):
    natural_major = deque(steps)
    modi = []

    for i in range(len(steps)):
        natural_major.rotate(1)
        modi.append(list(natural_major))

    return reversed(modi)


ionian, dorian, phrygian, lydian, mixolydian, aeolian, locrian = __round_the_table__([2, 2, 1, 2, 2, 2, 1])
