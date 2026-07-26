# Compiler
CC = sdcc-sdcc

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Sources
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/drivers/at89c51rb2/timer/timer2.c \
       $(SRC_DIR)/drivers/at89c51rb2/spi/spi.c \
       $(SRC_DIR)/drivers/at89c51rb2/sleep/sleep.c \
       $(SRC_DIR)/drivers/at89c51rb2/external_interrupts/external_interrupts.c \
       $(SRC_DIR)/drivers/at89c51rb2/segments_display/segments_display.c \
       $(SRC_DIR)/hal/at89c51rb2/hal_display.c \
       $(SRC_DIR)/hal/at89c51rb2/hal_timer.c \
       $(SRC_DIR)/hal/at89c51rb2/hal_spi.c \
       $(SRC_DIR)/application/FSM.c \
       $(SRC_DIR)/utils/stack.c

# Object files (.rel)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.rel)

# Output
TARGET = $(BUILD_DIR)/main

# Flags
CFLAGS = -I$(INC_DIR)

# Default rule
all: $(TARGET).ihx

# Link step
$(TARGET).ihx: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile each .c into .rel
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Optional: generate hex
hex: $(TARGET).hex

$(TARGET).hex: $(TARGET).ihx
	sdcc-packihx $< > $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean hex
