AS      = gcc
LD      = ld
OBJCOPY = objcopy

ASFLAGS = -m32
LDFLAGS = -m i386pe -Ttext 0x7C00

SRC_DIR = src/boot
TMP_DIR = tmp
BIN_DIR = bin

BOOT_SRC = $(SRC_DIR)/real.s
BOOT_OBJ = $(TMP_DIR)/boot.o
BOOT_EXE = $(TMP_DIR)/boot.exe
BOOT_BIN = $(BIN_DIR)/boot.bin

.PHONY: all boot clean run

all: boot

boot: $(BOOT_BIN)

$(BOOT_OBJ): $(BOOT_SRC) $(wildcard $(SRC_DIR)/*.s)
	mkdir -p $(TMP_DIR)
	$(AS) $(ASFLAGS) -c $(BOOT_SRC) -o $(BOOT_OBJ)

$(BOOT_EXE): $(BOOT_OBJ)
	$(LD) $(LDFLAGS) $(BOOT_OBJ) -o $(BOOT_EXE)

$(BOOT_BIN): $(BOOT_EXE)
	mkdir -p $(BIN_DIR)
	$(OBJCOPY) -O binary -j .text $(BOOT_EXE) $(BOOT_BIN)
	test "$$(wc -c < $(BOOT_BIN))" -eq 512 || \
		(echo "Error: boot sector must be exactly 512 bytes"; exit 1)
	printf '\125\252' | cmp - <(tail -c 2 $(BOOT_BIN)) || \
		(echo "Error: invalid boot signature"; exit 1)

clean:
	rm -rf $(TMP_DIR) $(BIN_DIR)

run: boot
	qemu-system-i386 -drive format=raw,file=$(BOOT_BIN)