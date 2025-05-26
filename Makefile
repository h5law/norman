MAKEFLAGS += --no-builtin-rules --no-builtin-variables
unexport CFLAGS LDFLAGS LIBRARY_PATH C_INCLUDE_PATH

CC=clang
AS=as
LD=ld.lld
CFLAGS=-nostdinc -fno-builtin -ffreestanding -fno-stack-protector
LDFLAGS=-nostdlib

SRC_DIR=src
TEST_DIR=tests
BUILD_DIR=build
INCLUDE_DIR=include
LIB_DIR=lib

ARCH := $(shell uname -m)
ifneq ($(ARCH),arm64)
ifneq ($(ARCH),aarch64)
    $(error Unsupported architecture: $(ARCH))
endif
endif

CRT_PRE=$(SRC_DIR)/crt/build/crt0.o $(SRC_DIR)/crt/build/crti.o $(SRC_DIR)/crt/build/crtbegin.o
CRT_POST=$(SRC_DIR)/crt/build/crtend.o $(SRC_DIR)/crt/build/crtn.o

.PHONY: clean build demo

always:
	mkdir -pv $(SRC_DIR)
	mkdir -pv $(TEST_DIR)
	mkdir -pv $(BUILD_DIR)
	mkdir -pv $(INCLUDE_DIR)
	mkdir -pv $(LIB_DIR)

nlibc: ${NORM_PATH}/$(SRC_DIR)
	make clean build clean_src -C ${NORM_PATH}/$(SRC_DIR)

crt: ${NORM_PATH}/$(SRC_DIR)/crt
	make clean build -C ${NORM_PATH}/$(SRC_DIR)/crt

$(BUILD_DIR)/demo: always nlibc crt $(TEST_DIR)/syscalls.c
	$(CC) -I${NORM_PATH}/$(INCLUDE_DIR) -L${NORM_PATH}/$(LIB_DIR) -lnlibc -o $(BUILD_DIR)/demo $(CRT_PRE) $(TEST_DIR)/syscalls.c $(CRT_POST)

build: clean always nlibc crt $(BUILD_DIR)/demo

clean: ${NORM_PATH}/$(SRC_DIR) ${NORM_PATH}/$(SRC_DIR)/crt
	make clean -C ${NORM_PATH}/$(SRC_DIR)
	make clean -C ${NORM_PATH}/$(SRC_DIR)/crt
	rm -vfrd $(BUILD_DIR)
