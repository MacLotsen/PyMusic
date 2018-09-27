import random
import sys
import time

import rtmidi

from pymusic.lang.duration import *
from pymusic.lang.interval import *
from pymusic.lang.scale.modern import *
from pymusic.midi import instrument
from pymusic.gen import gen
from pymusic.util import midinr_of_interval


def list_selector(l, msg="Enter index", value_getter=str, error_msg="Try again:"):
    for i in range(len(l)):
        print("#%i %s" % (i, value_getter(l[i])))
    print(msg)
    try:
        return l[int(input())]
    except ValueError:
        return list_selector(l, msg=error_msg, value_getter=value_getter, error_msg=error_msg)


generalIntervalPossibilities = [
    (5, Interval(-1)),
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
    (1, (r1_4, r1_4)),
    (1, (r1_4d, r1_8)),
    (18, (r1_16, r1_8, r1_16, r1_8, r1_8)),
    (12, (r1_32, r1_16d, r1_16, r1_16))
]

midiout = rtmidi.MidiOut()
available_ports = midiout.get_ports()

# dont forget: timidity -iA -B2,8 -Os &

try:

    if available_ports:
        for i in range(0, len(available_ports)):
            print("#%i = %s" % (i, available_ports[i]))
        i = input("Select port:")
        midiout.open_port(int(i))
    else:
        midiout.open_virtual_port("PyMusic virtual output")

    chosen_iset = list_selector(instrument.instrument_sets,
                                msg="Enter index for timbre")

    chosen_instrument = list_selector(instrument.instruments[chosen_iset[1]:chosen_iset[2]],
                                      msg="Enter index for instrument")

    # Timidity on this machine (see m.devices())
    #midiout.select(2)

    midiout.send_message([0xC0, instrument.instruments.index(chosen_instrument)])
    # m.send([0xC1, 0x00])
    # m.send([0xC2, 69])

    offset = 0
    register = 0
    dev = 0x90


    bpm = 60.0

    i = 0
    r = None

    rnd = random.Random(time.time())

    while True:
        randInterval = gen(generalIntervalPossibilities, rnd)
        if r is None:
            r = gen(rhythm_figures, rnd)

        if randInterval.index == -1:
            time.sleep(float(r[i]) * (240.0 / bpm))
            continue

        n = midinr_of_interval(randInterval, lydian, register) + (0xC * 3)
        midiout.send_message([dev, n, 0x40])
        time.sleep(float(r[i]) * (240.0 / bpm))
        midiout.send_message([dev, n, 0x00])
        i += 1
        if i >= len(r):
            i = 0
            r = None

except KeyboardInterrupt:
    print("Exit")
    sys.exit(0)
