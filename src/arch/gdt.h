#ifndef GDT_H
#define GDT_H

#define GDT_RING_0 0 // kernel
#define GDT_RING_1 1 // legacy
#define GDT_RING_2 2 // legacy
#define GDT_RING_3 3 // user space

#define GDT_SELECTOR_INDEX 3

#define GDT_SELECTOR_CS_KERNEL (GDT_SELECTOR_INDEX << 3) | GDT_RING_0

#endif