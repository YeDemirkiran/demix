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
GRUB_DIR := $(BOOT_DIR)/grub
KERNEL_BIN_NAME := kernel.bin
KERNEL_BIN_PATH := $(BOOT_DIR)/$(KERNEL_BIN_NAME)
GRUB_CFG_NAME := grub.cfg

LINKER_FILE := ./linker.ld

# Tools configuration
CC := gcc
AS := nasm
LD := ld
QEMU := qemu-system-i386

CC_FLAGS := -Wall -Wextra -Werror -m32 -ffreestanding -c
AS_FLAGS := -f elf32
LD_FLAGS := -m elf_i386 -T $(LINKER_FILE)
QEMU_FLAGS := -cdrom

# Rules

all: $(NAME)

run: all
	$(QEMU) $(QEMU_FLAGS) $(NAME)

$(NAME): kernel-bin copy-grub-cfg
	grub-mkrescue -o $@ $(ISO_DIR)

kernel-bin: $(KERNEL_BIN_PATH)
copy-grub-cfg: $(GRUB_DIR)/$(GRUB_CFG_NAME)

$(KERNEL_BIN_PATH): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) -o $@ $^

$(GRUB_DIR)/$(GRUB_CFG_NAME): $(GRUB_CFG_NAME)
	@mkdir -p $(dir $@)
	cp $< $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

fclean: clean
	rm -rf $(NAME) $(ISO_DIR)

re: fclean all

.PHONY: all kernel-bin copy-grub-cfg run clean fclean re
