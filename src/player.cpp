#include <unistd.h>
#include <thread>
#include <RtMidi.h>
#include "player.h"

using namespace std;

unsigned int port_in, port_out;

vector<unsigned char> instrument_buf(2);
vector<unsigned char> output_buf(3);
bool loop_running = false;
bool loop_alive = false;
thread* loop_thread = nullptr;

RtMidiIn* in = nullptr;
RtMidiOut* out = nullptr;

io_converter_f io_converter = nullptr;

template<class T>
device_map_t getDevices(T* t) {
    device_map_t map;
    unsigned int port_c = t->getPortCount();
    for (unsigned int i = 0; i < port_c; i++) {
        map[t->getPortName(i)] = i;
    }
    return map;
}

device_map_t getInputDevices() {
    return getDevices<RtMidiIn>(in);
}

device_map_t getOutputDevices() {
    return getDevices<RtMidiOut>(out);
}

void useInputDevice(unsigned int port) {
    if (in->isPortOpen())
        in->closePort();
    in->openPort(port);
}

void useOutputDevice(unsigned int port) {
    if (out->isPortOpen())
        out->closePort();
    out->openPort(port);
}

void process(midi_note_t e) {
    if (e.velocity < 0) {
        instrument_buf[0] = e.action;
        instrument_buf[1] = e.cent;
        if (out) {
            out->sendMessage(&instrument_buf);
        }
    } else {
        output_buf[0] = e.action;
        output_buf[1] = e.cent;
        output_buf[2] = static_cast<unsigned char>(e.velocity);
        if (out) {
            out->sendMessage(&output_buf);
        }
    }
}

void setIOConverter(io_converter_f f) {
    io_converter = f;
}

midi_note_t parseBuffer(vector<unsigned char>& msg) {
    midi_note_t e;
    e.action = *msg.begin();
    msg.erase(msg.begin());
    e.cent = *msg.begin();
    msg.erase(msg.begin());
    e.velocity = *msg.begin();
    msg.erase(msg.begin());
    return e;
}

vector<unsigned char> toRaw(midi_note_t msg) {
    return vector<unsigned char> { msg.action, msg.cent, msg.velocity };
}

void inputLoop() {
    loop_alive = loop_running = true;
    double stamp;
    vector<unsigned char> msg_buffer;
    while (loop_alive) {

        while (loop_running) {
            stamp = in->getMessage(&msg_buffer);

            if (msg_buffer.size() == 0) {
                usleep(20);
                continue;
            }

            do {
                midi_note_t in_e = parseBuffer(msg_buffer);
                midi_note_t out_e;
                io_converter(stamp, in_e, out_e);
                vector<unsigned char> out_msg = toRaw(out_e);
                out->sendMessage(&out_msg);
            } while (io_converter && msg_buffer.size() > 0);

            usleep(10);
        }

    }
}

void runLoop() {
    if (loop_thread)
        delete loop_thread;
    loop_thread = new thread(inputLoop);
}

void pauseLoop() {
    loop_running = false;
}

void destroyLoop() {
    loop_alive = loop_running = false;
    if (loop_thread->joinable())
        loop_thread->join();
    delete loop_thread;
    loop_thread = nullptr;
}

bool isAlive() {
    return loop_alive;
}

bool isRunning() {
    return loop_running;
}

void yield() {
    loop_thread->join();
}

int initDevices() {
    try {
        in = new RtMidiIn();
        out = new RtMidiOut();
    } catch (RtMidiError& error) {
        error.printMessage();
        return 1;
    }
    return 0;
}

void detachDevices() {
    if (in) {
        if (in->isPortOpen()) in->closePort();
        delete in;
        in = nullptr;
    }
    if (out) {
        if (out->isPortOpen()) out->closePort();
        delete out;
        out = nullptr;
    }
}
