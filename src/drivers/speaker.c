#include "speaker.h"

#include "../arch/port.h"
#include "pit.h"
#include "../kernel/timer.h"

#define SPEAKER_PORT 0x61

void play_sound(u32 frequency) {
    if (frequency == 0) {
        stop_sound();
        return;
    }

    u32 divisor;
    u8 tmp;

    // set PIT to desired frequency
    divisor = PIT_FREQUENCY / frequency;

    if (divisor == 0 || divisor > MAX_U16) {
        return;
    }

    outb(PIT_COMMAND_PORT, 0xB6);
    outb(PIT_AUDIO_PORT, (u8) divisor);
    outb(PIT_AUDIO_PORT, (u8) (divisor >> 8));

    // play sound using pc speaker
    tmp = inb(0x61);
    outb(SPEAKER_PORT, tmp | 3);
}

void stop_sound(void) {
    u8 tmp = inb(SPEAKER_PORT) & 0xFC;
    outb(SPEAKER_PORT, tmp);
}

void beep(u32 frequency, u32 length) {
    play_sound(frequency);
    timer_sleep(length);
    stop_sound();
}