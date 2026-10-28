## Using Docker
1. Ensure Docker is installed and the engine is running
2. Create the image container:
```bash
docker build --no-cache -t oort-builder .
```
3. Compile the OS:
```bash
docker run --rm -v ${PWD}:/os oort-builder make clean boot
```
4. Run using Make and QEMU:
```bash
make run
```
or run directly with QEMU
```bash
qemu-system-x86_64 -cpu qemu64 -drive format=raw,file=bin/oort.img
```