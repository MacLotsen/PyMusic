#include <iostream>
#include <signal.h>
#include "player.h"
#include "midi.h"

using namespace std;

void terminate(int) {
    if (isAlive())
        destroyLoop();
    detachDevices();
}

unsigned int prompt_device_selection(device_map_t map) {
    unsigned int index;
    for (auto kv : map) {
        cout << '#' << kv.second << ' ' << kv.first << endl;
    }
    cout << "Insert midi device id: " << endl;
    cin >> index;
    return index;
}

unsigned int prompt_instruments_selection() {
    unsigned int index = 0;
    for (auto timbre_sets : timbres) {
        cout << '#' << index << timbre_sets.name << endl;
        index++;
    }
    cout << "Choose timbre group" << endl;
    cin >> index;
    timbre_t timbre_set = timbres[index];
    for (index = 0; index <= timbre_set.to - timbre_set.from; index++) {
        cout << '#' << index << instruments[index + timbre_set.from] << endl;
    }
    cout << "Choose instrument" << endl;
    cin >> index;

    return timbre_set.from + index;
}

void myIoConverter(double stamp, midi_note_t in, midi_note_t& out) {
    out.action = in.action;
    out.cent = in.cent;

    // normal use case
    //out.velocity = in.velocity;

    // no case sensitivity
    out.velocity = (in.action == 128) ? 64: 127;

    cout << "Sysex: "
         << (int) in.action << endl
         << "Cent: " << (int) in.cent << endl
         << "Velocity: " << in.velocity << endl << endl;
}

int main(int argc, char** argv) {

    signal(SIGABRT, &terminate);
    signal(SIGTERM, &terminate);
    signal(SIGINT, &terminate);

    int status = initDevices();

    if (status > 0) {
        return status;
    }

    useInputDevice(prompt_device_selection(getInputDevices()));
    useOutputDevice(prompt_device_selection(getOutputDevices()));



    setIOConverter(myIoConverter);

    runLoop();

    midi_note_t instrument = {
            0xC0,
            static_cast<unsigned char>(prompt_instruments_selection()), -1
    };

    process(instrument);

    //yield();

    char progress;
    midi_note_t e;
    string cmd_buffer;
    string cmd;

    getline(cin, cmd);

    while (cmd != "q") {
        cmd_buffer = "";
        progress = 0;
        for (auto c : cmd) {
            if (c == ' ') {
                if (progress == 0)
                    e.action = (unsigned char) stoi(cmd_buffer);
                else if (progress == 1)
                    e.cent = (unsigned char) stoi(cmd_buffer);
                else
                    cerr << "Invalid count of commands..." << endl;
                cmd_buffer = "";
                progress++;
            } else {
                cmd_buffer += c;
            }
        }

        if (progress == 2) {
            e.velocity = (unsigned char) stoi(cmd_buffer);
            process(e);
        }

        getline(cin, cmd);
    }

    destroyLoop();
    detachDevices();

    return 0;
}
