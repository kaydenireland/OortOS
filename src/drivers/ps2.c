#include "ps2.h"

#include "../arch/port.h"

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64

#define PS2_STATUS_OUTPUT_FULL 0x01
#define PS2_STATUS_INPUT_FULL  0x02
#define PS2_STATUS_AUX_DATA 0x20

#define PS2_CMD_WRITE_TO_MOUSE 0xD4
#define PS2_REPLY_RESEND       0xFE

#define PS2_SPIN_LIMIT 100000

u8 ps2_read_scan_code(void) {
    return inb(PS2_DATA_PORT);
}

static b8 ps2_wait_write(void) {
    for (u32 i = 0; i < PS2_SPIN_LIMIT; i++) {
        if (!(inb(PS2_STATUS_PORT) & PS2_STATUS_INPUT_FULL)) return true;
    }
    return false;
}

static b8 ps2_wait_read(void) {
    for (u32 i = 0; i < PS2_SPIN_LIMIT; i++) {
        if (inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL) return true;
    }
    return false;
}

b8 ps2_write_command(u8 command) {
    if (!ps2_wait_write()) return false;
    outb(PS2_STATUS_PORT, command);
    return true;
}

b8 ps2_write_data(u8 data) {
    if (!ps2_wait_write()) return false;
    outb(PS2_DATA_PORT, data);
    return true;
}

i16 ps2_read_data(void) {
    if (!ps2_wait_read()) return -1;
    return inb(PS2_DATA_PORT);
}

void ps2_flush(void) {
    for (u8 i = 0; i < 16 && (inb(PS2_STATUS_PORT) & PS2_STATUS_OUTPUT_FULL); i++) {
        inb(PS2_DATA_PORT);
    }
}

static b8 ps2_read_from(b8 want_mouse, u8* out) {
    u8 status = inb(PS2_STATUS_PORT);

    if (!(status & PS2_STATUS_OUTPUT_FULL)) return false;
    if (((status & PS2_STATUS_AUX_DATA) != 0) != want_mouse) return false;

    *out = inb(PS2_DATA_PORT);
    return true;
}

b8 ps2_read_keyboard(u8* out) { 
    return ps2_read_from(false, out); 
}

b8 ps2_read_mouse(u8* out)    { 
    return ps2_read_from(true,  out); 
}

i16 ps2_mouse_command(u8 command) {
    for (u8 attempt = 0; attempt < 3; attempt++) {
        if (!ps2_write_command(PS2_CMD_WRITE_TO_MOUSE)) return -1;
        if (!ps2_write_data(command)) return -1;

        i16 reply = ps2_read_data();
        if (reply != PS2_REPLY_RESEND) return reply;
    }
    return -1;
}

i16 ps2_mouse_command_arg(u8 command, u8 argument) {
    if (ps2_mouse_command(command) != 0xFA) return -1;
    return ps2_mouse_command(argument);
}