import unittest

from pymusic.score import Score, Track, IncompleteScoreException
from pymusic.midi.instrument import instruments


class ScoreRequirements(unittest.TestCase):

    def test_tags_or_tracks(self):
        def incompletes(*args):
            Score()
            Score(title="MyScore")
            Score(title="MyScore", bpm=200)

        self.assertRaises(IncompleteScoreException, incompletes, "Tags or tracks must be present.")

    def test_tags(self):
        try:
            self.assertIsNotNone(Score(tags=['modern', 'c#', '2-5-1']), "Tags are sufficient for a score to generate.")
        except IncompleteScoreException as e:
            self.fail("The score had tags, but it says IncompleScoreException")

    def test_tracks(self):
        my_track = Track(instruments[0], [])
        self.assertEqual(type(my_track.tags), list, "When not giving tags, it must be type of list.")


if __name__ == '__main__':
    unittest.main()
