MAKEFLAGS += --no-builtin-rules --no-builtin-variables
unexport CFLAGS LDFLAGS LIBRARY_PATH C_INCLUDE_PATH

CC=clang
CFLAGS=-nostdinc -fno-builtin -fno-stack-protector

SRC_DIR=src
TEST_DIR=tests
BUILD_DIR=build

ARCH := $(shell uname -m)
ifneq ($(ARCH),arm64)
ifneq ($(ARCH),aarch64)
    $(error Unsupported architecture: $(ARCH))
endif
endif

CRT := syscall-arm64
SRC_FILES  := system.c errno.c stdio.c mem.c string.c
TEST_FILE  := syscalls.c
SRC_OBJS   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
TEST_OBJ   := $(patsubst %.c,$(BUILD_DIR)/%.o,$(TEST_FILE))
ALL_OBJS   := $(BUILD_DIR)/$(CRT).o $(SRC_OBJS) $(TEST_OBJ)
TARGET     := $(BUILD_DIR)/demo

.PHONY: always clean build demo

always:
	mkdir -pv $(SRC_DIR)
	mkdir -pv $(TEST_DIR)
	mkdir -pv $(BUILD_DIR)

clean: always
	rm -rfv $(ALL_OBJS) $(TARGET)

$(BUILD_DIR)/$(CRT).o: $(SRC_DIR)/$(CRT).S
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(CRT).o -c $(SRC_DIR)/$(CRT).S

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(TEST_DIR)/$(TEST_FILE)
	$(CC) $(CFLAGS) -c $(TEST_DIR)/$(TEST_FILE) -o $(TEST_OBJ)

build: clean $(ALL_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(ALL_OBJS)

demo: $(TARGET)
	./$(TARGET) $(SRC_DIR)/system.h
