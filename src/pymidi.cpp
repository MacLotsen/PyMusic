#include <boost/python.hpp>

#include "./pymidi.h"

using namespace std;
using namespace boost::python;

list get_output_names() {
    list _l = list();
    auto vec = get_midi_output_device_names();
    for (auto name : vec)
        _l.append(name);
    return _l;
}

list get_input_names() {
    list _l = list();
    auto vec = get_midi_input_device_names();
    for (auto name : vec)
        _l.append(name);
    return _l;
}

list *_instruments;
list *_instrument_sets;

list get_instruments() { return *_instruments; }

list get_instrument_sets() { return *_instrument_sets; }

BOOST_PYTHON_MODULE (pymidi) {

    enum_<midi_message_type>("MsgType")
            .value("channel", midi_message_type::CHANNEL_MESSAGE)
            .value("sys", midi_message_type::SYSTEM_MESSAGE);

    enum_<midi_channel_type>("ChannelType")
            .value("voice", midi_channel_type::CHANNEL_VOICE_MESSAGE)
            .value("mode", midi_channel_type::CHANNEL_MODE_MESSAGE);

    enum_<midi_channel_voice_type>("VoiceType")
            .value("note_off", midi_channel_voice_type::NOTE_OFF)
            .value("note_on", midi_channel_voice_type::NOTE_ON)
            .value("poly_key_pressure", midi_channel_voice_type::POLY_KEY_PRESSURE)
            .value("control_change", midi_channel_voice_type::CONTROL_CHANGE)
            .value("program_change", midi_channel_voice_type::PROGRAM_CHANGE)
            .value("channel_pressure", midi_channel_voice_type::CHANNEL_PRESSURE)
            .value("pitch_bend", midi_channel_voice_type::PITCH_BEND);

    enum_<midi_channel_mode_type>("ModeType")
            .value("all_sounds_off", midi_channel_mode_type::ALL_SOUNDS_OFF)
            .value("reset_all_controllers", midi_channel_mode_type::RESET_ALL_CONTROLLERS)
            .value("local_control", midi_channel_mode_type::LOCAL_CONTROL)
            .value("all_notes_off", midi_channel_mode_type::ALL_NOTES_OFF)
            .value("omni_off", midi_channel_mode_type::OMNI_OFF)
            .value("omni_on", midi_channel_mode_type::OMNI_ON)
            .value("mono_on", midi_channel_mode_type::MONO_ON)
            .value("poly_on", midi_channel_mode_type::POLY_ON);

    enum_<midi_system_type>("SystemType")
            .value("common", midi_system_type::SYSTEM_COMMON_MESSAGE)
            .value("realtime", midi_system_type::SYSTEM_REALTIME_MESSAGE)
            .value("exclusive", midi_system_type::SYSTEM_EXCLUSIVE_MESSAGE);

    enum_<midi_system_common_type>("CommonType")
            .value("midi_time_quarter_frame", midi_system_common_type::MIDI_TIME_QUARTER_FRAME)
            .value("song_position_pointer", midi_system_common_type::SONG_POSITION_POINTER)
            .value("song_select", midi_system_common_type::SONG_SELECT)
            .value("tune_request", midi_system_common_type::TUNE_REQUEST)
            .value("eox", midi_system_common_type::EOX);

    enum_<midi_system_realtime_type>("RealtimeType")
            .value("timing_clock", midi_system_realtime_type::TIMING_CLOCK)
            .value("start", midi_system_realtime_type::START)
            .value("continue", midi_system_realtime_type::CONTINUE)
            .value("stop", midi_system_realtime_type::STOP)
            .value("active_sensing", midi_system_realtime_type::ACTIVE_SENSING)
            .value("system_reset", midi_system_realtime_type::SYSTEM_RESET);

    class_<MidiMessage>("MidiMessage", init<list>())
            .def("get_type", &MidiMessage::get_type)
            .def("get_channel_type", &MidiMessage::get_channel_type)
            .def("get_system_type", &MidiMessage::get_system_type)
            .def("get_voice_type", &MidiMessage::get_voice_type)
            .def("get_mode_type", &MidiMessage::get_mode_type)
            .def("get_common_type", &MidiMessage::get_common_type)
            .def("get_realtime_type", &MidiMessage::get_realtime_type);

    def("output_names", &get_output_names);
    def("input_names", &get_input_names);

    class_<MidiOutputDevice>("MidiOutputDevice", init<string>())
            .add_property("name", &MidiOutputDevice::get_name);

    class_<MidiInputDevice>("MidiInputDevice", init<string, MidiOutputDevice*>())
            .add_property("name", &MidiInputDevice::get_name);

    _instruments = new list;
    for (int i = 0; i < (sizeof(instruments) / sizeof(*instruments)); i++) {
        _instruments->append(make_tuple(i, instruments[i]));
    }
    _instrument_sets = new list;
    for (int i = 0; i < (sizeof(instrument_sets) / sizeof(*instrument_sets)); i++) {
        _instrument_sets->append(instrument_sets[i].name);
    }

    def("instruments", &get_instruments);
    def("instrument_sets", &get_instrument_sets);

}