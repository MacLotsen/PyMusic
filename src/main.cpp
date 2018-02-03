#include <iostream>
#include <csignal>
#include "message.h"
#include "instrument.h"
#include "device.h"

using namespace std;

//void terminate(int) {
//    if (isAlive())
//        destroyLoop();
//    detachDevices();
//}
//
//unsigned int prompt_device_selection(device_map_t map) {
//    unsigned int index;
//    for (auto kv : map) {
//        cout << '#' << kv.second << ' ' << kv.first << endl;
//    }
//    cout << "Insert midi device id: " << endl;
//    cin >> index;
//    return index;
//}
//
//unsigned int prompt_instruments_selection() {
//    unsigned int index = 0;
//    for (auto timbre_sets : instrument_sets) {
//        cout << '#' << index << timbre_sets.name << endl;
//        index++;
//    }
//    cout << "Choose timbre group" << endl;
//    cin >> index;
//    instrument_set_t timbre_set = instrument_sets[index];
//    for (index = 0; index <= timbre_set.to - timbre_set.from; index++) {
//        cout << '#' << index << instruments[index + timbre_set.from] << endl;
//    }
//    cout << "Choose instrument" << endl;
//    cin >> index;
//
//    return timbre_set.from + index;
//}
//
//void myIoConverter(double stamp, midi_note_t in, midi_note_t& out) {
//    out.action = in.action;
//    out.cent = in.cent;
//
//    // normal use case
//    //out.velocity = in.velocity;
//
//    // no case sensitivity
//    out.velocity = (in.action == 128) ? 64: 127;
//
//    cout << "Sysex: "
//         << (int) in.action << endl
//         << "Cent: " << (int) in.cent << endl
//         << "Velocity: " << in.velocity << endl << endl;
//}

static void custom_loop(midi_output_device *d) {

}

bool begins_with(string str1, string str2) {
    if (str2.length() < str1.length())
        return false;
    for (unsigned int i = 0; i < str1.length(); i++)
        if (str1.at(i) != str2.at(i))
            return false;
    return true;
}

int main(int argc, char** argv) {
    // Prompting an output device
    auto output_devices = get_midi_output_device_names();
    for (unsigned int i = 0; i < output_devices.size(); i++) {
        cout << "#" << i << " " << output_devices.at(i) << endl;
    }
    cout << "Enter id:" << endl;
    unsigned int out_index;
    cin >> out_index;
    auto output_device = get_midi_output_device(output_devices.at(out_index));
    start_output_device(*output_device);

    // Setting custom loop for now
    midi_input_processor input;
    input.loop_func = custom_loop;
    input.output_device = output_device;
    start_input_processor(input);

    // Optional input device
    midi_input_device *input_device = nullptr;

    // Let the magic begin
    cout << "Enter a command:" << endl;
    string cmd;

    getline(cin, cmd);

    while (cmd != "q") {
        unsigned int index = 0;
        if (cmd == "instruments") {

        } else if (cmd == "types") {
            for (auto timbre_sets : instrument_sets) {
                cout << '#' << index << timbre_sets.name << endl;
                index++;
            }
        } else if (begins_with("type ", cmd)) {

        } else if (cmd == "inputs") {
            auto input_devices = get_midi_input_device_names();
            for (unsigned int i = 0; i < input_devices.size(); i++)
                cout << "#" << i << " " << input_devices[i] << endl;
        } else if (begins_with("input ", cmd)) {
            if (input_device == nullptr) {
                index = static_cast<unsigned int>(stoi(cmd.substr(6)));
                input_device = get_midi_input_device(get_midi_input_device_names().at(index));
                input_device->processor.output_device = output_device;
                start_input_device(*input_device);
            } else {
                cout << "Already registered an input device" << endl;
            }
        }
        cmd = "";
        getline(cin, cmd);
    }

    if (input_device)
        destroy_midi_input_device(input_device);
    input.output_device = nullptr;
    if (input.processing_thread && input.processing_thread->joinable())
        input.processing_thread->join();
    destroy_midi_output_device(output_device);

    return 0;
}
