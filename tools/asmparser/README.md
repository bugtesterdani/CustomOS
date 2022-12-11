# ASMParser

## Required packages
#### CMAKE
```bash
$ sudo apt install cmake
```

## How to use?
```bash
$ cd tools/asmparser
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./src/asmparser <.asm file> <folder with elf> <out dir>

where:
    <.asm file> : A .asm file for parsing
    <folder with elf> : Directory to .elf files
    <out dir>: Output dir, you have to create one
```