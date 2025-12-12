#!/bin/bash
set -e

echo "[+] Running in directory: $(pwd)"

# Build project
make

# Launch QEMU in a new terminal so current terminal is free
gnome-terminal -- bash -c "echo '[+] Launching QEMU...'; qemu-system-arm -M versatilepb -cpu arm926 -nographic -serial mon:stdio -kernel test.elf; echo '[+] QEMU exited'; exec bash"
