CC=clang
CFLAGS=-xc -std=c99 -Wall -Werror -Wpedantic

TEST_DIR=tests
TEST_BINS=dyn mem calls

.PHONY: always all clean test run_tests

all: clean test run_tests

always:
	mkdir -pv $(TEST_DIR)

clean: always
	rm -rfv $(TEST_DIR)/*.o $(addprefix $(TEST_DIR)/, $(TEST_BINS))

test: always clean $(addprefix $(TEST_DIR)/, $(TEST_BINS))

$(TEST_DIR)/dyn:
	$(CC) $(CFLAGS) -lxxhash -L/opt/homebrew/opt/xxhash/lib -I/opt/homebrew/opt/xxhash/include -o $(TEST_DIR)/dyn $(TEST_DIR)/dyn_simple.c

$(TEST_DIR)/mem:
	$(CC) $(CFLAGS) -o $(TEST_DIR)/mem $(TEST_DIR)/mem_simple.c

run_tests: test
	$(addprefix $(TEST_DIR)/, $(TEST_BINS))

syscall_demo: clean
	as -o $(TEST_DIR)/calls.o -c src/syscall-arm64.S
	$(CC) -nostdinc -ffreestanding -fno-builtin -ffunction-sections -fdata-sections -O0 -c src/system.c -o $(TEST_DIR)/system.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -ffunction-sections -fdata-sections -O0 -c src/errno.c -o $(TEST_DIR)/errno.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -ffunction-sections -fdata-sections -O0 -c src/stdio.c -o $(TEST_DIR)/stdio.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -ffunction-sections -fdata-sections -O0 -c $(TEST_DIR)/syscalls.c -o $(TEST_DIR)/syscalls.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -ffunction-sections -fdata-sections -O0 -o $(TEST_DIR)/calls $(TEST_DIR)/calls.o $(TEST_DIR)/system.o $(TEST_DIR)/errno.o $(TEST_DIR)/stdio.o $(TEST_DIR)/syscalls.o
	# ld -o $(TEST_DIR)/calls $(TEST_DIR)/calls.o $(TEST_DIR)/system.o $(TEST_DIR)/errno.o $(TEST_DIR)/stdio.o $(TEST_DIR)/syscalls.o
	./$(TEST_DIR)/calls $(TEST_DIR)/syscalls.c

