#!/bin/sh

mkdir -p build
mkdir -p build_asmparser
cp /root/Toolchain/asmparser/asmparser .

# Bootloader Stage 1
nasm -f elf32 -g3 -F dwarf bootloader/stage1/stage1.asm -o build/stage1.o
ld -Ttext=0x7c00 -melf_i386 build/stage1.o -o build/stage1.elf
objcopy -O binary build/stage1.elf build/boot.bin

./asmparser bootloader/stage2/stage2.asm build/ build_asmparser/

# Build Bootloader Stage 2
nasm -f elf32 -g3 -F dwarf build_asmparser/stage2.asm -o build/stage2.o
ld -Ttext=0x8000 -melf_i386 build/stage2.o -o build/stage2.elf
objcopy -O binary build/stage2.elf build/stage2.bin

cd bootloader/stage3
make
cd ../..

# Build Image file to boot
cd build
dd if=/dev/zero of=disk.img bs=512 count=1440000
mformat -F -i disk.img ::
mcopy -i disk.img stage2.bin ::
mcopy -i disk.img stage2.bin ::/stage22.bin
mcopy -i disk.img ../bootloader/stage3/build/stage3.bin ::
# mcopy -i disk.img stage4.bin ::
dd if=boot.bin of=disk.img conv=notrunc