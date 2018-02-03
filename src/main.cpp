#include <iostream>
#include "message.h"
#include "instrument.h"
#include "device.h"

using namespace std;

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

    // Just in case reset all controllers
    auto controller_reset_msg = vector<unsigned char>(2);
    controller_reset_msg[0] = 0xB0;
    controller_reset_msg[1] = RESET_ALL_CONTROLLERS;
    output_device->rt_midi_out->sendMessage(&controller_reset_msg);

    // Optional input device
    midi_input_device *input_device = nullptr;

    // Let the magic begin
    cout << "Enter a command:" << endl;
    string cmd;

    getline(cin, cmd);

    while (cmd != "q") {
        unsigned int index = 0;
        if (cmd == "instruments") {
            // Print the instrument list...
            for (unsigned int i = 1; i < (sizeof(instruments)/sizeof(*instruments)); i++)
                cout << '#' << i << " " << instruments[i] << endl;
        } else if (begins_with("instrument ", cmd)) {
            // Set instrument for first channel (0xC0, 0xC for program change and next nibble for channel)
            index = static_cast<unsigned int>(stoi(cmd.substr(11)));
            midi_message set_instrument_msg;
            set_instrument_msg.size = 2;
            set_instrument_msg.data = new uint8_t[2] {0xC0, static_cast<uint8_t>(index)};
            //TODO make helper method
            auto raw_msg = vector<unsigned char>(2);
            for (int i = 0; i < set_instrument_msg.size; i++)
                raw_msg[i] = set_instrument_msg.data[i];
            output_device->rt_midi_out->sendMessage(&raw_msg);
        } else if (cmd == "types") {
            // Print types of the instruments
            for (auto timbre_sets : instrument_sets) {
                cout << '#' << index << " " << timbre_sets.name << endl;
                index++;
            }
        } else if (begins_with("type ", cmd)) {
            // Print instruments of type ...
            index = static_cast<unsigned int>(stoi(cmd.substr(5)));
            auto instrument_set = instrument_sets[index];
            for (int i = instrument_set.from; i < instrument_set.to; i++) {
                cout << '#' << i << " " << instruments[i] << endl;
            }
        } else if (cmd == "inputs") {
            // Print input devices
            auto input_devices = get_midi_input_device_names();
            for (unsigned int i = 0; i < input_devices.size(); i++)
                cout << "#" << i << " " << input_devices[i] << endl;
        } else if (begins_with("input ", cmd)) {
            // Select input device
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

    // Turn off all notes for first channel
    vector<unsigned char> turn_off_msg = vector<unsigned char>(2);
    turn_off_msg[0] = 0xB0;
    turn_off_msg[1] = ALL_NOTES_OFF;

    output_device->rt_midi_out->sendMessage(&turn_off_msg);
    turn_off_msg[1] = ALL_SOUNDS_OFF;
    output_device->rt_midi_out->sendMessage(&turn_off_msg);


    // Destroy devices
    if (input_device)
        destroy_midi_input_device(input_device);
    destroy_midi_output_device(output_device);

    return 0;
}
