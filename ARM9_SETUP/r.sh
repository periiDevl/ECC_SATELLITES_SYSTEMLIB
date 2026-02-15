#!/bin/bash
set -e

echo "[+] Running in directory: $(pwd)"

# Build project
make

# Run QEMU in background with UART output, timeout after 5 seconds to auto-exit
echo "[+] Launching QEMU (will auto-exit after 5s)..."
timeout 5s qemu-system-arm -M versatilepb -cpu arm926 -nographic -serial mon:stdio -kernel test.elf

echo "[+] QEMU exited, terminal usable again."
