TARGET = ../../bin/hello.ndf
CONV = ../../utilities/ndfconv/ndfconv

$(TARGET): hello.elf
	$(CONV) hello.elf $(TARGET)

hello.elf : entry.o hello.o stdio.o
	gcc -m32 -nostdlib -static-pie \
	    -T ../general/user.ld entry.o stdio.o hello.o -o hello.elf

hello.o : hello.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c hello.c -o hello.o

entry.o: ../general/entry.asm
	nasm -f elf32 ../general/entry.asm -o entry.o

stdio.o: ../libs/stdio.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/stdio.c -o stdio.o

clean :
	rm *.o *.bin hello.elf