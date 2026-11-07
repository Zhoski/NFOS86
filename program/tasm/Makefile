TARGET = ../../bin/tasm.ndf
CONV = ../../utilities/ndfconv/ndfconv

$(TARGET): tasm.elf
	$(CONV) tasm.elf $(TARGET)

tasm.elf : entry.o main.o stdio.o lexer.o parser.o sys.o string.o memory.o
	gcc -m32 -nostdlib -static-pie \
	    -T ../general/user.ld entry.o stdio.o sys.o string.o memory.o parser.o lexer.o main.o -o tasm.elf

main.o : main.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c main.c -o main.o

lexer.o : lexer.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c lexer.c -o lexer.o

parser.o : parser.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c parser.c -o parser.o

entry.o: ../general/entry.asm
	nasm -f elf32 ../general/entry.asm -o entry.o

stdio.o: ../libs/stdio.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/stdio.c -o stdio.o

sys.o: ../libs/sys.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/sys.c -o sys.o

string.o: ../libs/string.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/string.c -o string.o

memory.o: ../libs/memory.c
	gcc -m32 -ffreestanding -fPIE -static-pie -fno-plt -Werror -Os -I./../libs/include -c ../libs/memory.c -o memory.o

clean :
	rm *.o *.bin tasm.elf