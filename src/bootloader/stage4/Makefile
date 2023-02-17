BUILD_DIR?=build
ASM?=nasm
ASMFLAGS?=-f elf
CC=i686-elf-gcc
# CCFLAGS?=-std=c99 -g -ffreestanding -nostdlib -Wall
CCFLAGS?=-std=c99 -g -ffreestanding -nostdlib -Wall -Wno-packed-bitfield-compat
LD=i686-elf-gcc
# LDFLAGS?=-g -T linker.ld -nostdlib -Wall
LDFLAGS?=-g -T linker.ld -nostdlib
STAGEFILENAME=stage4
FILEENDING=elf

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/${STAGEFILENAME}/c/%.obj, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/${STAGEFILENAME}/asm/%.obj, $(SOURCES_ASM))

.PHONY: all ${STAGEFILENAME} clean always

all: ${STAGEFILENAME}

${STAGEFILENAME}: $(BUILD_DIR)/${STAGEFILENAME}.${FILEENDING}

$(BUILD_DIR)/${STAGEFILENAME}.${FILEENDING}: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/${STAGEFILENAME}.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/${STAGEFILENAME}/c/%.obj: %.c always
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BUILD_DIR)/${STAGEFILENAME}/asm/%.obj: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/${STAGEFILENAME}/c
	mkdir -p $(BUILD_DIR)/${STAGEFILENAME}/asm

clean:
	rm -f $(BUILD_DIR)/${STAGEFILENAME}.${FILEENDING}