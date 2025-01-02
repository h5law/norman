CC=clang
CFLAGS=-xc -std=c99 -Wall -Werror -Wpedantic -Wintrinsics-std

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
	$(CC) $(CFLAGS) -lxxhash -o $(TEST_DIR)/dyn_test.o $(TEST_DIR)/dyn_simple.c

$(TEST_DIR)/mem:
	$(CC) $(CFLAGS) -o $(TEST_DIR)/mem_test.o $(TEST_DIR)/mem_simple.c

run_tests: test
	$(addprefix ./tests/, $(TEST_BINS:=_test.o; ))
