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
ld -Ttext=0x8400 -melf_i386 build/stage2.o -o build/stage2.elf
objcopy -O binary build/stage2.elf build/stage2.bin

# ./asmparser bootloader/stage3/stage3.asm build/ build_asmparser/

# Build Bootloader Stage 3
# nasm -f elf32 -g3 -F dwarf build_asmparser/stage3.asm -o build/stage3.o
# ld -Ttext=0x8400 -melf_i386 build/stage3.o -o build/stage3.elf
# objcopy -O binary build/stage3.elf build/stage3.bin

# rm bootloader/stage3/stage3.asm
# cp build_asmparser/stage3.asm bootloader/stage3/stage3.asm

cd bootloader/stage3
make
cd ../..

tar -czvf stage3.tar.gz bootloader/stage3/build

# Bootloader Stage 3_2
nasm -f elf32 -g3 -F dwarf bootloader/stage3_2/jmploader.asm -o build/stage32.o
ld -Ttext=0x00 -melf_i386 build/stage32.o -o build/stage32.elf
objcopy -O binary build/stage32.elf build/stage32.bin

cd bootloader/stage4
make
cd ../..

#Build User Software
cd user_Software/template
make
cd ../..

# cd bootloader/stage4
# make
# cd ../..

# Build Image file to boot
cd build
rm -f disk.img
dd if=/dev/zero of=disk.img bs=512 count=1440000
mformat -F -i disk.img ::
mcopy -i disk.img stage2.bin ::
mcopy -i disk.img stage2.bin ::/stage22.bin
# mcopy -i disk.img stage32.bin ::
# mmd -i disk.img ::/folder
# mmd -i disk.img ::/folder/subfolder
# mcopy -i disk.img stage3.bin ::
mcopy -i disk.img ../bootloader/stage3/build/stage3.bin ::
mcopy -i disk.img ../bootloader/stage4/build/stage4.elf ::kernel.elf
# mcopy -i disk.img ../bootloader/stage4/build/stage4.bin ::
# mcopy -i disk.img stage4.bin ::
mcopy -i disk.img ../user_Software/template/build/template.elf ::
dd if=boot.bin of=disk.img conv=notrunc