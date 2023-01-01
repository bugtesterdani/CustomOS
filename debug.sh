#!/bin/bash

# Start debugging Instance
qemu-system-x86_64 -s -S -m 4096 -hda build/disk.img &
# qemu-system-i386 -s -S -hda build/disk.img &
#gdb -ix "gdb_init_real_mode.txt" build/stage3.elf -ex "target remote localhost:1234" -ex "br *0x7c00" -ex "br *0x8000" -ex "br *0x8400" -ex "c"
gdb -ix "gdb_init_test.txt" -ex "target remote localhost:1234" -ex "br *0x1702" -ex "c"
# gdb -ix "gdb_init_real_mode.txt" -ex "target remote localhost:1234" -ex "br *0x8400"
