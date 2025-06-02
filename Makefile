MAKEFLAGS += --no-builtin-rules --no-builtin-variables

LIB_DIR=lib
BUILD_DIR=build
TEST_DIR=tests
INCLUDE_DIR=include

ARCH=arm

CC=/usr/local/bin/arm-none-eabi-gcc -mcpu=cortex-m33 -mthumb -g
AS=/usr/local/bin/arm-none-eabi-as -mcpu=cortex-m33 -mthumb
AR=/usr/local/bin/arm-none-eabi-ar
LD=/usr/local/bin/arm-none-eabi-ld

CFLAGS=-O3 -nostdinc -I${NORM_PATH}/include -fno-builtin -ffreestanding -fno-exceptions -fno-unwind-tables -specs=${NORM_PATH}/nlibc.specs
LDFLAGS=-nostdlib -L${NORM_PATH}/lib -lnlibc

CRT_PRE=$(LIB_DIR)/crt/build/crt0.o
#$(LIB_DIR)/crt/build/crti.o
# CRT_POST=$(LIB_DIR)/crt/build/crtn.o

.PHONY: clean build demo always

always:
	mkdir -pv $(LIB_DIR)
	mkdir -pv $(TEST_DIR)
	mkdir -pv $(BUILD_DIR)
	mkdir -pv $(INCLUDE_DIR)

nlibc: ${NORM_PATH}/$(LIB_DIR)/libc
	make clean build -C ${NORM_PATH}/$(LIB_DIR)/libc

crt: ${NORM_PATH}/$(LIB_DIR)/crt
	make clean build -C $(LIB_DIR)/crt

$(BUILD_DIR)/demo.o: $(TEST_DIR)/demo.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/demo.o -c $(TEST_DIR)/demo.c

demo: always nlibc crt $(BUILD_DIR)/demo.o $(LIB_DIR)/libc/sys/$(ARCH)/syscall.o
	$(CC) $(CFLAGS) -DCRT0_SEMIHOST $(LFLAGS) -nostartfiles -o $(BUILD_DIR)/demo.elf -Ur $(CRT_PRE) $(BUILD_DIR)/demo.o $(LIB_DIR)/libc/sys/$(ARCH)/syscall.o
	#\$(CRT_POST)

build: clean always nlibc crt demo

clean: $(LIB_DIR)/libc $(LIB_DIR)/crt
	make clean -C ${NORM_PATH}/lib/libc
	make clean -C ${NORM_PATH}/lib/crt
	rm -vfrd $(BUILD_DIR)
