#include <boost/python.hpp>
#include "pymidi.h"
#include "instrument.h"


MidiOut::MidiOut() {
    out = new RtMidiOut();
    out->openPort(0);
}

MidiOut::~MidiOut() {
    close();
    delete out;
}

void MidiOut::send(boost::python::list n) {
    auto l = static_cast<unsigned long>(len(n));
    auto N = new std::vector<unsigned char>(l);
    for (int i = 0; i < l; i++) {
        (*N)[i] = boost::python::extract<unsigned char>(n[i]);
    }

    out->sendMessage(N);
}

boost::python::list MidiOut::devices() {
    boost::python::list devs;
    for(int i = 0; i < out->getPortCount(); i++) {
        devs.insert(i, boost::python::make_tuple(i, out->getPortName(static_cast<unsigned int>(i))));
    }
    return devs;
}

void MidiOut::device(int i) {
    if (out->isPortOpen())
        out->closePort();
    out->openPort(static_cast<unsigned int>(i));
}

boost::python::list MidiOut::instruments() {
    return boost::python::list();
}

void MidiOut::close() {
    if (out->isPortOpen())
        out->closePort();
}

boost::python::list * _instruments;
boost::python::list * _timbres;

boost::python::list get_instruments() {
    return *_instruments;
}

boost::python::list get_timbres() {
    return *_timbres;
}

boost::python::list get_instruments_by_timbre(std::string t) {
    boost::python::list result;
    for (int i = 0; i < (sizeof(instrument_sets)/sizeof(*instrument_sets)); i++) {
        if (!instrument_sets[i].name.compare(t)) {
            for (int j = instrument_sets[i].from; j <= instrument_sets[i].to; j++) {
                result.append(boost::python::make_tuple(j, instruments[j]));
            }
            return result;
        }
    }
    return result;
}

BOOST_PYTHON_MODULE (pymidi) {
    using namespace boost::python;


    _instruments = new boost::python::list;
    for (int i = 0; i < (sizeof(instruments)/sizeof(*instruments)); i++) {
        _instruments->append(boost::python::make_tuple(i, instruments[i]));
    }
    _timbres = new boost::python::list;
    for (int i = 0; i < (sizeof(instrument_sets)/sizeof(*instrument_sets)); i++) {
        _timbres->append(instrument_sets[i].name);
    }

    def("instruments", &get_instruments);
    def("instruments", &get_instruments_by_timbre);
    def("instrument_sets", &get_timbres);

    class_<MidiOut>("MidiOut")
            .def("send", &MidiOut::send)
            .def("devices", &MidiOut::devices)
            .def("select", &MidiOut::device)
                    ;
}