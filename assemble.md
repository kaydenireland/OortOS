## With GCC (Windows + MinGW64)

Assemble `boot.s`:
1. ```bash
   gcc -m32 -c src/boot/boot.s -o tmp/boot.o
   ```
2. ```bash
   ld -m i386pe -Ttext 0x7C00 tmp/boot.o -o tmp/boot.exe
   ```
3. ```bash
   objcopy -O binary -j .text tmp/boot.exe bin/boot.bin
   ```

Verify the boot sector:
1. ```bash
   wc -c bin/boot.bin
   ```
2. Confirm the output is `512` bytes and the final two bytes are `55 AA`.

Run with QEMU:
1. ```bash
   qemu -drive format=raw,file=bin/boot.bin
   ```