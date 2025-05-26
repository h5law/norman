CC=clang
CFLAGS=-nostdinc -fno-builtin -ffreestanding -fno-stack-protector
LDFLAGS=-nostdlib

ARCH=$(shell uname -m | sed 's/arm64/aarch64/')
SRC_DIR=$(ARCH)
CMN_DIR=common
BUILD_DIR=build

CRTI=$(SRC_DIR)/crti.c
CRTBEGIN=$(CMN_DIR)/crtbegin.c
CRTEND=$(CMN_DIR)/crtend.c
CRTN=$(SRC_DIR)/crtn.c

CRTI_OBJ=$(BUILD_DIR)/crti.o
CRTBEGIN_OBJ=$(BUILD_DIR)/crtbegin.o
CRTEND_OBJ=$(BUILD_DIR)/crtend.o
CRTN_OBJ=$(BUILD_DIR)/crtn.o

OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)

.PHONY: check clean build

check: $(SRC_DIR) $(CMN_DIR)
	[ -d $(SRC_DIR) ] || (echo "Unsuppported architecture" && exit 1)
	[ -d $(CMN_DIR) ] || (echo "Missing common" && exit 1)
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/crt0.o: $(SRC_DIR)/crt0.S $(SRC_DIR)/crt0.c
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(CMN_DIR) -I${NORM_PATH}/include -o $(BUILD_DIR)/crt0.o -r $(SRC_DIR)/crt0.S $(SRC_DIR)/crt0.c

$(BUILD_DIR)/crti.o: $(CRTI)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/crti.o -c $(CRTI)

$(BUILD_DIR)/crtbegin.o: $(CRTBEGIN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/crtbegin.o -c $(CRTBEGIN)

$(BUILD_DIR)/crtend.o: $(CRTEND)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/crtend.o -c $(CRTEND)

$(BUILD_DIR)/crtn.o: $(CRTN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/crtn.o -c $(CRTN)

build: check $(OBJ_LINK_LIST) $(BUILD_DIR)/crt0.o

clean:
	rm -vfrd $(BUILD_DIR)/
