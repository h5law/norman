CC=clang
CFLAGS=-xc -std=c99 -Wall -Werror -Wpedantic

TEST_DIR=tests
TEST_BINS=dyn mem

.PHONY: always all clean test run_tests

all: clean test run_tests

always:
	mkdir -pv $(TEST_DIR)

clean: always
	rm -rf $(TEST_DIR)/*.o

test: always clean $(addprefix $(TEST_DIR)/, $(TEST_BINS))

$(TEST_DIR)/dyn:
	$(CC) $(CFLAGS) -lxxhash -L/opt/homebrew/opt/xxhash/lib -I/opt/homebrew/opt/xxhash/include -o $(TEST_DIR)/dyn_test.o $(TEST_DIR)/dyn_simple.c

$(TEST_DIR)/mem:
	$(CC) $(CFLAGS) -o $(TEST_DIR)/mem_test.o $(TEST_DIR)/mem_simple.c

run_tests: test
	$(addprefix ./tests/, $(TEST_BINS:=_test.o; ))

syscall_demo: clean
	$(CC) -nostdinc -ffreestanding -fno-builtin -O0 -c src/syscall-arm64.S -o $(TEST_DIR)/calls.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -O0 -c src/errno.c -o $(TEST_DIR)/errno.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -O0 -c src/stdio.c -o $(TEST_DIR)/stdio.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -O0 -c $(TEST_DIR)/syscalls.c -o $(TEST_DIR)/syscalls.o
	$(CC) -nostdinc -ffreestanding -fno-builtin -O0 -o calls $(TEST_DIR)/calls.o $(TEST_DIR)/errno.o $(TEST_DIR)/stdio.o $(TEST_DIR)/syscalls.o
