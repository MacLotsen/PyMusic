import random
import time
from pymusic import rhythm
from pymusic.util import cents_of_interval
from pymusic.interval import *
from pymusic.modi import aeolian, locrian, lydian
from pymusic.provider import provide

import pymidi

generalIntervalPossibilities = [
    (3, primus),
    (9, secundo),
    (9, tertius),
    (5, quartus),
    (7, quinto),
    (2, sexta),
    (1, septimo),
    (3, octava)
]

rhythm_figures = [
    (1, rhythm.figures[0]),
    (1, rhythm.figures[1]),
    (18, rhythm.figures[2])
]

m = pymidi.MidiOut()
m.select(1)

m.send([0xC0, 0x41])
m.send([0xC1, 0x00])
m.send([0xC2, 69])

offset = 0
register = 0
dev = 0x90


bpm = 60.0

i = 0
r = None


try:
    while True:
        rnd = random.Random(time.time())
        n = cents_of_interval(provide(generalIntervalPossibilities, rnd), register, lydian) + (0xC * 3)
        if r is None:
            r = provide(rhythm_figures, rnd)
        m.send([dev, n, 0x40])
        time.sleep(float(r[i]) * (240.0 / bpm))
        m.send([dev, n, 0x00])
        i += 1
        if i > 1:
            i = 0
            r = None
except KeyboardInterrupt:
    print("Exit")
    sys.exit(0)

# m.send([0x90, cents_of_interval(primus, locrian)+ (0xC * 2), 0x40])
# time.sleep(1)
# m.send([0x90, cents_of_interval(primus, locrian)+ (0xC * 2), 0x00])
