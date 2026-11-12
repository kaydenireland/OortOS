#ifndef MUSIC_H
#define MUSIC_H

#include "../util/numbers.h"


enum NoteFrequency {
    REST = 0,

    C4  = 262,
    CS4 = 277,
    D4  = 294,
    DS4 = 311,
    E4  = 330,
    F4  = 349,
    FS4 = 370,
    G4  = 392,
    GS4 = 415,
    A4  = 440,
    AS4 = 466,
    B4  = 494,

    C5  = 523,
    CS5 = 554,
    D5  = 587,
    DS5 = 622,
    E5  = 659,
    F5  = 698,
    FS5 = 740,
    G5  = 784,
    GS5 = 831,
    A5  = 880,
    AS5 = 932,
    B5  = 988,

    C6  = 1047
};

struct MusicNote {
    u32 frequency;
    u32 duration;
};

void play_entertainer(void);
void play_tetris(void);


#endif