TARGET = ../../bin/hex.ndf
CONV = ../../utilities/ndfconv/ndfconv

$(TARGET): hex.elf
	$(CONV) hex.elf $(TARGET)

hex.elf : entry.o main.o stdio.o sys.o
	gcc -m32 -nostdlib -static-pie \
	    -T ../general/user.ld entry.o stdio.o sys.o main.o -o hex.elf

main.o : main.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c main.c -o main.o

entry.o: ../general/entry.asm
	nasm -f elf32 ../general/entry.asm -o entry.o

stdio.o: ../libs/stdio.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/stdio.c -o stdio.o

sys.o: ../libs/sys.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/sys.c -o sys.o

clean :
	rm *.o *.bin hex.elf