#ifndef KEY_CODE_H
#define KEY_CODE_H

#include "../util/numbers.h"

// PS/2 Keyboard Keys
#define KEY_0 0x0B
#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0A
#define KEY_A 0x1E
#define KEY_B 0x30
#define KEY_C 0x2E
#define KEY_D 0x20
#define KEY_E 0x12
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_I 0x17
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_M 0x32
#define KEY_N 0x31
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_Q 0x10
#define KEY_R 0x13
#define KEY_S 0x1F
#define KEY_T 0x14
#define KEY_U 0x16
#define KEY_V 0x2F
#define KEY_W 0x11
#define KEY_X 0x2D
#define KEY_Y 0x15
#define KEY_Z 0x2C
#define KEY_COMMA 0x33
#define KEY_PERIOD 0x34
#define KEY_DASH 0x0C
#define KEY_EQUAL 0x0D
#define KEY_LEFT_BRACKET 0x1A
#define KEY_RIGHT_BRACKET 0x1B
#define KEY_SLASH 0x35
#define KEY_BACK_SLASH 0x2B
#define KEY_SEMICOLON 0x27
#define KEY_QUOTE 0x28
#define KEY_BACK_TICK 0x29
#define KEY_SPACE 0x39
#define KEY_ENTER 0x1C
#define KEY_BACKSPACE 0x0E
#define KEY_TAB 0x0F
#define KEY_CAPS_LOCK 0x3A
#define KEY_LEFT_SHIFT 0x2A
#define KEY_RIGHT_SHIFT 0x36


char key_to_ascii(u16 code, b8 shift, b8 caps);

#endif