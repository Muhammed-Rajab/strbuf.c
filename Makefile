CC 			:= gcc
CFLAGS 	:= -Wall -Wextra -Werror -Iinclude -O0 -g -fsanitize=address
LDFLAGS :=
LDLIBS 	:=

SRC_DIR 	:= src
BUILD_DIR := build
TESTS_DIR := tests

all: $(BUILD_DIR) test

test: test_strbuf
	$(BUILD_DIR)/test_strbuf

test_strbuf: $(BUILD_DIR)/strbuf.o $(BUILD_DIR)/test_strbuf.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

$(BUILD_DIR)/strbuf.o: $(SRC_DIR)/strbuf.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_strbuf.o: $(TESTS_DIR)/test_strbuf.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf build/

.PHONY: all clean test
