#!/bin/bash
export PATH="/root/Toolchain/i686-elf/bin:$PATH"
export TOOLCHAIN="/root/Toolchain/i686-elf"
echo $PATH
cd /root/
tar -xzf src.tar.gz
cd src
/bin/bash builder.sh