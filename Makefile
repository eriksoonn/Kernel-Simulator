CC := gcc
CFLAGS := -Iinclude
SRC_DIR := src
OBJ_DIR := obj
EXECUTABLE := main

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(EXECUTABLE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o ./$@

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

.PHONY: all clean
