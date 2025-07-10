Build Instructions:
--------------------
To build the shell with authentication and account control:

1. Install sqlite3 and the libsodium runtime library:
   sudo apt install libsqlite3-dev libsodium23
   (The required `sodium.h` header is bundled under `third_party/libsodium`.)

2. Then run:
   make

3. Run the shell:
   ./shell

This will launch an interactive terminal with user login and command execution.

Simulation Mode:
----------------
You can boot Flinstone OS in an emulator instead of writing it to a disk.

1. Install QEMU:
   sudo apt install qemu-system-x86

2. Run the simulator script:
   ./simulate_os.sh

   The script builds the bootloader and kernel, creates a disk image, and
   launches QEMU with that image so you can experiment with the OS from an
   IDE or terminal.

3. Optionally, install VirtualBox:
   sudo apt install virtualbox

4. Run the VirtualBox simulator script:
   ./simulate_os_virtualbox.sh

   This script converts the disk image to a VDI file and boots it in a
   temporary VirtualBox VM so you can test Flinstone OS without QEMU.
