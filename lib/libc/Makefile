MAKEFLAGS += --no-builtin-rules --no-builtin-variables

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

CFLAGS=-nostdinc -I${NORM_PATH}/include -fno-builtin -ffreestanding -fno-exceptions -fno-unwind-tables -ffunction-sections -fdata-sections -specs=${NORM_PATH}/nlibc.specs
LDFLAGS=-nostdlib -L${NORM_PATH}/lib -lnlibc

LIBRARY=${NORM_PATH}/lib/libnlibc.a
HEADERS=stdio stdlib string sys semihost unistd
ALL_ASM=${NORM_PATH}/lib/libc/sys/$(ARCH) ${NORM_PATH}/lib/libc/semihost/$(ARCH)
ASM=$(NORM_PATH)/lib/libc/sys/$(ARCH)
ifeq ($(BUILD_SEMIHOSTING),on)
	ASM+=$(NORM_PATH)/lib/libc/semihost/$(ARCH)
endif


SOURCES = $(foreach dir,$(HEADERS),$(wildcard $(dir)/*.c))
ASM_SOURCES = $(foreach dir,$(ASM),$(wildcard $(dir)/*.S))
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)

.PHONY: check clean clean_src build

check:
	mkdir -pv ${NORM_PATH}/lib/libc

build: check $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

${NORM_PATH}/lib/libc/sys/$(ARCH)/syscall.o: ${NORM_PATH}/lib/libc/sys/$(ARCH)/syscall.S
	# $(AS) -c $< -o $@
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

${NORM_PATH}/lib/libc/semihost/$(ARCH)/semihost.o: $(wildcard semihost/*.c) ${NORM_PATH}/lib/libc/semihost/$(ARCH)/semihost.S
	# $(AS) -c $< -o $@
	$(CC) $(CFLAGS) $(LDFLAGS) -c ${NORM_PATH}/lib/libc/semihost/$(ARCH)/semihost.S -o ${NORM_PATH}/lib/libc/semihost/$(ARCH)/semihost.o
	$(LD) -o ${NORM_PATH}/build/semihost.o -Ur ${NORM_PATH}/lib/libc/semihost/$(ARCH)/semihost.o $(wildcard semihost/*.o)

clean:
	rm -vfrd ${NORM_PATH}/lib/libc/build
	rm -vf $(OBJECTS) $(LIBRARY)

clean_src:
	rm -vf $(OBJECTS)

