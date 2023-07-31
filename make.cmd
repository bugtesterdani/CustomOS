:: Run Docker Container
docker run -t -d --name compile_runner compile_image

:: Check Exit Code and compile dockerfile to image if, exit code is not success (0)
if %ErrorLevel% neq 0 (
    docker build --tag compile_image .
    :: Start the failed earlier Docker Container
    docker run -t -d --name compile_runner compile_image
)

:: Delete src zipped file
del src.tar.gz
tar -czf src.tar.gz src

:: copy src file to server with executable test.elf file
docker cp src.tar.gz compile_runner:/root/src.tar.gz
docker cp tests/test.elf compile_runner:/root/test.elf

:: execute the build script
docker exec -it compile_runner /bin/bash /root/build.sh

:: copy back all files needed for running and debugging
del /F /S /Q build
mkdir build
docker cp compile_runner:/root/src/build/disk.img build/disk.img
docker cp compile_runner:/root/src/build/boot.bin build/stage1.bin
docker cp compile_runner:/root/src/build/stage2.bin build/stage2.bin
docker cp compile_runner:/root/src/stage3.tar.gz build/stage3.tar.gz
docker cp compile_runner:/root/src/bootloader/stage3/build/stage3.bin build/stage3.bin
docker cp compile_runner:/root/src/build/stage32.bin build/stage32.bin
docker cp compile_runner:/root/src/bootloader/stage4/build/stage4.elf build/stage4.elf
docker cp compile_runner:/root/src/user_Software/template/build/template.elf build/template.elf

:: Stoping the Docker Runner
docker stop compile_runner

:: Delete again the Docker Runner
docker rm compile_runner

:: Compile the Image for VirtualBox
::del build/disk.vdi
"C:\Program Files\Oracle\VirtualBox\VBoxManage.exe" convertfromraw --format VDI --uuid=220868a3-515d-4693-875f-491f7bf30660 build/disk.img build/disk.vdi

:: Run QEMU Debugging Instance
start "" "C:\Program Files\qemu\qemu-system-x86_64.exe" -s -S -m 2m -hda build/disk.img

:: Run GDB Debugger for the QEMU Instance
"C:\Windows\System32\wsl.exe" -u root --cd /mnt/e/tmp/custom-os -e gdb -ix "gdb_init_test.txt" -ex "target remote 172.22.48.1:1234" -ex "c"