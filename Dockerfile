FROM ubuntu:latest

ENV TZ=Europe/Berlin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

ARG VersionGCC=11.1.0
ARG VersionBU=2.37
ARG TARGETARCH=i686
ARG PathBuild=Build_ENV
ARG PathTools=tools

ARG TARGET=${TARGETARCH}-elf
ARG PREFIX="/root/Toolchain/${TARGET}"
ARG PATH="${PREFIX}/bin:$PATH"

COPY ${PathBuild}/sources.list /etc/apt/sources.list
RUN apt-get update
RUN apt-get install build-essential nasm mtools qemu-system-x86 -y
RUN apt-get build-dep gcc-10 -y
RUN mkdir -p /root/Toolchain
COPY ${PathBuild}/gcc-${VersionGCC}.tar.gz /root/Toolchain/gcc.tar.gz
COPY ${PathBuild}/binutils-${VersionBU}.tar.xz /root/Toolchain/binutils.tar.xz
WORKDIR /root/Toolchain
RUN apt-get install xz-utils -y
RUN tar -xvf binutils.tar.xz
RUN tar -xzvf gcc.tar.gz
RUN mkdir -p /root/Toolchain/binutils-build
WORKDIR /root/Toolchain/binutils-build
RUN ../binutils-${VersionBU}/configure --target=$TARGET --prefix="${PREFIX}" --with-sysroot --disable-nls --disable-werror
RUN make -j 4
RUN make install
RUN mkdir -p /root/Toolchain/gcc-build
WORKDIR /root/Toolchain/gcc-build
RUN ../gcc-${VersionGCC}/configure --target=$TARGET --prefix="${PREFIX}" --disable-nls --enable-languages=c,c++ --without-headers
RUN make all-gcc -j 4
RUN make all-target-libgcc -j 4
RUN make install-gcc
RUN make install-target-libgcc
RUN apt-get install cmake -y
COPY ${PathTools}/asmparser /root/Toolchain/asmparser/
RUN mkdir -p /root/Toolchain/asmparser/build
WORKDIR /root/Toolchain/asmparser/build
RUN cmake ..
RUN make
RUN cp src/asmparser /root/Toolchain/asmparser/.
COPY ${PathBuild}/build.sh /root/build.sh
RUN chmod +x /root/build.sh