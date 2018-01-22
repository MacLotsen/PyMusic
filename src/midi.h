//
// Created by erik on 17-10-17.
//

#ifndef MUSIC_MAKER_INSTRUMENT_H
#define MUSIC_MAKER_INSTRUMENT_H



// COPIED FROM: http://www.ccarh.org/courses/253/handout/gminstruments/

typedef struct {
    std::string name;
    int from, to;
} timbre_t;

const timbre_t timbres[] = {
        {"Piano_Timbres", 1, 8},
        {"Chromatic_Percussion", 9, 16},
        {"Organ_Timbres", 17, 24},
        {"Guitar_Timbres", 25, 32},
        {"Bass_Timbres", 33, 40},
        {"String_Timbres", 41, 48},
        {"Ensemble_Timbres", 49, 56},
        {"Brass_Timbres", 57, 64},
        {"Reed_Timbres", 65, 72},
        {"Pipe_Timbres", 73, 80},
        {"Synth_Lead", 81, 88},
        {"Synth_Pad", 89, 96},
        {"Synth_Effects", 97, 104},
        {"Ethnic_Timbres", 105, 112},
        {"Sound Effects", 113, 128},
};


const std::string instruments[] = {
    "None",
    // Piano Timbres:
    "Acoustic_Grand_Piano",
    "Bright_Acoustic_Piano",
    "Electric_Grand_Piano",
    "Honkytonk_Piano",
    "Rhodes_Piano",
    "Chorused_Piano",
    "Harpsichord",
    "Clavinet",
    // Chromatic Percussion:
    "Celesta",
    "Glockenspiel",
    "Music_Box",
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "Tubular_Bells",
    "Dulcimer",
    // Organ Timbres:
    "Hammond_Organ",
    "Percussive_Organ",
    "Rock_Organ",
    "Church_Organ",
    "Reed_Organ",
    "Accordion",
    "Harmonica",
    "Tango_Accordion",
    // Guitar Timbres:
    "Acoustic_Nylon_Guitar",
    "Acoustic_Steel_Guitar",
    "Electric_Jazz_Guitar",
    "Electric_Clean_Guitar",
    "Electric_Muted_Guitar",
    "Overdriven_Guitar",
    "Distortion_Guitar",
    "Guitar_Harmonics",
    // Bass Timbres:
    "Acoustic_Bass",
    "Fingered_Electric_Bass",
    "Plucked_Electric_Bass",
    "Fretless_Bass",
    "Slap_Bass_1",
    "Slap_Bass_2",
    "Synth_Bass_1",
    "Synth_Bass_2",
    // String Timbres:
    "Violin",
    "Viola",
    "Cello",
    "Contrabass",
    "Tremolo_Strings",
    "Pizzicato_Strings",
    "Orchestral_Harp",
    "Timpani",
    // Ensemble Timbres:
    "String_Ensemble_1",
    "String_Ensemble_2",
    "Synth_Strings_1",
    "Synth_Strings_2",
    "Choir_Aah",
    "Choir_Ooh",
    "Synth_Voice",
    "Orchestral_Hit",
    // Brass Timbres:
    "Trumpet",
    "Trombone",
    "Tuba",
    "Muted_Trumpet",
    "French_Horn",
    "Brass_Section",
    "Synth_Brass_1",
    "Synth_Brass_2",
    // Reed Timbres:
    "Soprano_Sax",
    "Alto_Sax",
    "Tenor_Sax",
    "Baritone_Sax",
    "Oboe",
    "English_Horn",
    "Bassoon",
    "Clarinet",
    // Pipe Timbres:
    "Piccolo",
    "Flute",
    "Recorder",
    "Pan_Flute",
    "Bottle_Blow",
    "Shakuhachi",
    "Whistle",
    "Ocarina",
    // Synth Lead:
    "Square_Wave_Lead",
    "Sawtooth_Wave_Lead",
    "Calliope_Lead",
    "Chiff_Lead",
    "Charang_Lead",
    "Voice_Lead",
    "Fifths_Lead",
    "Bass_Lead",
    // Synth Pad:
    "New_Age_Pad",
    "Warm_Pad",
    "Polysynth_Pad",
    "Choir_Pad",
    "Bowed_Pad",
    "Metallic_Pad",
    "Halo_Pad",
    "Sweep_Pad",
    // Synth Effects:
    "Rain_Effect",
    "Soundtrack_Effect",
    "Crystal_Effect",
    "Atmosphere_Effect",
    "Brightness_Effect",
    "Goblins_Effect",
    "Echoes_Effect",
    "SciFi_Effect",
    // Ethnic Timbres:
    "Sitar",
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "Bagpipe",
    "Fiddle",
    "Shanai",
    // Sound Effects:
    "Tinkle_Bell",
    "Agogo",
    "Steel_Drums",
    "Woodblock",
    "Taiko_Drum",
    "Melodic_Tom",
    "Synth_Drum",
    "Reverse_Cymbal",
    // Sound Effects:
    "Guitar_Fret_Noise",
    "Breath_Noise",
    "Seashore",
    "Bird_Tweet",
    "Telephone_Ring",
    "Helicopter",
    "Applause",
    "Gun_Shot"
};

#endif //MUSIC_MAKER_INSTRUMENT_H
