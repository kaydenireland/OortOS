CC      = gcc
AS      = gcc
LD      = ld
OBJCOPY = objcopy
CP      = cp
DD      = dd

BOOT_LDFLAGS   = -m elf_i386 -Ttext 0x7C00
STAGE2_LDFLAGS = -m elf_x86_64 -Ttext 0x9000

ASFLAGS_32   = -m32

ASFLAGS_64   = -m64 -Wa,--64
KERNEL_FLAGS = -m64 -ffreestanding -mno-red-zone -O2 -Wall -Wextra -mno-red-zone

BOOT_DIR   = src/boot
UTIL_DIR  = src/util
KERN_DIR  = src/kernel
ARCH_DIR  = src/arch
DRIVE_DIR = src/drivers
TMP_DIR   = tmp
BIN_DIR   = bin

BOOT_SRC  = $(BOOT_DIR)/real.s
BOOT_OBJ  = $(TMP_DIR)/boot.o
BOOT_EXE  = $(TMP_DIR)/boot.elf
BOOT_BIN  = $(BIN_DIR)/boot.bin

PROT_SRC  = $(BOOT_DIR)/protected.s
PROT_BIN  = $(BIN_DIR)/prot.bin
PROT_EXE  = $(TMP_DIR)/prot.elf

STAGE2_OBJS = $(TMP_DIR)/prot.o \
              $(TMP_DIR)/kernel.o \
              $(TMP_DIR)/debug.o \
              $(TMP_DIR)/stdio.o \
              $(TMP_DIR)/port.o \
              $(TMP_DIR)/port_s.o \
              $(TMP_DIR)/rtc.o \
              $(TMP_DIR)/idt.o \
              $(TMP_DIR)/idt_s.o \
              $(TMP_DIR)/keyboard.o \
              $(TMP_DIR)/key_code.o \
              $(TMP_DIR)/pic.o \
              $(TMP_DIR)/ps2.o \
              $(TMP_DIR)/pit.o

OORT_IMG = $(BIN_DIR)/oort.img


# Dynamically calculate the number of sectors that need to be loaded to the disk
KERNEL_SECTORS_INC = $(TMP_DIR)/kernel_sectors.inc

$(KERNEL_SECTORS_INC): $(PROT_BIN) | $(TMP_DIR)
	@bytes=$$(wc -c < $(PROT_BIN)); \
	sectors=$$(( (bytes + 511) / 512 )); \
	echo ".equ KERNEL_SECTORS, $$sectors" > $@; \
	echo "Kernel size: $$bytes bytes ($$sectors sectors)"

.PHONY: all boot clean

all: boot

boot: $(OORT_IMG)

$(TMP_DIR):
	mkdir -p $(TMP_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Stage 1: BIOS boot sector
$(BOOT_OBJ): $(BOOT_SRC) $(KERNEL_SECTORS_INC) | $(TMP_DIR)
	$(AS) $(ASFLAGS_32) -c $(BOOT_SRC) -o $(BOOT_OBJ)

$(BOOT_EXE): $(BOOT_OBJ)
	$(LD) $(BOOT_LDFLAGS) $(BOOT_OBJ) -o $(BOOT_EXE)

$(BOOT_BIN): $(BOOT_EXE) | $(BIN_DIR)
	$(OBJCOPY) -O binary $(BOOT_EXE) $(BOOT_BIN)


# Stage 2 - ASM
$(TMP_DIR)/prot.o: $(PROT_SRC) | $(TMP_DIR)
	$(AS) $(ASFLAGS_64) -c $< -o $@

$(TMP_DIR)/port_s.o: $(ARCH_DIR)/port_s.s | $(TMP_DIR)
	$(AS) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/idt_s.o: $(ARCH_DIR)/idt_s.s | $(TMP_DIR)
	$(AS) $(KERNEL_FLAGS) -c $< -o $@

# Stage 2 - C

$(TMP_DIR)/kernel.o: $(KERN_DIR)/kernel.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/debug.o: $(UTIL_DIR)/debug.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/stdio.o: $(KERN_DIR)/stdio.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/port.o: $(ARCH_DIR)/port.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/rtc.o: $(DRIVE_DIR)/rtc.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/idt.o: $(ARCH_DIR)/idt.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/keyboard.o: $(DRIVE_DIR)/keyboard.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/key_code.o: $(DRIVE_DIR)/key_code.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/pic.o: $(DRIVE_DIR)/pic.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/ps2.o: $(DRIVE_DIR)/ps2.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@

$(TMP_DIR)/pit.o: $(DRIVE_DIR)/pit.c | $(TMP_DIR)
	$(CC) $(KERNEL_FLAGS) -c $< -o $@


# Link Stage 2 using 64-bit ELF engine
$(PROT_EXE): $(STAGE2_OBJS)
	$(LD) $(STAGE2_LDFLAGS) $(STAGE2_OBJS) -o $(PROT_EXE)

# Extract binary stream for Stage 2
$(PROT_BIN): $(PROT_EXE) | $(BIN_DIR)
	$(OBJCOPY) -O binary $(PROT_EXE) $(PROT_BIN)

# Assemble flat disk block sequence
$(OORT_IMG): $(BOOT_BIN) $(PROT_BIN) | $(BIN_DIR)
	$(CP) $(BOOT_BIN) $(OORT_IMG)
	$(DD) if=/dev/zero bs=512 \
		count=$$(( ($$(wc -c < $(PROT_BIN)) + 511) / 512 )) \
		>> $(OORT_IMG)
	$(DD) if=$(PROT_BIN) of=$(OORT_IMG) bs=512 seek=1 conv=notrunc

clean:
	rm -rf $(TMP_DIR) $(BIN_DIR)

size: $(OORT_IMG)
	wc -c $(BOOT_BIN)
	wc -c $(PROT_BIN)
	wc -c $(OORT_IMG)

run: $(OORT_IMG)
	qemu-system-x86_64 -cpu qemu64 -debugcon file:log/debug.txt -drive format=raw,file=$(OORT_IMG) -D log/qemu.log -d cpu_reset,guest_errors,unimp