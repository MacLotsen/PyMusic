#ifndef MIDI_DEVICE_H
#define MIDI_DEVICE_H

#include <map>
#include <mutex>
#include <thread>
#include <RtMidi.h>

#include "./message.h"

#define MESSAGE_ARRAY_SIZE 64


typedef struct {
    std::string device_name;

    unsigned int midi_port;

    RtMidiOut* rt_midi_out = nullptr;

    std::thread* processing_thread = nullptr;

    std::mutex* head_lock = new std::mutex;
    volatile int message_array_head = 1;
    std::mutex* tail_lock = new std::mutex;
    volatile int message_array_tail = 0;
    midi_message message_array[MESSAGE_ARRAY_SIZE];
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


int message_array_count(midi_output_device&);
bool message_array_empty(midi_output_device&);
int message_array_next_index(midi_output_device&);
void message_array_push(midi_output_device&, midi_message);
midi_message message_array_pop(midi_output_device&);

std::vector<std::string> get_midi_output_device_names();
midi_output_device* get_midi_output_device(std::string);
void destroy_midi_output_device_by_name(std::string);
void destroy_midi_output_device(midi_output_device*);

std::vector<std::string> get_midi_input_device_names();
midi_input_device* get_midi_input_device(std::string);
void destroy_midi_input_device_by_name(std::string);
void destroy_midi_input_device(midi_input_device*);

void start_output_device(midi_output_device&);
void start_input_processor(midi_input_processor&);
void start_input_device(midi_input_device&);

#endif //MIDI_DEVICE_H
