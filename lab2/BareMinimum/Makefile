#--------------------------------------Makefile-------------------------------------
CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean kernel8.img run

boot.o: boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c boot.S -o boot.o
%.o: %.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@
kernel8.img: boot.o $(OFILES)
	aarch64-none-elf-ld -nostdlib boot.o $(OFILES) -T link.ld -o kernel8.elf
	aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img
clean:
	rm -f kernel8.elf *.o *.img
run: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -d in_asm