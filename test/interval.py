import unittest

from pymusic.lang.interval import Interval
from pymusic.lang.scale import modern
from pymusic.lang.scale import chromatic, harmonic


class TestCaseIntervalMarks(unittest.TestCase):
    def test_ionian_perfects(self):
        I = Interval(index=1, scale=modern.ionian)
        IV = Interval(index=4, scale=modern.ionian)
        V = Interval(index=5, scale=modern.ionian)
        VIII = Interval(index=8, scale=modern.ionian)

        self.assertTrue(I.is_perfect())
        self.assertTrue(IV.is_perfect())
        self.assertTrue(V.is_perfect())
        self.assertTrue(VIII.is_perfect())

    def test_lydian_aug_fourth(self):
        IV = Interval(index=4, scale=modern.lydian)

        self.assertTrue(IV.is_augmented(), "Lydian fourth must be augmented")

    def test_harmonic_aug_third_on_sixth(self):
        from pymusic.lang.scale import __round_the_table__
        offset_six = list(__round_the_table__(harmonic.harmonic))[5]
        II = Interval(index=2, scale=offset_six)

        self.assertTrue(II.is_augmented(), "From six to seven in harmonic must be augmented")

    def test_locrian_dim_fifth(self):
        V = Interval(index=5, scale=modern.locrian)

        self.assertTrue(V.is_diminished(), "Locrian fifth must be diminished")

    def test_chromatic_dim_third(self):
        III = Interval(index=3, scale=chromatic.chromatic)

        self.assertTrue(III.is_diminished(), "Chromatic third must be diminished")


if __name__ == '__main__':
    unittest.main()
