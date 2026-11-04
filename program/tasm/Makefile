TARGET = ../../bin/tasm.ndf
CONV = ../../utilities/ndfconv/ndfconv

$(TARGET): tasm.elf
	$(CONV) tasm.elf $(TARGET)

tasm.elf : entry.o main.o stdio.o
	gcc -m32 -nostdlib -static-pie \
	    -T ../general/user.ld entry.o stdio.o main.o -o tasm.elf

main.o : main.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c main.c -o main.o

entry.o: ../general/entry.asm
	nasm -f elf32 ../general/entry.asm -o entry.o

stdio.o: ../libs/stdio.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/stdio.c -o stdio.o

clean :
	rm *.o *.bin tasm.elf