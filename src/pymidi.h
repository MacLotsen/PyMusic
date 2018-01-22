#ifndef PYMIDI_H
#define PYMIDI_H

#include <RtMidi.h>
#include "player.h"

boost::python::list get_instruments_by_timbre(std::string t);

class MidiOut {
public:
    MidiOut();
    ~MidiOut();
    void send(boost::python::list);
    boost::python::list devices();
    void device(int i);
    boost::python::list instruments();
    void close();
private:
    RtMidiOut* out;
    std::vector<unsigned char> buf;
};

#endif
