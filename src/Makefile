AR=llvm-ar
CC=clang
CFLAGS=-nostdinc -fno-builtin -ffreestanding -fno-stack-protector
LDFLAGS=-nostdlib

ARCH=$(shell uname -m | sed 's/arm64/aarch64/')

INCLUDES=${NORM_PATH}/include
LIB=${NORM_PATH}/lib

LIBRARY=$(LIB)/libnlibc.a
HEADERS=stdio stdlib string sys
ASM=sys/$(ARCH)

SOURCES = $(foreach dir,$(HEADERS),$(wildcard $(dir)/*.c))
ASM_SOURCES = $(wildcard $(ASM)/*.S)
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

.PHONY: check clean build

check:
	mkdir -pv $(LIB)

build: check $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -vf $(OBJECTS) $(LIBRARY)

clean_src:
	rm -vf $(OBJECTS)

