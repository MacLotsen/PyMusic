import unittest

from pymusic.midi.parser import MidiParser


class TestCaseParser(unittest.TestCase):
    def test_something(self):
        p = MidiParser('../res/palestrina_missa_papae_marcelli_kyrie.mid')
        score = p.parse()

        times = 0
        for i, track in enumerate(score.tracks[1:-1]):
            pass

        self.assertEqual(True, True)


if __name__ == '__main__':
    unittest.main()
