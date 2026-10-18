gcc -m32 tondf.c elfparser.c main.c -w -o main.elf
chmod +x main.elf
./main.elf bin/shell.elf -o bin/shell.ndf

rm *.elf