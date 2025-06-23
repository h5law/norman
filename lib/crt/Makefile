MAKEFLAGS += --no-builtin-rules --no-builtin-variables

ARCH := $(shell uname -m)
ifeq ($(ARCH),arm64)
	ARCH = aarch64
else ifeq ($(ARCH),x86_64)
	ARCH = amd64
else
	$(error "Unsupported architecture: $(ARCH)")
endif

SRC_DIR=$(ARCH)
BUILD_DIR=build

CFLAGS=-nostdinc -I${NORM_PATH}/include -fno-builtin -fno-pie
CFLAFS+= -ffreestanding -specs=${NORM_PATH}/nlibc.specs
LDFLAGS=-nostdlib -L${NORM_PATH}/lib -lnlibc

CRTI=$(SRC_DIR)/crti.c
CRTN=$(SRC_DIR)/crtn.c

CRTI_OBJ=$(BUILD_DIR)/crti.o
CRTN_OBJ=$(BUILD_DIR)/crtn.o

OBJ_LINK_LIST:=$(BUILD_DIR)/crt0.o $(CRTI_OBJ) $(CRTN_OBJ) $(BUILD_DIR)/block.o

.PHONY: check clean build

check: $(SRC_DIR)
	mkdir -pv $(BUILD_DIR)
	mkdir -pv $(SRC_DIR)

$(BUILD_DIR)/crt0.o: $(SRC_DIR)/crt0.c $(SRC_DIR)/crt0.S
	$(CC) -I$(SRC_DIR) $(CFLAGS) -o $(BUILD_DIR)/crt0_s.o -c $(SRC_DIR)/crt0.S
	$(CC) -I$(SRC_DIR) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/crt0_c.o -c $(SRC_DIR)/crt0.c
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/crt0.o -r $(BUILD_DIR)/crt0_s.o $(BUILD_DIR)/crt0_c.o

$(BUILD_DIR)/crti.o: $(CRTI)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(CRTI_OBJ) -c $(CRTI)

$(BUILD_DIR)/crtn.o: $(CRTN)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(CRTN_OBJ) -c $(CRTN)

$(BUILD_DIR)/block.o: $(SRC_DIR)/block.S
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/block.o -c $(SRC_DIR)/block.S

build: check $(OBJ_LINK_LIST)

clean:
	rm -vfrd $(BUILD_DIR)/
