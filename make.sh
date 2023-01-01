#!/bin/bash

sudo docker run -t -d --name compile_runner compile_image

rm src.tar.gz
tar -czf src.tar.gz src

sudo docker cp src.tar.gz compile_runner:/root/src.tar.gz

sudo docker exec -it compile_runner /bin/bash /root/build.sh

mkdir -p build
sudo docker cp compile_runner:/root/src/build/disk.img build/disk.img
# sudo docker cp compile_runner:/root/src/bootloader/stage3/build/stage3.map build/stage3.map
sudo docker cp compile_runner:/root/src/build/boot.bin build/stage1.bin
sudo docker cp compile_runner:/root/src/build/stage2.bin build/stage2.bin
sudo docker cp compile_runner:/root/src/bootloader/stage3/build/stage3.bin build/stage3.bin
sudo chown $USER:$USER build/disk.img

# sudo docker exec -it compile_runner /bin/bash

sudo docker stop compile_runner

sudo docker rm compile_runner

# Compile for VirtualBox
rm build/disk.vdi
vboxmanage convertfromraw --format VDI --uuid=220868a3-515d-4693-875f-491f7bf30660 build/disk.img build/disk.vdi
chmod +w build/disk.vdi

# Start debugging Instance
qemu-system-x86_64 -s -S -m 4096 -hda build/disk.img &
# qemu-system-i386 -s -S -hda build/disk.img &
#gdb -ix "gdb_init_real_mode.txt" build/stage3.elf -ex "target remote localhost:1234" -ex "br *0x7c00" -ex "br *0x8000" -ex "br *0x8400" -ex "c"
gdb -ix "gdb_init_test.txt" -ex "target remote localhost:1234" -ex "c"
# gdb -ix "gdb_init_real_mode.txt" -ex "target remote localhost:1234" -ex "br *0x8400"
