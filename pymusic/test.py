import sys, random, time

import pymidi
from pymusic.lang import *
from pymusic.provider import provide
from pymusic.cli import list_selector
from pymusic.util import cents_of_interval

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
    (1, figures[0]),
    (1, figures[1]),
    (18, figures[2])
]

m = pymidi.MidiOut()


chosen_timbre = list_selector(pymidi.timbres(),
                              msg="Enter index for timbre")


def instrument_name(instr):
    return instr[1]


chosen_instrument = list_selector(pymidi.instruments(chosen_timbre),
                                  msg="Enter index for instrument",
                                  value_getter=instrument_name)

# Timidity on this machine (see m.devices())
m.select(1)

m.send([0xC0, chosen_instrument[0]])
# m.send([0xC1, 0x00])
# m.send([0xC2, 69])

offset = 0
register = 0
dev = 0x90


bpm = 60.0

i = 0
r = None


try:
    while True:
        rnd = random.Random(time.time())
        n = cents_of_interval(provide(generalIntervalPossibilities, rnd), lydian, register) + (0xC * 3)
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
