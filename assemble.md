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
   gcc -m32 -c src/boot/protected.s -o tmp/prot.o
   ```
2. ```bash
   ld -m i386pe -Ttext 0x7C00 tmp/boot.o -o tmp/boot.exe
   ld -m i386pe -Ttext 0x9000 tmp/prot.o -o tmp/prot.exe
   ```
3. ```bash
   objcopy -O binary -j .text tmp/boot.exe bin/boot.bin
   objcopy -O binary -j .text tmp/prot.exe bin/prot.bin
   ```
4. ```bash
   cp bin/boot.bin bin/oort.img
   ```
5. ```bash
   dd if=/dev/zero bs=512 count=16 >> bin/oortos.img
   ```
6. ```bash
   dd if=bin/prot.bin of=bin/oortos.img bs=512 seek=1 conv=notrunc
   ```

Verify the boot sector:
1. ```bash
   wc -c bin/boot.bin
   ```
2. Confirm the output is `512` bytes and the final two bytes are `55 AA`.

Verify prot sector:
1. ```bash
   wc -c bin/prot.bin
   ```
2. Confirm the output is `8704` bytes

Verify image:
1. ```bash
   wc -c bin/oort.img
   ```
2. Confirm the output is `8192` bytes

Run with QEMU:
1. ```bash
   qemu-system-i386 -cpu qemu64 -drive format=raw,file=bin/boot.bin
   ```