CC = gcc
CFLAGS = -Wall
SRC_DIR = src
OBJ_DIR = bin/obj
BIN_DIR = bin
TEST_DIR = tests

TARGET = $(BIN_DIR)/graph
TEST_TARGET = $(BIN_DIR)/test

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TEST_OBJS_REQUIRED = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS) $(TEST_OBJS_REQUIRED)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_SRCS) $(TEST_OBJS_REQUIRED) -o $(TEST_TARGET)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean test
