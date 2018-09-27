import unittest


class TestCaseScales(unittest.TestCase):

    def _scale_eq(self, s1, s2):
        self.assertEqual(len(s1), len(s2), "Length must be the same")
        for index in range(0, len(s1)):
            self.assertEqual(s1[index], s2[index], "Every step must be the same")

    def test_modern(self):
        from pymusic.lang.scale import modern
        self._scale_eq([2, 2, 1, 2, 2, 2, 1], modern.ionian)

    def test_chromatic(self):
        from pymusic.lang.scale.chromatic import chromatic
        self.assertEqual(len(chromatic), 12, "Must be 12 steps")
        for w in chromatic:
            self.assertEqual(1, w, "Every step must be one cent")


if __name__ == '__main__':
    unittest.main()
