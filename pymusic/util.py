import math

from pymusic.lang.interval import Interval

__MIDINR_OFFSET__ = 21


def midinr_of_interval(i: Interval, m: list, register=4):
    n = __MIDINR_OFFSET__
    for i in range(i.index-1):
        n += m[i]
    return (12 * register) + n


def interval_of_mininr(n: int, m: list):
    normalized = (n - __MIDINR_OFFSET__) % 12
    i = val = 0
    while val < normalized:
        val += m[i]
        i += 1
    return i + 1


def hertz_of_midinr(midinr: int, base=440.0):
    return base * math.pow(2, (midinr - 69) / 12)

