#!/bin/bash
set -e

BOOTDIR="flinstone_os_drivers/bootloader"
KERNELDIR="flinstone_os_drivers/kernel"

if ! command -v qemu-system-i386 >/dev/null 2>&1; then
    echo "qemu-system-i386 not found. Please install QEMU." >&2
    exit 1
fi

# Build bootloader
make -C "$BOOTDIR"

# Build kernel (fallback to host tools if cross compiler missing)
if command -v i686-elf-gcc >/dev/null 2>&1; then
    make -C "$KERNELDIR"
else
    make -C "$KERNELDIR" CC=gcc LD='ld -m elf_i386'
fi

# Create disk image
dd if=/dev/zero of=os-image.bin bs=512 count=2880 2>/dev/null
dd if="$BOOTDIR/boot.bin" of=os-image.bin conv=notrunc 2>/dev/null
dd if="$KERNELDIR/kernel.bin" of=os-image.bin bs=512 seek=1 conv=notrunc 2>/dev/null

# Launch QEMU
qemu-system-i386 os-image.bin
