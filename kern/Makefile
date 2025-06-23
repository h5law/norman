MAKEFLAGS += --no-builtin-rules --no-builtin-variables

ARCH := $(shell uname -m)
ifeq ($(ARCH),arm64)
	ARCH = aarch64
else ifeq ($(ARCH),x86_64)
	ARCH = amd64
else
	$(error "Unsupported architecture: $(ARCH)")
endif


CFLAGS=-I${NORM_PATH}/kern/include -I${NORM_PATH}/kern/include/hw -I${NORM_PATH}/include
CFLAGS+= -ffreestanding -fno-builtin -nostdinc -nostartfiles -fno-pie
LDFLAGS=-L${NORM_PATH}/lib -lnlibc -Tqemu.ld
LDFLAGS+= -nostdlib

BUILD_DIR=build
BOOT_DIR=boot/$(ARCH)
KERNEL_DIR=lib

SRC=lib
SOURCES = $(foreach dir,$(SRC),$(wildcard $(dir)/*.c))
OBJECTS = $(BOOT_DIR)/boot.o $(SOURCES:.c=.o)

.PHONY: check

check:
	mkdir -pv $(BUILD_DIR) $(BOOT_DIR) $(KERNEL_DIR)

clean:
	rm -vf $(OBJECTS)
	rm -vfrd $(BUILD_DIR)

$(BOOT_DIR)/boot.o: $(BOOT_DIR)/boot.S
	$(AS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(BUILD_DIR)/qemu.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) -L/Users/harry/Dev/projects/norman/lib -lnlibc -Wl,-Map=$(BUILD_DIR)/qemu.map

build: check $(BUILD_DIR)/qemu.elf

qemu: check $(BUILD_DIR)/qemu.elf
	qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel $(BUILD_DIR)/qemu.elf -nographic -serial mon:stdio
