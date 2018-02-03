#ifndef MIDI_MESSAGE_H
#define MIDI_MESSAGE_H

// http://oktopus.hu/uploaded/Tudastar/MIDI%201.0%20Detailed%20Specification.pdf


#include <cstdlib>
#include <cstdint>


typedef struct {
    size_t size;
    uint8_t * data;
} midi_message;


typedef enum {
    CHANNEL_MESSAGE,
    SYSTEM_MESSAGE
} midi_message_type;


typedef enum {
    CHANNEL_VOICE_MESSAGE,
    CHANNEL_MODE_MESSAGE,
} midi_channel_type;


typedef enum {
    NOTE_OFF = 0x8,
    NOTE_ON = 0x9,
    POLY_KEY_PRESSURE = 0xA,
    CONTROL_CHANGE = 0xB,
    PROGRAM_CHANGE = 0xC,
    CHANNEL_PRESSURE = 0xD,
    PITCH_BEND = 0xE
} midi_channel_voice_type;


typedef enum {
    ALL_SOUNDS_OFF = 120,
    RESET_ALL_CONTROLLERS = 121,
    LOCAL_CONTROL = 122,
    ALL_NOTES_OFF = 123,
    OMNI_OFF = 124,
    OMNI_ON = 125,
    MONO_ON = 126,
    POLY_ON = 127
} midi_channel_mode_type;


typedef enum {
    SYSTEM_COMMON_MESSAGE,
    SYSTEM_REALTIME_MESSAGE,
    SYSTEM_EXCLUSIVE_MESSAGE
} midi_system_type;


typedef enum {
    MIDI_TIME_QUARTER_FRAME = 0x1,
    SONG_POSITION_POINTER = 0x2,
    SONG_SELECT = 0x3,
    TUNE_REQUEST = 0x6,
    EOX = 0x7
} midi_system_common_type;


typedef enum {
    TIMING_CLOCK = 0x8,
    START = 0xA,
    CONTINUE = 0xB,
    STOP = 0xC,
    ACTIVE_SENSING = 0xE,
    SYSTEM_RESET = 0xF
} midi_system_realtime_type;


midi_message_type message_type_of(const midi_message&);

midi_channel_type message_channel_type_of(const midi_message&);
midi_channel_voice_type message_channel_voice_type_of(const midi_message &);
midi_channel_mode_type message_channel_mode_type_of(const midi_message &);

midi_system_type message_system_type_of(const midi_message&);
midi_system_common_type message_system_common_type_of(const midi_message &);
midi_system_realtime_type message_system_realtime_type_of(const midi_message &);

#endif //MIDI_MESSAGE_H
