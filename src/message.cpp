#include "message.h"




static uint8_t most_significant_nibble(const midi_message &m) {
    return m.data[0] >> 4;
}


static uint8_t least_significant_nibble(const midi_message &m) {
    return static_cast<uint8_t>(m.data[0] & 0x0F);
}


midi_message_type message_type_of(const midi_message &m) {
    uint8_t status = most_significant_nibble(m);
    if (status >= 0x8 && status <= 0xE)
        return CHANNEL_MESSAGE;
    return SYSTEM_MESSAGE;
}

midi_channel_type message_channel_type_of(const midi_message &m) {
    uint8_t status = most_significant_nibble(m);
    uint8_t data = m.data[1];
    if (status == 0xB && data > 119)
        return CHANNEL_MODE_MESSAGE;
    return CHANNEL_VOICE_MESSAGE;
}

midi_channel_voice_type message_channel_voice_type_of(const midi_message &m) {
    return static_cast<midi_channel_voice_type>(most_significant_nibble(m));
}

midi_channel_mode_type message_channel_mode_type_of(const midi_message &) {
    return ALL_NOTES_OFF;
}

midi_system_type message_system_type_of(const midi_message &m) {
    uint8_t nibble = static_cast<uint8_t>(m.data[0] & 0x0F);
    if (nibble == 0)
        return SYSTEM_EXCLUSIVE_MESSAGE;
    else if (nibble < 0x08)
        return SYSTEM_COMMON_MESSAGE;
    return SYSTEM_REALTIME_MESSAGE;
}

midi_system_common_type message_system_common_type_of(const midi_message &m) {
    return static_cast<midi_system_common_type>(least_significant_nibble(m));
}

midi_system_realtime_type message_system_realtime_type_of(const midi_message &m) {
    return static_cast<midi_system_realtime_type>(least_significant_nibble(m));
}
