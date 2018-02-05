#ifndef PYMUSIC_PYMIDI_H
#define PYMUSIC_PYMIDI_H

#include "./device.h"
#include "./instrument.h"

class MidiMessage {
private:
    midi_message * msg = nullptr;
public:
    explicit MidiMessage(boost::python::list bytes) {
        uint8_t * data = nullptr;
        if (len(bytes) > 2)
            data = new uint8_t[3] {
                    boost::python::extract<unsigned char>(bytes[0]),
                    boost::python::extract<unsigned char>(bytes[1]),
                    boost::python::extract<unsigned char>(bytes[2])
            };
        else
            data = new uint8_t[2] {
                    boost::python::extract<unsigned char>(bytes[0]),
                    boost::python::extract<unsigned char>(bytes[1])
            };
        msg = new midi_message {static_cast<size_t>(len(bytes)), data };
    }
    ~MidiMessage() {
        delete [] msg->data;
        delete msg;
    }
    midi_message_type get_type() {
        return message_type_of(*msg);
    }

    midi_channel_type get_channel_type() {
        assert(get_type() == midi_message_type::CHANNEL_MESSAGE);
        return message_channel_type_of(*msg);
    }

    midi_channel_voice_type get_voice_type() {
        assert(get_channel_type() == midi_channel_type::CHANNEL_VOICE_MESSAGE);
        return message_channel_voice_type_of(*msg);
    }

    midi_channel_mode_type get_mode_type() {
        assert(get_channel_type() == midi_channel_type::CHANNEL_MODE_MESSAGE);
        return message_channel_mode_type_of(*msg);
    }

    midi_system_type get_system_type() {
        assert(get_type() == midi_message_type::SYSTEM_MESSAGE);
        return message_system_type_of(*msg);
    }

    midi_system_common_type get_common_type() {
        assert(get_system_type() == midi_system_type::SYSTEM_COMMON_MESSAGE);
        return message_system_common_type_of(*msg);
    };

    midi_system_realtime_type get_realtime_type() {
        assert(get_system_type() == midi_system_type::SYSTEM_REALTIME_MESSAGE);
        return message_system_realtime_type_of(*msg);
    }
};

class MidiOutputDevice {
private:
    midi_output_device *output_device = nullptr;
public:
    explicit MidiOutputDevice(std::string device_name) {
        output_device = get_midi_output_device(device_name);
    }

    ~MidiOutputDevice() {
        destroy_midi_output_device(output_device);
    }

    std::string get_name() {
        return output_device->device_name;
    }
    friend class MidiInputDevice;
};

class MidiInputDevice {
private:
    midi_input_device *input_device = nullptr;
public:
    MidiInputDevice(std::string device_name, MidiOutputDevice *output_device) {
        input_device = get_midi_input_device(device_name);
        input_device->processor.output_device = output_device->output_device;
    }
    ~MidiInputDevice() {
        destroy_midi_input_device(input_device);
    }

    std::string get_name() {
        return input_device->device_name;
    }
};

boost::python::list get_output_names();
boost::python::list get_input_names();

boost::python::list get_instruments();
boost::python::list get_instrument_sets();

#endif //PYMUSIC_PYMIDI_H
