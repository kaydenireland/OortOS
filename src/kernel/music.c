#include "music.h"


void play_entertainer(void) {
    static const struct MusicNote melody[] = {
        // Opening melody of The Entertainer

        { DS5, 140 },
        { E5,  140 },
        { C5,  140 },
        { A4,  140 },

        { C5,  140 },
        { D5,  140 },
        { C5,  140 },
        { E5,  140 },

        { C5,  140 },
        { D5,  140 },
        { C5,  140 },
        { A4,  180 },

        { C5,  140 },
        { E5,  140 },
        { A5,  280 },

        { REST, 100 },

        { A5,  140 },
        { G5,  140 },
        { E5,  140 },
        { C5,  140 },

        { E5,  140 },
        { G5,  140 },
        { E5,  140 },
        { C5,  280 },

        { REST, 150 },

        { D5,  140 },
        { F5,  140 },
        { A5,  140 },
        { G5,  140 },

        { E5,  140 },
        { C5,  140 },
        { E5,  280 }
    };

    u32 count = sizeof(melody) / sizeof(melody[0]);

    for (u32 i = 0; i < count; i++) {
        if (melody[i].frequency == REST) {
            stop_sound();
            timer_sleep(melody[i].duration);
        } else {
            beep(
                melody[i].frequency,
                melody[i].duration
            );
        }

        timer_sleep(15);
    }

    stop_sound();
}



void play_tetris(void) {
    static const struct MusicNote melody[] = {
        // First phrase
        { E5,  260 },
        { B4,  130 },
        { C5,  130 },
        { D5,  260 },
        { C5,  130 },
        { B4,  130 },

        { A4,  260 },
        { A4,  130 },
        { C5,  130 },
        { E5,  260 },
        { D5,  130 },
        { C5,  130 },

        { B4,  390 },
        { C5,  130 },
        { D5,  260 },
        { E5,  260 },

        { C5,  260 },
        { A4,  260 },
        { A4,  520 },

        // Second phrase
        { D5,  260 },
        { F5,  130 },
        { A5,  260 },
        { G5,  130 },
        { F5,  130 },

        { E5,  390 },
        { C5,  130 },
        { E5,  260 },
        { D5,  130 },
        { C5,  130 },

        { B4,  260 },
        { B4,  130 },
        { C5,  130 },
        { D5,  260 },
        { E5,  260 },

        { C5,  260 },
        { A4,  260 },
        { A4,  520 }
    };

    u32 count = sizeof(melody) / sizeof(melody[0]);

    for (u32 i = 0; i < count; i++) {
        beep(melody[i].frequency, melody[i].duration);
        timer_sleep(35);
    }

    stop_sound();
}
