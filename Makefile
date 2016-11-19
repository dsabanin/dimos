C_SOURCES = $(wildcard  kernel/*.c drivers/*.c)
CFLAGS = -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -ffreestanding \
		 -Wall -Wextra -Werror -O0 -m32
LDFLAGS = -m elf_i386
AS = nasm
ASFLAGS = -f elf32
CC = gcc
QEMU = qemu-system-i386
OBJ = ${C_SOURCES:.c=.o}

all: os_image.vmdk

run: all
	${QEMU} -s -fda os_image.vmdk -boot a

debug: all
	${QEMU} -S -s -fda os_image.vmdk -boot a &
	gdb
	pkill -9 qemu-system

os_image.vmdk: os_image.bin
	qemu-img convert -f raw -O vmdk $< $@

os_image.bin: boot/boot_sect.bin kernel.bin
	cat $^ > $@

kernel.bin: kernel/kernel_entry.o ${OBJ}
		ld ${LDFLAGS} -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c
		${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
		${AS} $< ${ASFLAGS} -o $@

%.bin: %.asm
	${AS} $< -f bin -o $@

clean:
	rm -rf *.bin *.o *.dis os-image.* *.map
	rm -rf kernel/*.o boot/*.bin drivers/*.o

kernel.dis: kernel.bin
	ndisasm -o 0x7c00 -b 32 $< > $@
