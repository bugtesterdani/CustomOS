#!/bin/bash

# Bootloader Stage 1
nasm -f elf32 -g3 -F dwarf src/bootloader/stage1/boot.asm -o build/boot.o

# Build Bootloader Stage 2
nasm -f elf32 -g3 -F dwarf src/bootloader/stage2/boot.asm -o build/stage2.o

# Build Bootloader Stage 3
nasm -f elf32 -g3 -F dwarf src/bootloader/stage3/boot.asm -o build/stage3.o

ld -Ttext=0x7c00 -melf_i386 build/boot.o -o build/boot.elf
objcopy -O binary build/boot.elf build/boot.bin

ld -Ttext=0x8000 -melf_i386 build/stage2.o -o build/stage2.elf
objcopy -O binary build/stage2.elf build/stage2.bin

ld -Ttext=0x8400 -melf_i386 build/stage3.o -o build/stage3.elf
objcopy -O binary build/stage3.elf build/stage3.bin

# Build Assembler Stage 4 Files
nasm -f obj -o build/main.obj src/bootloader/stage4/main.asm
nasm -f obj -o build/x86.obj src/bootloader/stage4/x86.asm

# Build C Stage 4 Files
/usr/bin/watcom/binl64/wcc -4 -d3 -s -wx -ms -zl -zq -fo=build/main_c.obj src/bootloader/stage4/main.c
/usr/bin/watcom/binl64/wcc -4 -d3 -s -wx -ms -zl -zq -fo=build/stdio_c.obj src/bootloader/stage4/stdio.c
/usr/bin/watcom/binl64/wcc -4 -d3 -s -wx -ms -zl -zq -fo=build/fat32_c.obj src/bootloader/stage4/fat32.c

# Build Stage 4 Binary File
/usr/bin/watcom/binl64/wlink NAME build/stage4.bin FILE { build/main.obj build/x86.obj build/main_c.obj build/stdio_c.obj build/fat32_c.obj } OPTION MAP=build/stage4.map @src/bootloader/stage4/linker.lnk

# make image file
cd build
rm disk.img
dd if=/dev/zero of=disk.img bs=512 count=1440000
mformat -F -i disk.img ::
mcopy -i disk.img stage2.bin ::
mcopy -i disk.img stage2.bin ::/stage22.bin
mcopy -i disk.img stage3.bin ::
mcopy -i disk.img stage4.bin ::
mcopy -i disk.img ../test.txt ::
dd if=boot.bin of=disk.img conv=notrunc

# run the disk image
# to run use x86_64
#qemu-system-x86_64 -hda disk.img
# to debug we need to use i386
qemu-system-i386 -s -S -hda disk.img &
cd ..
gdb -ix "gdb_init_real_mode.txt" build/stage3.elf -ex "target remote localhost:1234" -ex "br *0x7c00" -ex "br *0x8000" -ex "br *0x8400" -ex "br *0x8414" -ex "c"
