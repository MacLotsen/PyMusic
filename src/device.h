#ifndef MIDI_DEVICE_H
#define MIDI_DEVICE_H

#include <map>
#include <mutex>
#include <thread>
#include <rtmidi/RtMidi.h>

#include "./message.h"


typedef struct {
    std::string device_name;

    unsigned int midi_port;

    RtMidiOut* rt_midi_out = nullptr;
} midi_output_device;


typedef void (*input_broadcast_loop) (midi_output_device*);


typedef struct {
    input_broadcast_loop loop_func = nullptr;

    std::thread *processing_thread = nullptr;

    midi_output_device *output_device = nullptr;
} midi_input_processor;


typedef struct {
    std::string device_name;

    unsigned int midi_port;

    RtMidiIn * rt_midi_in = nullptr;

    midi_input_processor processor;
} midi_input_device;


std::vector<std::string> get_midi_output_device_names();
midi_output_device* get_midi_output_device(std::string);
void destroy_midi_output_device_by_name(std::string);
void destroy_midi_output_device(midi_output_device*);


std::vector<std::string> get_midi_input_device_names();
midi_input_device* get_midi_input_device(std::string);
void destroy_midi_input_device_by_name(std::string);
void destroy_midi_input_device(midi_input_device*);


void start_input_processor(midi_input_processor&);
void start_input_device(midi_input_device&);

#endif //MIDI_DEVICE_H
