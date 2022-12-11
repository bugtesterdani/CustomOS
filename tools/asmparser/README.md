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
$ ./src/asmparser <.asm file> <folder with elf>

where:
    <.asm file> : A .asm file for parsing
    <folder with elf> : Directory to .elf files
```