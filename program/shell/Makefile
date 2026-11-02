TARGET = ../../bin/shell.ndf
CONV = ../../utilities/ndfconv/ndfconv

$(TARGET): shell.elf
	$(CONV) shell.elf $(TARGET)

shell.elf : entry.o shell.o stdio.o string.o sys.o mmemory.o
	gcc -m32 -nostdlib -static-pie \
	    -T ../general/user.ld entry.o stdio.o string.o sys.o mmemory.o shell.o -o shell.elf

shell.o : shell.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c shell.c -o shell.o

entry.o: ../general/entry.asm
	nasm -f elf32 ../general/entry.asm -o entry.o

stdio.o: ../libs/stdio.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/stdio.c -o stdio.o

string.o: ../libs/string.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/string.c -o string.o

sys.o: ../libs/sys.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/sys.c -o sys.o

mmemory.o: ../libs/memory.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror  -Os -I./../libs/include -c ../libs/memory.c -o mmemory.o

clean :
	rm *.o *.bin shell.elf