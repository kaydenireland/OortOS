#include "port.h"

#define RTC_COMMAND_PORT 0x70
#define RTC_DATA_PORT 0x71
#define RTC_REGISTER_SECONDS 0x00
#define RTC_REGISTER_STATUS_A 0x0A
#define RTC_REGISTER_STATUS_B 0x0B
#define RTC_UPDATE_IN_PROGRESS 0x80
#define RTC_DATA_MODE 0x04

u8 rtc_read_register(u8 reg) {
    port_outb(RTC_COMMAND_PORT, reg);
    return port_inb(RTC_DATA_PORT);
}

void rtc_wait(void) {
    while(rtc_read_register(RTC_REGISTER_STATUS_A) &RTC_UPDATE_IN_PROGRESS) { }
}

u8 rtc_is_bcd(void) {
    return !(rtc_read_register(RTC_REGISTER_STATUS_B) &RTC_DATA_MODE);
} 

u8 rtc_seconds(void) {
    // Possible corruption can occur in the data if hardware is updating the seconds while we read it

    u8 seconds_a;
    u8 seconds_b;
    b8 is_bcd = rtc_is_bcd();

    do {
        rtc_wait();
        seconds_a = rtc_read_register(RTC_REGISTER_SECONDS);
        seconds_b = rtc_read_register(RTC_REGISTER_SECONDS);
    } while (seconds_a != seconds_b);

    if (is_bcd) {
        return (seconds_b & 0x0F) + ((seconds_b & 0xF0) >> 4) * 10;
    }

    return seconds_b;
}