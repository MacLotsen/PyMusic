#include <unistd.h>
#include "device.h"


using namespace std;


static map<string, midi_output_device*> output_devices;
static map<string, midi_input_device*> input_devices;


static vector<string> get_device_list(RtMidi* t) {
    vector<string> device_list(t->getPortCount());
    for (unsigned int i = 0; i < t->getPortCount(); i++) {
        device_list[i] = t->getPortName(i);
    }
    return device_list;
}

static const unsigned int unknown_device_name = static_cast<unsigned int>(~0);

static unsigned int get_device_name_match(RtMidi* t, string name) {
    for (unsigned int i = 0; i < t->getPortCount(); i++)
        if (name == t->getPortName(i))
            return i;
    return unknown_device_name;
}

vector<string> get_midi_output_device_names() {
    RtMidiOut * out = new RtMidiOut();
    auto list = get_device_list(out);
    delete out;
    return list;
}


midi_output_device* get_midi_output_device(string device_name) {
    auto it = output_devices.find(device_name);
    if (it != output_devices.end())
        return it->second;

    auto output_device = new midi_output_device;
    output_device->device_name = device_name;
    output_devices[device_name] = output_device;
    output_device->rt_midi_out = new RtMidiOut();

    output_device->midi_port = get_device_name_match(output_device->rt_midi_out, device_name);
    output_device->rt_midi_out->openPort(output_device->midi_port, device_name);

    return output_device;
}

vector<string> get_midi_input_device_names() {
    RtMidiIn * in = new RtMidiIn();
    auto list = get_device_list(in);
    delete in;
    return list;
}

midi_input_device* get_midi_input_device(string device_name) {
    auto it = input_devices.find(device_name);
    if (it != input_devices.end())
        return it->second;

    auto input_device = new midi_input_device;
    input_device->device_name = device_name;
    input_devices[device_name] = input_device;
    input_device->rt_midi_in = new RtMidiIn();

    input_device->midi_port = get_device_name_match(input_device->rt_midi_in, device_name);
    input_device->rt_midi_in->openPort(input_device->midi_port, device_name);

    return input_device;
}

static void basic_input_processor(midi_input_processor *p) {
    while (p->output_device != nullptr)
        if (p->loop_func != nullptr)
            p->loop_func(p->output_device);
}

void start_input_processor(midi_input_processor &p) {
    if (p.processing_thread != nullptr)
        throw "Thread already started";
    if (p.output_device == nullptr)
        throw "No output device selected";
    if (p.loop_func == nullptr)
        throw "No loop function set";
    p.processing_thread = new thread(basic_input_processor, &p);
}

static void device_input_processor(midi_input_device *d) {
    while (d->rt_midi_in->isPortOpen()) {
        vector<unsigned char> in_buf;
        d->rt_midi_in->getMessage(&in_buf);

        if (in_buf.empty()) {
            usleep(20);
            continue;
        }

        d->processor.output_device->rt_midi_out->sendMessage(&in_buf);
    }
}

void start_input_device(midi_input_device &d) {
    if (d.processor.processing_thread != nullptr)
        throw "Thread already started";
    if (d.processor.output_device == nullptr)
        throw "No output device selected";
    d.processor.processing_thread = new thread(device_input_processor, &d);
}

void destroy_midi_output_device_by_name(string name) {
    destroy_midi_output_device(output_devices[name]);
}

void destroy_midi_output_device(midi_output_device *d) {
    if (d->rt_midi_out->isPortOpen())
        d->rt_midi_out->closePort();
    output_devices.erase(d->device_name);
    delete d;
}

void destroy_midi_input_device_by_name(string name) {
    destroy_midi_input_device(input_devices[name]);
}

void destroy_midi_input_device(midi_input_device *d) {
    if (d->rt_midi_in->isPortOpen())
        d->rt_midi_in->closePort();
    if (d->processor.processing_thread->joinable())
        d->processor.processing_thread->join();
    input_devices.erase(d->device_name);
    delete d;
}


