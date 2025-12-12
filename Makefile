CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
CFLAGS=-mcpu=arm926ej-s -nostdlib -O2

all: test.elf

test.elf: start.s main.c link.ld
	$(CC) $(CFLAGS) -T link.ld start.s main.c -o test.elf

run: test.elf
	qemu-system-arm -M versatilepb -cpu arm926 -nographic -kernel test.elf

clean:
	rm -f *.o *.elf *.bin
