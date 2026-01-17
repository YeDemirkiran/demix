NAME := demix.iso

# Source and Binary fles
C_FILES := kernel.c
ASM_FILES := boot.asm
SRC_DIR := ./src
BIN_DIR := ./bin
SRC_FILES = $(addprefix $(SRC_DIR)/, $(C_FILES) $(ASM_FILES))
OBJ_FILES = $(addprefix $(BIN_DIR)/, $(C_FILES:.c=.o) $(ASM_FILES:.asm=.o))

# ISO directory and kernel file
ISO_DIR := ./iso
BOOT_DIR := $(ISO_DIR)/boot
KERNEL_BIN_NAME := kernel.bin
KERNEL_BIN_PATH := $(BOOT_DIR)/$(KERNEL_BIN_NAME)

LINKER_FILE := ./linker.ld

# Tools configuration
CC := gcc
AS := nasm
LD := ld
QEMU := qemu-system-i386

CC_FLAGS := -Wall -Wextra -Werror -m32 -ffreestanding
AS_FLAGS := -f elf32
LD_FLAGS := -m elf_i386 -T $(LINKER_FILE)
QEMU_FLAGS := -cdrom

# Rules

all: $(NAME)

run: all
	$(QEMU) $(QEMU_FLAGS) $(NAME)

$(NAME): kernel-bin
	grub-mkrescue -o $@ $(ISO_DIR)

kernel-bin: $(KERNEL_BIN_PATH)

$(KERNEL_BIN_PATH): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm $(NAME) $(KERNEL_BIN_PATH)

re: fclean all

.PHONY: all kernel-bin run clean fclean re
