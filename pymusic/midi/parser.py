from mido import MidiFile, Message, MetaMessage

from pymusic.score import Sentence, Sequence, Word, Syllable, Track, Note, Score, Rest


class MidiParser:
    score = None

    def __init__(self, filename):
        self.score = MidiFile(filename)

    def parse(self):
        tracks = []
        for i, track in enumerate(self.score.tracks[1:-1]):
            instrument = 0
            sentences = []
            sequences = []
            words = []
            syllables = []

            sentence = Sentence()
            sequence = Sequence()
            word = Word()
            syllable = Syllable()
            messages = []
            txt = ""

            # startpoint & duration too
            absolute_time = 0
            note_state_buf = {}

            # print("Track %i: %s" % (i, track.name))
            for msg in track:
                absolute_time += msg.time
                if type(msg) == MetaMessage:
                    if msg.time > 0:
                        messages.append(msg)

                        if len(note_state_buf) > 0:
                            for k in note_state_buf.keys():
                                the_note = note_state_buf[k]
                                the_note.duration = msg.time
                                syllable.notes.append(the_note)
                            note_state_buf.clear()
                        else:
                            r = Rest(absolute_time)
                            r.duration = msg.time
                            syl = Syllable()
                            syl.notes.append(r)
                            syllables.append(syl)
                            w = Word()
                            w.syllables.append(syl)
                            words.append(w)
                            seq = Sequence()
                            seq.words.append(w)
                            sequences.append(seq)
                            sentences.append(seq)
                    if msg.type == 'lyrics':
                        if syllable.text != '':
                            syllables.append(syllable)
                            syllable = Syllable()

                        raw_txt = msg.text.replace('_', '')
                        word_complete = True
                        sequence_complete = False
                        sentence_complete = False
                        if raw_txt[-1] == '-':
                            word_complete = False
                            word.text = word.text + raw_txt.replace('-', '')
                        elif '.' in raw_txt:
                            word.text = word.text + raw_txt.replace('.', '')
                            sentence_complete = True
                        elif raw_txt[-1] == ',':
                            word.text = word.text + raw_txt.replace(',', '')
                            sequence_complete = True
                        else:
                            word.text = word.text + raw_txt

                        syllable.text = raw_txt

                        word.syllables.append(syllable)
                        if word_complete:
                            sequence.words.append(word)
                            words.append(word)
                            word = Word()

                        if sequence_complete:
                            sequence.text = ' '.join([x.text for x in sequence.words])
                            sentence.sequences.append(sequence)
                            sequences.append(sequence)
                            sequence = Sequence()

                        if sentence_complete:
                            sentence.text = ','.join([x.text for x in sentence.sequences])
                            sentence.text = sentence.text + '.'
                            sentences.append(sentence)
                            sentence = Sentence()

                        txt = txt + msg.text.replace('-', '').replace('_', '').replace(',', ', ').replace('.', '.\n')
                else:
                    if msg.type == 'program_change':
                        instrument = int(msg.program)
                    messages.append(msg)
                    if msg.type == 'note_on':
                        note_state_buf[msg.note] = Note(absolute_time, msg.note)
                    if msg.type == 'note_off':
                        absolute_time += msg.time
                        the_note = note_state_buf[msg.note]
                        the_note.duration = msg.time
                        syllable.notes.append(the_note)
                        note_state_buf.pop(msg.note)
                # print("\tMessage %s" % str(msg))
            # print("Track %s\nText:\n%s" % (track.name, txt))
            # print(instrument)
            the_track = Track(instrument, messages, track.name, txt)
            the_track.sentences = sentences
            the_track.sequences = sequences
            the_track.words = words
            the_track.syllables = syllables
            tracks.append(the_track)

        the_score = Score(title=self.score.filename, tracks=tracks, text='\n'.join([t.text for t in tracks]), midi_score=self.score)

        return the_score
