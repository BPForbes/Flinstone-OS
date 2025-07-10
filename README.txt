Build Instructions:
--------------------
To build the shell with authentication and account control:

1. Make sure you have sqlite3 and libsodium installed:
   sudo apt install libsqlite3-dev libsodium-dev

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
