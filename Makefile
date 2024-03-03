
TARGET := main
CC := gcc

SRC_DIR := ./src
BUILD_DIR := ./build

DATA_DIR := ./data
TRAINING_FILE := "mnist_train.csv"
TESTING_FILE := "mnist_test.csv"

SRCS := $(shell find $(SRC_DIR)/* -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.c=.o))

INC_DIRS := $(shell find $(SRC_DIR)/* -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := -Wall
CFLAGS += -O3

# Color Formatting
CC_GREEN := $(shell echo "\033[92;1m")
CC_BLUE := $(shell echo "\033[94;1m")
CC_YELLOW := $(shell echo "\033[93;1m")
CC_END := $(shell echo "\033[0m")

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

all: $(OBJS)
	@if ! [ -d $(DATA_DIR) ]; then \
		echo "$(CC_YELLOW)WARNING:$(CC_END) Include training and test data in .csv format in ./data folder!"; \
		mkdir ./data; \
	fi
	@if ! [ -f $(DATA_DIR)/$(TRAINING_FILE) ]; then \
		echo "$(CC_YELLOW)WARNING:$(CC_END) Training data in $(DATA_DIR) folder is missing!"; \
		echo "Please include a .csv file called $(TRAINING_FILE) with training data in the $(DATA_DIR) directory!"; \
	fi
	@if ! [ -f $(DATA_DIR)/$(TESTING_FILE) ]; then \
		echo "$(CC_YELLOW)WARNING:$(CC_END) Testing data in $(DATA_DIR) folder is missing!"; \
		echo "Please include a .csv file called $(TESTING_FILE) with testing data in the $(DATA_DIR) directory!"; \
	fi

	$(CC) $(CFLAGS) $(INC_FLAGS) -o $(TARGET) $^
	@echo "$(CC_GREEN)Build done!$(CC_END)"

.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	@if [ -d $(BUILD_DIR) ]; then \
		rm -r $(BUILD_DIR); \
	fi 
	@echo "Cleaning target executable..."
	@if [ -f $(TARGET) ]; then \
		rm $(TARGET);\
	fi
	@echo "$(CC_BLUE)Clean done!$(CC_END)"

