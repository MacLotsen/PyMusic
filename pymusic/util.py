import math


__CENT_OFFSET__ = 21


def cents_of_interval(i, m, register=4):
    cents = 0
    for c in range(i-1):
        cents += m[c]
    return (__CENT_OFFSET__ + (12 * register)) + cents


def interval_of_cents(c, m):
    normalized_cents = (c - __CENT_OFFSET__) % 12
    i = val = 0
    while val < normalized_cents:
        val += m[i]
        i += 1
    return i + 1


def herz_of_cents(cents, base=440.0):
    return base * math.pow(2, cents/12)


if __name__ == '__main__':
    from pymusic.lang import *

    assert(primus == 1)

    # TESTING def cents_of_interval(i, m):
    assert(cents_of_interval(secundo, ionian, 0) == 2 + __CENT_OFFSET__)
    assert(cents_of_interval(quartus, ionian, 0) == 5 + __CENT_OFFSET__)

    # TESTING def interval_of_cents(c, m):
    assert(interval_of_cents(cents_of_interval(primus, ionian), ionian) == primus)
    assert(interval_of_cents(cents_of_interval(secundo, ionian), ionian) == secundo)
    assert(interval_of_cents(cents_of_interval(septimo, ionian), ionian) == septimo)

    c4 = herz_of_cents(cents_of_interval(tertius, aeolian))
    assert(round(c4, 2) == 261.63)
