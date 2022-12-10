#!/bin/bash

sudo docker run -t -d --name compile_runner compile_image

rm src.tar.gz
tar -czf src.tar.gz src

sudo docker cp src.tar.gz compile_runner:/root/src.tar.gz

sudo docker exec -it compile_runner /bin/bash /root/build.sh

mkdir -p build
sudo docker cp compile_runner:/root/src/build/disk.img build/disk.img
sudo chown $USER:$USER build/disk.img

sudo docker stop compile_runner

sudo docker rm compile_runner

# Start debugging Instance
qemu-system-x86_64 -s -S -hda build/disk.img &
#gdb -ix "gdb_init_real_mode.txt" build/stage3.elf -ex "target remote localhost:1234" -ex "br *0x7c00" -ex "br *0x8000" -ex "br *0x8400" -ex "c"
gdb -ix "gdb_init_test.txt" -ex "target remote localhost:1234" -ex "br *0x8459" -ex "c"