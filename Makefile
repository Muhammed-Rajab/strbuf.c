CC 			:= gcc
CFLAGS 	:= -Wall -Wextra -Werror -Iinclude -O0 -g -fsanitize=address
LDFLAGS :=
LDLIBS 	:=

SRC_DIR 	:= src
BUILD_DIR := build
TESTS_DIR := tests

SRC := $(SRC_DIR)/strbuf.c
OBJ	:= $(BUILD_DIR)/strbuf.o
LIB := $(BUILD_DIR)/libstrbuf.a

all: $(LIB)

# make static library
$(LIB): $(OBJ)
	ar rcs $@ $^

# compile
$(OBJ): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@



# test
test: test_strbuf
	$(BUILD_DIR)/test_strbuf

test_strbuf: $(OBJ) $(BUILD_DIR)/test_strbuf.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

$(BUILD_DIR)/test_strbuf.o: $(TESTS_DIR)/test_strbuf.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf build/

.PHONY: all clean test
