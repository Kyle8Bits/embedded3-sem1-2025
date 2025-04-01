#--------------------------------------Makefile-------------------------------------

CFILES = $(wildcard ./kernel/kernel.c)
OFILES = $(CFILES:./kernel/%.c=./object/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean uart1_build  kernel8.img run1

	

uart1: GCCFLAGS += -DUART1
uart1: clean uart1_build kernel8.img run1

uart0: GCCFLAGS += -DUART0
uart0: clean uart0_build kernel8.img run0


uart0_build: ./peripheral/uart0.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o ./object/uart.o

uart1_build: ./kernel/uart1.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o ./object/uart.o

./object/boot.o: ./kernel/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./kernel/boot.S -o ./object/boot.o

./object/%.o: ./kernel/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./kernel/kernel.c -o $@

kernel8.img: ./object/boot.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./object/boot.o $(OFILES) -T ./kernel/link.ld -o kernel8.elf
	aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -f kernel8.elf ./object/*.o *.img

clean_uart1:
	rm -f ./object/uart1.o

# Run emulation with QEMU
run0: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio

run1: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio
