## With GCC (Windows + MinGW64)

Assemble `boot.s`:
1. ```bash
   gcc -m32 -c boot.s -o boot.o
   ```
2. ```bash
   ld -m i386pe -Ttext 0x7C00 boot.o -o boot.exe
   ```
3. ```bash
   objcopy -O binary -j .text boot.exe boot.bin
   ```

Verify the boot sector:
1. ```bash
   wc -c boot.bin
   ```
2. Confirm the output is `512` bytes and the final two bytes are `55 AA`.

Run with QEMU:
1. ```bash
   qemu-system-i386 -drive format=raw,file=boot.bin
   ```