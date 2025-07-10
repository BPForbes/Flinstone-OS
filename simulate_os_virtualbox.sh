#!/bin/bash
set -e

BOOTDIR="flinstone_os_drivers/bootloader"
KERNELDIR="flinstone_os_drivers/kernel"
VM_NAME="FlinstoneOS"

if ! command -v VBoxManage >/dev/null 2>&1; then
    echo "VBoxManage not found. Please install VirtualBox." >&2
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

# Convert to VirtualBox disk image
VBoxManage convertfromraw os-image.bin os-image.vdi --format VDI >/dev/null 2>&1

# Create VM if it does not exist
if ! VBoxManage list vms | grep -q "\"$VM_NAME\""; then
    VBoxManage createvm --name "$VM_NAME" --register --ostype Other
    VBoxManage modifyvm "$VM_NAME" --memory 64 --boot1 disk --nic1 none
    VBoxManage storagectl "$VM_NAME" --name "IDE Controller" --add ide
    VBoxManage storageattach "$VM_NAME" --storagectl "IDE Controller" --port 0 --device 0 --type hdd --medium os-image.vdi
else
    VBoxManage storageattach "$VM_NAME" --storagectl "IDE Controller" --port 0 --device 0 --type hdd --medium os-image.vdi --mtype immutable
fi

VBoxManage startvm "$VM_NAME"
