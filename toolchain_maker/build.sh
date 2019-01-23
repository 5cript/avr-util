#!/bin/bash

# Read the notes.txt if you have any trouble

function exit_on_error 
{
	echo -e "e[91mAn error occured:\e[0m $1"
	exit 1
}

# Please consider updating if you are from the future.
BINUTILS_VERSION="2.31.1"
GCC_RELEASE_TAG="8_2_release"
AVR_LIBC_VERSION="2.0.0"

# Other Constants
INSTALL_DIRECTORY="$(pwd)/install"
echo -e "AVR binaries will be installed under: \e[32m${INSTALL_DIRECTORY}\e[0m"

MAKE_BINUTIL_THREADS=4
MAKE_GCC_THREADS=4

# Make and navigate Directories
mkdir -p sources
cd sources

mkdir -p downloads
cd downloads

# Download binutils
if [ ! -f ./binutils.tar.bz2 ]; then
	echo -e "Downloading \e[1m\e[32mBinutils\e[0m"
	wget -O binutils.tar.bz2 "ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.bz2" || exit_on_error "could not download binutils"
else
	echo -e "\e[1m\e[32mBinutils\e[0m exists"
fi

# Download avr-libc
if [ ! -f "./avr-libc-$AVR_LIBC_VERSION.tar.bz2" ]; then
	echo -e "Downloading \e[1m\e[32mAVR-libc\e[0m"
	wget "http://download.savannah.gnu.org/releases/avr-libc/avr-libc-$AVR_LIBC_VERSION.tar.bz2" || exit_on_error "could not download avr-libc"
else
	echo -e "\e[1m\e[32mAVR-libc\e[0m exists"
fi

# Go up from "downloads"
cd ..

# Unpack downloads
if [ ! -d "binutils-$BINUTILS_VERSION" ]; then
	echo "Unpacking files binutils"
	tar -xjvf "./downloads/binutils.tar.bz2" || exit_on_error "could not unpack binutils"
fi

if [ ! -d "avr-libc-$AVR_LIBC_VERSION" ]; then
	echo "Unpacking files avr"
	tar -xjvf "./downloads/avr-libc-$AVR_LIBC_VERSION.tar.bz2" || exit_on_error "could not unpack avr-libc"
fi

# Checkout GCC
if [ ! -d "gcc" ]; then
 	echo -e "Downloading \e[1m\e[32mGCC\e[0m"
 	echo -e "Please do NOT interupt or delete folder before script rerun"
 	echo -e "Alternatively perform this step manually"
 	sleep 5s
 	svn checkout "svn://gcc.gnu.org/svn/gcc/trunk" gcc || exit_on_error "could not checkout gcc"
else
 	echo -e "\e[1m\e[32mGCC\e[0m exists"
fi

# Build Binutils
cd ..
mkdir -p builds
cd builds

PREFIX=$INSTALL_DIRECTORY

if [ ! -f "$INSTALL_DIRECTORY/bin/avr-ld.exe" ]; then
	echo "Building binutils"
	mkdir -p binutils
	cd binutils
	../../sources/binutils-$BINUTILS_VERSION/configure --prefix=$PREFIX --target=avr --disable-nls || exit_on_error "could not configure binutils"
	make -j $MAKE_BINUTIL_THREADS || exit_on_error "could not make binutils"
	make install || exit_on_error "could not install binutils"
	cd ..
else
 	echo -e "\e[1m\e[32mSkipping binutils build\e[0m, sporadic check determined existance."
fi

if [ ! -f "$INSTALL_DIRECTORY/bin/avr-gcc.exe" ]; then
	mkdir -p gcc
	cd gcc
	../../sources/gcc/configure --prefix=$PREFIX --target=avr --enable-languages=c,c++ --disable-nls --disable-libssp --with-dwarf2 || exit_on_error "could not configure gcc"
	make -j $MAKE_GCC_THREADS || exit_on_error "could not make gcc"
	make install || exit_on_error "could not install gcc"
else 
 	echo -e "\e[1m\e[32mSkipping gcc build\e[0m, sporadic check determined existance."
fi