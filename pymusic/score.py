

class IncompleteScoreException(Exception):
    pass


class Track:
    instrument = None
    part = None
    tags = None
    text = None
    sentences = None
    sequences = None
    words = None
    syllables = None

    def __init__(self, instrument, part, name="Unknown", text="", tags=None):
        self.instrument = instrument
        self.part = part
        self.name = name
        self.text = text
        self.tags = tags or []


class Sentence:
    def __init__(self):
        self.text = ""
        self.sequences = []



class Sequence:
    def __init__(self):
        self.text = ""
        self.words = []


class Word:
    def __init__(self):
        self.text = ""
        self.syllables = []


class Syllable:
    def __init__(self):
        self.text = ""
        self.notes = []


class Note:
    def __init__(self, time_in_score, midi_note):
        self.time_in_score = time_in_score
        self.midi_note = midi_note
        self.duration = 0


class Rest(Note):
    def __init__(self, time_in_score):
        super().__init__(time_in_score, -1)


class Score:
    title = "Unnamed Score"
    tags = []
    bpm = 120
    tracks = []
    midi_score = None
    text = None

    def __init__(self, **kwargs):
        if 'tags' not in kwargs.keys() and 'tracks' not in kwargs.keys():
            raise IncompleteScoreException("Either give tags for score generation or tracks to play.")
        for key, val in kwargs.items():
            if hasattr(self, key):
                setattr(self, key, val)
