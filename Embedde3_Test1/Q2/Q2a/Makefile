#--------------------------------------Makefile-------------------------------------

CFILES = $(wildcard ./kernel/*.c)
OFILES = $(CFILES:./kernel/%.c=./object/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean uart_build kernel8.img run


uart_build: ./peripheral/uart0.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./peripheral/uart0.c -o ./object/uart0.o

./object/boot.o: ./kernel/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./kernel/boot.S -o ./object/boot.o

./object/%.o: ./kernel/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./object/boot.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./object/boot.o $(OFILES) -T ./kernel/link.ld -o kernel8.elf
	aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -f kernel8.elf ./object/*.o *.img

# Run emulation with QEMU
run: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio
