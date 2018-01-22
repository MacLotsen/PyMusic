import math
from pymusic.modi import *
from pymusic.interval import *


def cents_of_interval(i, register=4, m=ionian):
    cents = 0
    for c in range(i-1):
        cents += m[c]
    return (21 + (12 * register)) + cents


def interval_of_cents(c, m = ionian):
    val = 0
    for i in range(7):
        if c == val:
            return i+1
        val += m[i]
    return -1


def herz_of_cents(cents, base=440.0):
    return base * math.pow(2, cents/12)


if __name__ == '__main__':
    assert(primus == 1)

    #TESTING def cents_of_interval(i, m):
    assert(cents_of_interval(secundo, ionian) == 2)
    assert(cents_of_interval(quartus, ionian) == 5)

    #TESTING def interval_of_cents(c, m):
    assert(interval_of_cents(cents_of_interval(primus, ionian), ionian) == primus)
    assert(interval_of_cents(cents_of_interval(secundo, ionian), ionian) == secundo)
    assert(interval_of_cents(cents_of_interval(septimo, ionian), ionian) == septimo)

    c4 = herz_of_cents(cents_of_interval(tertius, aeolian)-12)
    assert(round(c4, 2) == 261.63)
