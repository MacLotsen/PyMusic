from rtmidi import MidiOut
from heapq import heappop, heappush
from time import sleep
from pymusic.midi.parser import MidiParser
from pymusic.score import Rest

p = MidiParser('res/palestrina_missa_papae_marcelli_kyrie.mid')
score = p.parse()


output = MidiOut()
output.open_port(5)

cursor_heap = []
tracks = {}
cur_time = 0

_track_id = 0

for track in score.tracks:
    notes_sets_of_sylls = [s.notes for s in track.syllables]
    notes = []
    for nset in notes_sets_of_sylls:
        notes += nset
    if len(notes) == 0:
        continue
    tracks[_track_id] = notes
    output.send_message([0xC0 + _track_id, 0])#track.instrument])
    heappush(cursor_heap, (tracks[_track_id][0].time_in_score, (0, _track_id)))
    if type(tracks[_track_id][0]) != Rest:
        output.send_message([0x90 + _track_id, tracks[_track_id][0].midi_note, 0x40])
    _track_id += 1

while len(cursor_heap) > 0:
    duration, nearest = heappop(cursor_heap)

    i, ti = nearest

    print("Track: %d, CurTime: %5d, Duration: %4d, Note: %d" % (ti, cur_time, duration, tracks[ti][i].midi_note))

    if duration != 0:
        cur_time += duration
        cursor_heap = [(x - duration, y) for x, y in cursor_heap]
        sleep(duration/(score.bpm*2))

    if tracks[ti][i].midi_note != -1:
        output.send_message([0x90 + ti, tracks[ti][i].midi_note, 0x00])

    i = i + 1
    if i < len(tracks[ti]):
        heappush(cursor_heap, (tracks[ti][i].duration, (i, ti)))
        if tracks[ti][i].midi_note != -1:
            output.send_message([0x90 + ti, tracks[ti][i].midi_note, 0x40])








