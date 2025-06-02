MAKEFLAGS += --no-builtin-rules --no-builtin-variables

ARCH=arm

CC=/usr/local/bin/arm-none-eabi-gcc -mcpu=cortex-m33 -mthumb -g
AS=/usr/local/bin/arm-none-eabi-as -mcpu=cortex-m33 -mthumb
AR=/usr/local/bin/arm-none-eabi-ar
LD=/usr/local/bin/arm-none-eabi-ld

CFLAGS=-O3 -nostdinc -I${NORM_PATH}/include -fno-builtin -ffreestanding -fno-exceptions -fno-unwind-tables -specs=${NORM_PATH}/nlibc.specs
LDFLAGS=-nostdlib -L${NORM_PATH}/lib -lnlibc

LIBRARY=${NORM_PATH}/lib/libnlibc.a
HEADERS=stdio stdlib string sys semihost
ASM=${NORM_PATH}/lib/libc/sys/$(ARCH) ${NORM_PATH}/lib/libc/semihost/$(ARCH)

SOURCES = $(foreach dir,$(HEADERS),$(wildcard $(dir)/*.c))
ASM_SOURCES = $(foreach dir,$(ASM),$(wildcard $(dir)/*.S))
OBJECTS = $(ASM_SOURCES:.S=.o) $(SOURCES:.c=.o)

.PHONY: check clean clean_src build

check:
	mkdir -pv ${NORM_PATH}/lib/libc

build: check $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

%.o: %.S
	# $(AS) -c $< -o $@
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -vf $(OBJECTS) $(LIBRARY)

clean_src:
	rm -vf $(OBJECTS)

