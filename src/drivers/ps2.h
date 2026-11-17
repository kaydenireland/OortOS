#ifndef PS2_H
#define PS2_H

#include "../util/numbers.h"

u8 ps2_read_scan_code(void);
b8 ps2_write_command(u8 command);
b8 ps2_write_data(u8 data);
i16 ps2_read_data(void);
void ps2_flush(void);

b8 ps2_read_keyboard(u8* out);
b8 ps2_read_mouse(u8* out);

i16 ps2_mouse_command(u8 command);
i16 ps2_mouse_command_arg(u8 command, u8 argument);

#endif