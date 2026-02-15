#!/bin/bash
set -e

echo "[+] Creating project: arm9_baremetal"
mkdir -p arm9_baremetal
cd arm9_baremetal

echo "[+] Writing main.c"
cat > main.c << 'EOF'
volatile unsigned int *UART0 = (unsigned int*)0x101f1000; // QEMU VersatilePB UART0

void putc(char c) {
    *UART0 = c;
}

void puts(const char *s) {
    while (*s) putc(*s++);
}

int main() {
    puts("Hello from ARM bare-metal!\\n");

    // ECC test example (change as you like)
    unsigned data = 0b10110101;
    unsigned parity = __builtin_parity(data);

    puts("Data parity bit: ");
    putc(parity ? '1' : '0');
    putc('\n');

    return 0;
}
EOF

echo "[+] Writing start.s"
cat > start.s << 'EOF'
    .global _start
_start:
    bl main
1:  b 1b
EOF

echo "[+] Writing link.ld"
cat > link.ld << 'EOF'
ENTRY(_start)

SECTIONS {
    . = 0x10000; /* VersatilePB load address */

    .text : {
        *(.text*)
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
EOF

echo "[+] Writing Makefile"
cat > Makefile << 'EOF'
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
CFLAGS=-mcpu=arm926ej-s -nostdlib -O2

all: test.elf

test.elf: start.s main.c link.ld
	\$(CC) \$(CFLAGS) -T link.ld start.s main.c -o test.elf

run: test.elf
	qemu-system-arm -M versatilepb -cpu arm926 -nographic -kernel test.elf

clean:
	rm -f *.o *.elf *.bin
EOF

echo "[+] Building"
make

echo "[+] Running in QEMU:"
sleep 1
make run
