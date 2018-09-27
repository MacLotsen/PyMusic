import unittest

from pymusic.util import __MIDINR_OFFSET__, midinr_of_interval, interval_of_mininr, hertz_of_midinr
from pymusic.lang.interval import *
from pymusic.lang.scale.modern import *


# https://newt.phys.unsw.edu.au/jw/notes.html
class TestCaseUtilities(unittest.TestCase):
    def test_prime(self):
        self.assertTrue(primus.index == 1)

    def test_cents_of_interval(self):
        # TESTING def cents_of_interval(i, m):
        self.assertEquals(midinr_of_interval(secundo, ionian, 0), 2 + __MIDINR_OFFSET__)
        self.assertEquals(midinr_of_interval(quartus, ionian, 0), 5 + __MIDINR_OFFSET__)

    def test_interval_of_cents(self):
        # TESTING def interval_of_cents(c, m):
        self.assertEquals(interval_of_mininr(midinr_of_interval(primus, ionian), ionian), primus.index)
        self.assertEquals(interval_of_mininr(midinr_of_interval(secundo, ionian), ionian), secundo.index)
        self.assertEquals(interval_of_mininr(midinr_of_interval(septimo, ionian), ionian), septimo.index)

    def test_herz_of_cents(self):
        cents = midinr_of_interval(tertius, aeolian)
        self.assertEqual(cents, 72)
        c5 = hertz_of_midinr(cents)
        self.assertEqual(round(c5, 2), 523.25)


if __name__ == '__main__':
    unittest.main()
