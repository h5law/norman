MAKEFLAGS += --no-builtin-rules --no-builtin-variables

LIB_DIR=lib
KERNEL_DIR=kern
BUILD_DIR=build
TEST_DIR=tests
INCLUDE_DIR=include

ARCH := $(shell uname -m)
ifeq ($(ARCH),arm64)
	ARCH = aarch64
else ifeq ($(ARCH),x86_64)
	ARCH = amd64
else
	$(error "Unsupported architecture: $(ARCH)")
endif

CC=/usr/local/bin/aarch64-none-elf-gcc
AS=/usr/local/bin/aarch64-none-elf-as
AR=/usr/local/bin/aarch64-none-elf-ar
LD=/usr/local/bin/aarch64-none-elf-ld

CFLAGS=-nostdinc -I${NORM_PATH}/include -fno-builtin -fno-pie
CFLAGS+= -ffreestanding -specs=${NORM_PATH}/nlibc.specs
LDFLAGS=-nostdlib -L${NORM_PATH}/lib -lnlibc

# CRT_PRE=$(LIB_DIR)/crt/build/crt0.o $(LIB_DIR)/crt/build/crti.o
# CRT_POST=$(LIB_DIR)/crt/build/crtn.o

.PHONY: clean build demo always

always:
	mkdir -pv $(LIB_DIR)
	mkdir -pv $(KERNEL_DIR)
	mkdir -pv $(TEST_DIR)
	mkdir -pv $(BUILD_DIR)
	mkdir -pv $(INCLUDE_DIR)

nlibc: ${NORM_PATH}/$(LIB_DIR)/libc
	make clean build -C ${NORM_PATH}/$(LIB_DIR)/libc

kernel: ${NORM_PATH}/$(KERNEL_DIR)
	make clean build -C ${NORM_PATH}/$(KERNEL_DIR)

crt: ${NORM_PATH}/$(LIB_DIR)/crt
	# make clean build -C $(LIB_DIR)/crt

$(BUILD_DIR)/demo.elf: $(TEST_DIR)/demo.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/demo.elf -c $(TEST_DIR)/demo.c

demo: always nlibc crt $(BUILD_DIR)/demo.elf

build: clean always nlibc crt demo kernel

clean: $(LIB_DIR)/libc $(LIB_DIR)/crt
	make clean -C ${NORM_PATH}/lib/libc
	make clean -C ${NORM_PATH}/lib/crt
	rm -vfrd $(BUILD_DIR)
