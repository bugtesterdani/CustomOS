#!/bin/bash

# Start debugging Instance
qemu-system-x86_64 -s -S -m 8192m -hda build/disk.img &
# qemu-system-i386 -s -S -hda build/disk.img &
#gdb -ix "gdb_init_real_mode.txt" build/stage3.elf -ex "target remote localhost:1234" -ex "br *0x7c00" -ex "br *0x8000" -ex "br *0x8400" -ex "c"
# gdb -ix "gdb_init_test.txt" -ex "target remote localhost:1234" -ex "br *0x415c" -ex "br *0x462c" -ex "br *0x42da" -ex "br *0x00" -ex "br *0x172f" -ex "br *0x4376" -ex "br *0x414e" -ex "br *0xFF000000" -ex "watch *0x00" -ex "watch *0x01" -ex "watch *0x02" -ex "watch *0x1c0cd5" -ex "c"
# gdb -ix "gdb_init_test.txt" -ex "target remote localhost:1234" -ex "br *0xa00" -ex "br *0x84e6" -ex "br *0x00" -ex "br *0x172f" -ex "br *0xFF000000" -ex "watch *0x00" -ex "watch *0x01" -ex "watch *0x02" -ex "watch *0x1c2000" -ex "watch *0x1c0cd5" -ex "c"
gdb -ix "gdb_init_test.txt" -ex "target remote localhost:1234" -ex "br *0x00" -ex "br *0x44b8" -ex "c"
# gdb -ix "gdb_init_real_mode.txt" -ex "target remote localhost:1234" -ex "br *0x8400"
