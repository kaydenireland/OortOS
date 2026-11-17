#include "mouse.h"

#include "../util/debug.h"
#include "../arch/port.h"
#include "../drivers/ps2.h"
#include "../arch/idt.h"
#include "../kernel/timer.h"

#define ENABLE_MOUSE_REPORTING 0xF4
#define MOUSE_CMD_SET_DEFAULTS     0xF6
#define MOUSE_CMD_SET_RESOLUTION   0xE8
#define MOUSE_ACK                  0xFA

#define PACKET_TIMEOUT_MS 20

#define PS2_CMD_READ_CONFIG   0x20
#define PS2_CMD_WRITE_CONFIG  0x60
#define PS2_CMD_ENABLE_AUX    0xA8

static void (*mouse_handler_user)(struct MouseEvent event);

static u8 packet[3];
static u8 packet_index = 0;
static u64 last_byte_tick = 0;

static void mouse_handler(void) {
    u8 data;

    if (!ps2_read_mouse(&data)) return;

    u64 now = timer_ticks();
    if (packet_index != 0 && (now - last_byte_tick) > PACKET_TIMEOUT_MS) {
        packet_index = 0;
    }
    last_byte_tick = now;

    // First byte must have 3 bit set
    if (packet_index == 0 && !(data & 0x08)) {
        return;
    }

    packet[packet_index++] = data;

    if (packet_index < 3) return;

    packet_index = 0;

    // Ignore packets with overflow
    if (packet[0] & 0xC0) return;

    struct MouseEvent event;

    event.left   = (packet[0] & 0x01) != 0;
    event.right  = (packet[0] & 0x02) != 0;
    event.middle = (packet[0] & 0x04) != 0;

    // Movement is a 9-bit two's complement value: bit 8 is the sign bit stored in packet[0]
    i16 dx = (i16) packet[1] - (i16) ((packet[0] << 4) & 0x100);
    i16 dy = (i16) packet[2] - (i16) ((packet[0] << 3) & 0x100);

    event.x_mov = dx;
    event.y_mov = -dy;      // up is positive

    if (mouse_handler_user != null) {
        mouse_handler_user(event);
    }
}

static inline u64 irq_save(void) {
    u64 flags;
    asm volatile("pushfq; popq %0; cli" : "=r"(flags) :: "memory");
    return flags;
}

static inline void irq_restore(u64 flags) {
    asm volatile("pushq %0; popfq" :: "r"(flags) : "memory", "cc");
}

static b8 mouse_send(u8 command, char* failure_message) {
    if (ps2_mouse_command(command) != MOUSE_ACK) {
        dbg_putln(failure_message);
        return false;
    }
    return true;
}

void mouse_init(void) {
    dbg_putln("Initializing Mouse");

    // The handler goes in before anything can possibly raise IRQ12
    idt_set_mouse_handler(mouse_handler);

    u64 flags = irq_save();

    ps2_write_command(PS2_CMD_READ_CONFIG);
    i16 config = ps2_read_data();

    if (config < 0) {
        irq_restore(flags);
        dbg_putln("PS/2 controller did not answer");
        return;
    }

    ps2_write_command(PS2_CMD_ENABLE_AUX);
    ps2_flush();

    b8 ok = mouse_send(MOUSE_CMD_SET_DEFAULTS, "Mouse defaults failed")
        && mouse_send(MOUSE_CMD_ENABLE_REPORTING, "Failed to enable mouse reporting");

    ps2_flush();
    packet_index = 0;

    u8 new_config = (u8) config & ~(1 << 5);
    if (ok) new_config |= (1 << 1);

    ps2_write_command(PS2_CMD_WRITE_CONFIG);
    ps2_write_data(new_config);

    irq_restore(flags);

    if (ok) dbg_putln("Mouse reporting enabled");
}

void mouse_set_handler(void (*handler)(struct MouseEvent event)) {
	mouse_handler_user = handler;	
}

b8 mouse_resolution(u8 resolution) {
    u64 flags = irq_save();
    b8 ok = ps2_mouse_command_arg(MOUSE_CMD_SET_RESOLUTION, (u8) resolution) == MOUSE_ACK;
    irq_restore(flags);
    return ok;
}