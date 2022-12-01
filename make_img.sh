dd if=/dev/zero of=disk.img bs=512 count=1440000
mformat -F -i disk.img ::
mcopy -i disk.img stage2.bin ::
mcopy -i disk.img test.txt ::
dd if=boot.bin of=disk.img conv=notrunc
