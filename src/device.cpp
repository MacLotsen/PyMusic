#include <unistd.h>
#include "device.h"


using namespace std;


static map<string, midi_output_device*> output_devices;
static map<string, midi_input_device*> input_devices;

int message_array_count(midi_output_device &d) {
    if (d.message_array_head >= d.message_array_tail)
        return (d.message_array_tail - d.message_array_head) + 1;
    else
        return (d.message_array_tail - (d.message_array_head + MESSAGE_ARRAY_SIZE)) + 1;
}

bool message_array_empty(midi_output_device &d) {
    return message_array_count(d) == 0;
}

int message_array_next_index(midi_output_device &d) {
    return ++d.message_array_tail % MESSAGE_ARRAY_SIZE;
}

void message_array_push(midi_output_device &d, midi_message msg) {
    d.tail_lock->lock();
    d.message_array[message_array_next_index(d)] = msg;
    d.tail_lock->unlock();
}

midi_message message_array_pop(midi_output_device &d) {
    midi_message *msg = nullptr;
    d.head_lock->lock();
    msg = &d.message_array[d.message_array_head++];
    d.head_lock->unlock();
    if (msg == nullptr)
        throw "Empty queue. Use '!message_array_empty()' first...";
    return *msg;
}

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

static void output_processor(midi_output_device *d) {
    while (d->rt_midi_out->isPortOpen()) {
        while (!message_array_empty(*d)) {
            midi_message msg = message_array_pop(*d);

            vector<unsigned char> rt_midi_buf(msg.size);
            for (int i = 0; i < msg.size; i++)
                rt_midi_buf[i] = msg.data[i];
            d->rt_midi_out->sendMessage(&rt_midi_buf);
        }
    }
}

void start_output_device(midi_output_device &d) {
    if (d.processing_thread != nullptr)
        throw "Thread already started";
    d.processing_thread = new thread(output_processor, &d);
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
    if (d->processing_thread->joinable())
        d->processing_thread->join();
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


