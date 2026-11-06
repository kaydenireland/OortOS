#ifndef SPEAKER_H
#define SPEAKER_H

#include "../util/types.h"

enum {
    C4 = 262,
    D4 = 294,
    E4 = 330,
    F4 = 349,
    G4 = 392,
    A4 = 440,
    B4 = 494,
    C5 = 523
};

void play_sound(u32 frequency);
void stop_sound(void);
void beep(u32 frequency, u32 length);

#endif