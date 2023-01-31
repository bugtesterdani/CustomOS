BUILD_DIR?=build
ASM?=nasm
ASMFLAGS?=-f elf
CC=i686-elf-gcc
CCFLAGS?=-std=c99 -g -ffreestanding -nostdlib
LD=i686-elf-gcc
LDFLAGS?=-g -T linker.ld -nostdlib
STAGEFILENAME=stage4

SOURCES_C=$(wildcard *.c)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/${STAGEFILENAME}/c/%.obj, $(SOURCES_C))

.PHONY: all ${STAGEFILENAME} clean always

all: ${STAGEFILENAME}

${STAGEFILENAME}: $(BUILD_DIR)/${STAGEFILENAME}.elf

$(BUILD_DIR)/${STAGEFILENAME}.elf: $(OBJECTS_C)
	$(LD) $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/${STAGEFILENAME}.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/${STAGEFILENAME}/c/%.obj: %.c always
	$(CC) $(CCFLAGS) -c -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/${STAGEFILENAME}/c

clean:
	rm -f $(BUILD_DIR)/${STAGEFILENAME}.elf