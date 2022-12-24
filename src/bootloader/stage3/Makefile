BUILD_DIR?=build
ASM?=nasm
ASMFLAGS?=-f elf
CC=i686-elf-gcc
CCFLAGS?=-std=c99 -g -ffreestanding -nostdlib
LD=i686-elf-gcc
LDFLAGS?=-g -T linker.ld -nostdlib

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/stage3/c/%.obj, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/stage3/asm/%.obj, $(SOURCES_ASM))

.PHONY: all stage3 clean always

all: stage3

stage3: $(BUILD_DIR)/stage3.bin

$(BUILD_DIR)/stage3.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/stage3.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/stage3/c/%.obj: %.c always
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BUILD_DIR)/stage3/asm/%.obj: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/stage3/c
	mkdir -p $(BUILD_DIR)/stage3/asm

clean:
	rm -f $(BUILD_DIR)/stage3.bin