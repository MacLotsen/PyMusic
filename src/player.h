#ifndef MUSIC_MAKER_DEVICES_H
#define MUSIC_MAKER_DEVICES_H

#include <string>
#include <map>

int initDevices();
void detachDevices();

typedef std::map<std::string, unsigned int> device_map_t;

typedef struct {
    unsigned char action, cent;
    int velocity;
} midi_note_t;

typedef void (*io_converter_f) (double stamp, midi_note_t in, midi_note_t& out);

device_map_t getInputDevices();
device_map_t getOutputDevices();

void process(midi_note_t e);

void useInputDevice(unsigned int);
void useOutputDevice(unsigned int);

void setIOConverter(io_converter_f);

void runLoop();
void pauseLoop();
void destroyLoop();

bool isAlive();
bool isRunning();

void yield();

#endif //MUSIC_MAKER_DEVICES_H
