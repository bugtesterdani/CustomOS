#!/bin/bash

sudo docker run -t -d --name compile_runner compile_image
status=$?
if [ $status -ne 0 ]; then
    echo "Docker Image nicht vorhanden ?!"
    sudo docker build --tag compile_image .
    status=$?
    if [ $status -ne 0 ]; then
        echo "Error. Docker Image schon vorhanden, kann nicht aber gestartet werden?!?"
        return $status
    fi
fi

rm src.tar.gz
tar -czf src.tar.gz src

sudo docker cp src.tar.gz compile_runner:/root/src.tar.gz
sudo docker cp tests/test.elf compile_runner:/root/test.elf

sudo docker exec -it compile_runner /bin/bash /root/build.sh

# sudo docker exec -it compile_runner /bin/bash

mkdir -p build
sudo docker cp compile_runner:/root/src/build/disk.img build/disk.img
sudo docker cp compile_runner:/root/src/build/customos.iso build/customos.iso
# sudo docker cp compile_runner:/root/src/bootloader/stage3/build/stage3.map build/stage3.map
sudo docker cp compile_runner:/root/src/build/boot.bin build/stage1.bin
sudo docker cp compile_runner:/root/src/build/stage2.bin build/stage2.bin
sudo docker cp compile_runner:/root/src/stage3.tar.gz build/stage3.tar.gz
sudo docker cp compile_runner:/root/src/bootloader/stage3/build/stage3.bin build/stage3.bin
sudo docker cp compile_runner:/root/src/build/stage32.bin build/stage32.bin
sudo docker cp compile_runner:/root/src/bootloader/stage4/build/stage4.elf build/stage4.elf
sudo docker cp compile_runner:/root/src/user_Software/template/build/template.elf build/template.elf
sudo chown $USER:$USER build/disk.img build/customos.iso

# sudo docker exec -it compile_runner /bin/bash

sudo docker stop compile_runner

sudo docker rm compile_runner

# Compile for VirtualBox
rm build/disk.vdi
vboxmanage convertfromraw --format VDI --uuid=220868a3-515d-4693-875f-491f7bf30660 build/disk.img build/disk.vdi
chmod +w build/disk.vdi

# Start debugging Instance
qemu-system-x86_64 -s -S -m 2m -cdrom build/customos.iso -boot d &
# qemu-system-i386 -s -S -hda build/disk.img &
#gdb -ix "gdb_init_real_mode.txt" build/stage3.elf -ex "target remote localhost:1234" -ex "br *0x7c00" -ex "br *0x8000" -ex "br *0x8400" -ex "c"
gdb -ix "gdb_init_test.txt" -ex "disassembly-flavor intel" -ex "target remote localhost:1234" -ex "apropos lx" -ex "br *0x0" -ex "c"
# gdb -ix "gdb_init_real_mode.txt" -ex "target remote localhost:1234" -ex "br *0x8400"
