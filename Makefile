
AS      = gcc
LD      = ld
OBJCOPY = objcopy
CP      = cp
DD      = dd

ASFLAGS  = -m32
LDFLAGS  = -m i386pe -Ttext
DD1_FLAGS = if=/dev/zero bs=512 count=40
DD2_FLAGS = bs=512 seek=1 conv=notrunc

SRC_DIR = src/boot
TMP_DIR = tmp
BIN_DIR = bin

BOOT_SRC = $(SRC_DIR)/real.s
BOOT_OBJ = $(TMP_DIR)/boot.o
BOOT_EXE = $(TMP_DIR)/boot.exe
BOOT_BIN = $(BIN_DIR)/boot.bin

PROT_SRC = $(SRC_DIR)/protected.s
PROT_OBJ = $(TMP_DIR)/prot.o
PROT_EXE = $(TMP_DIR)/prot.exe
PROT_BIN = $(BIN_DIR)/prot.bin

OORT_IMG = $(BIN_DIR)/oort.img

.PHONY: all boot clean size run

all: boot

# Build the complete disk image.
boot: $(OORT_IMG)

$(TMP_DIR):
	mkdir -p $(TMP_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Stage 1: BIOS boot sector.
$(BOOT_OBJ): $(BOOT_SRC) $(wildcard $(SRC_DIR)/*.s) | $(TMP_DIR)
	$(AS) $(ASFLAGS) -c $(BOOT_SRC) -o $(BOOT_OBJ)

$(BOOT_EXE): $(BOOT_OBJ)
	$(LD) $(LDFLAGS) 0x7C00 $(BOOT_OBJ) -o $(BOOT_EXE)

$(BOOT_BIN): $(BOOT_EXE) | $(BIN_DIR)
	$(OBJCOPY) -O binary -j .text $(BOOT_EXE) $(BOOT_BIN)
	test "$$(wc -c < $(BOOT_BIN))" -eq 512 || \
		(echo "Error: boot sector must be exactly 512 bytes"; exit 1)
	printf '\125\252' | cmp - <(tail -c 2 $(BOOT_BIN)) || \
		(echo "Error: invalid boot signature"; exit 1)

# Stage 2: protected-mode code.
$(PROT_OBJ): $(PROT_SRC) $(wildcard $(SRC_DIR)/*.s) | $(TMP_DIR)
	$(AS) $(ASFLAGS) -c $(PROT_SRC) -o $(PROT_OBJ)

$(PROT_EXE): $(PROT_OBJ)
	$(LD) $(LDFLAGS) 0x9000 $(PROT_OBJ) -o $(PROT_EXE)

$(PROT_BIN): $(PROT_EXE) | $(BIN_DIR)
	$(OBJCOPY) -O binary -j .text $(PROT_EXE) $(PROT_BIN)
	test "$$(wc -c < $(PROT_BIN))" -le 20480 || \
		(echo "Error: Stage 2 must be 20,480 bytes or smaller"; exit 1)

# Disk image: boot sector + 16 reserved sectors + Stage 2.
$(OORT_IMG): $(BOOT_BIN) $(PROT_BIN) | $(BIN_DIR)
	$(CP) $(BOOT_BIN) $(OORT_IMG)
	$(DD) $(DD1_FLAGS) >> $(OORT_IMG)
	$(DD) if=$(PROT_BIN) of=$(OORT_IMG) $(DD2_FLAGS)

clean:
	rm -rf $(TMP_DIR) $(BIN_DIR)

size: $(OORT_IMG)
	wc -c $(BOOT_BIN)
	wc -c $(PROT_BIN)
	wc -c $(OORT_IMG)

run: $(OORT_IMG)
	qemu-system-x86_64 -cpu qemu64 -drive format=raw,file=$(OORT_IMG)
