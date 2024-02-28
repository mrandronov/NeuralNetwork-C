
TARGET := main
CC := gcc

SRC_DIR := ./src
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR)/* -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.c=.o))

INC_DIRS := $(shell find $(SRC_DIR)/* -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := -Wall

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) $(CFLAGS) $(INC_FLAGS) -o $(TARGET) $^
	@echo "Done!"

.PHONY: clean
clean:
	@rm -r main $(BUILD_DIR)

