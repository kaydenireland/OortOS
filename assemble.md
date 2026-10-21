## With Make
- ```make clean``` to clean temporary files
- ```make boot``` to compile src into bin
- ```make run``` to compile src and run with qemu
- ```make all``` to run every make command

Note: Make only compiles files that have changed since last compilation, unless the tmp/bin files have been deleted.


## With GCC (Windows + MinGW64)

Assemble bootloader:
1. ```bash
   gcc -m32 -c src/boot/real.s -o tmp/boot.o
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
   qemu-system-i386 -drive format=raw,file=bin/boot.bin
   ```