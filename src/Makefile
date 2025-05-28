MAKEFLAGS += --no-builtin-rules --no-builtin-variables
unexport LIBRARY_PATH C_INCLUDE_PATH

ARCH=$(shell uname -m | sed 's/arm64/aarch64/')

INCLUDES=${NORM_PATH}/include
LIB=${NORM_PATH}/lib

LIBRARY=$(LIB)/libnlibc.a
HEADERS=stdio stdlib string sys
ASM=sys/$(ARCH)

SOURCES = $(foreach dir,$(HEADERS),$(wildcard $(dir)/*.c))
ASM_SOURCES = $(wildcard $(ASM)/*.S)
OBJECTS = $(ASM_SOURCES:.S=.o) $(SOURCES:.c=.o)

.PHONY: check clean build

check:
	mkdir -pv $(LIB)

build: check $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -vf $(OBJECTS) $(LIBRARY)

clean_src:
	rm -vf $(OBJECTS)

