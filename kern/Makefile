MAKEFLAGS += --no-builtin-rules --no-builtin-variables

CC=/usr/local/bin/aarch64-none-elf-gcc -mcpu=cortex-a53 -march=armv8-a+crc
AS=/usr/local/bin/aarch64-none-elf-as -mcpu=cortex-a53 -march=armv8-a+crc
LD=/usr/local/bin/aarch64-none-elf-ld

CFLAGS=-ffreestanding -fno-builtin -nostdinc -fno-stack-protector -I${NORM_PATH}/kern/include
LDFLAGS=-nostdlib -L${NORM_PATH}/lib -lnlibc -Tqemu.ld

BUILD_DIR=build
BOOT_DIR=boot
KERNEL_DIR=core

.PHONY: check

check:
	mkdir -pv $(BUILD_DIR) $(BOOT_DIR) $(KERNEL_DIR)

clean:
	rm -vfrd $(BUILD_DIR)

$(BUILD_DIR)/boot.o: $(BOOT_DIR)/boot.S
	$(AS) -o $@ $<

$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kstring.o: $(KERNEL_DIR)/kstring.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/qemu.elf: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/kstring.o
	$(LD) $(LDFLAGS) -o $@ $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/kstring.o

qemu: check $(BUILD_DIR)/qemu.elf
	qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel $(BUILD_DIR)/qemu.elf -nographic -serial mon:stdio -d int
