Assemble ```boot.s```:
- ```gcc -m32 -c boot.s -o boot.o```
- ```objcopy -O binary -j .text boot.o boot.bin```