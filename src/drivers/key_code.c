#include "key_code.h"

char key_to_ascii(u16 code, b8 shift, b8 caps) {
    switch (code) {
        case KEY_0: return shift ? ')' : '0';
        case KEY_1: return shift ? '!' : '1';
        case KEY_2: return shift ? '@' : '2';
        case KEY_3: return shift ? '#' : '3';
        case KEY_4: return shift ? '$' : '4';
        case KEY_5: return shift ? '%' : '5';
        case KEY_6: return shift ? '^' : '6';
        case KEY_7: return shift ? '&' : '7';
        case KEY_8: return shift ? '*' : '8';
        case KEY_9: return shift ? '(' : '9';
        case KEY_A: return (shift ^ caps) ? 'A' : 'a';
        case KEY_B: return (shift ^ caps) ? 'B' : 'b';
        case KEY_C: return (shift ^ caps) ? 'C' : 'c';
        case KEY_D: return (shift ^ caps) ? 'D' : 'd';
        case KEY_E: return (shift ^ caps) ? 'E' : 'e';
        case KEY_F: return (shift ^ caps) ? 'F' : 'f';
        case KEY_G: return (shift ^ caps) ? 'G' : 'g';
        case KEY_H: return (shift ^ caps) ? 'H' : 'h';
        case KEY_I: return (shift ^ caps) ? 'I' : 'i';
        case KEY_J: return (shift ^ caps) ? 'J' : 'j';
        case KEY_K: return (shift ^ caps) ? 'K' : 'k';
        case KEY_L: return (shift ^ caps) ? 'L' : 'l';
        case KEY_M: return (shift ^ caps) ? 'M' : 'm';
        case KEY_N: return (shift ^ caps) ? 'N' : 'n';
        case KEY_O: return (shift ^ caps) ? 'O' : 'o';
        case KEY_P: return (shift ^ caps) ? 'P' : 'p';
        case KEY_Q: return (shift ^ caps) ? 'Q' : 'q';
        case KEY_R: return (shift ^ caps) ? 'R' : 'r';
        case KEY_S: return (shift ^ caps) ? 'S' : 's';
        case KEY_T: return (shift ^ caps) ? 'T' : 't';
        case KEY_U: return (shift ^ caps) ? 'U' : 'u';
        case KEY_V: return (shift ^ caps) ? 'V' : 'v';
        case KEY_W: return (shift ^ caps) ? 'W' : 'w';
        case KEY_X: return (shift ^ caps) ? 'X' : 'x';
        case KEY_Y: return (shift ^ caps) ? 'Y' : 'y';
        case KEY_Z: return (shift ^ caps) ? 'Z' : 'z';
        case KEY_COMMA: return shift ? '<' : ',';
        case KEY_PERIOD: return shift ? '>' : '.';
        case KEY_SPACE: return ' ';
        case KEY_ENTER: return '\n';
        case KEY_BACKSPACE: return '\b';
        // case KEY_TAB: return '\t';
    }    
    
    return '?';
}