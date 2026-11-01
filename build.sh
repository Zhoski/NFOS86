make
# Загрузчик
nasm -f bin src/bootloader/boot.asm -o boot.bin
nasm -f bin src/bootloader/stage2.asm -o rootFS/stage2.bin
# ==============================================================================
# ДРАЙВЕРЫ 
# ==============================================================================
gcc -m32 -ffreestanding -c src/drivers/video/vga_640_480.c -Werror -Os -o vga_640_480.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/video/video.c -Werror -Os -o video.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/keyboard/keyboard.c -Werror -Os -o keyboard.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/disk/ata.c -Werror -Os -o ata.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/disk/sata.c -Werror -o sata.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/disk/disk.c -Werror -Os -o disk.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/timer/timer.c -Werror -Os -o timer.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/drivers/pci/pci.c -Werror -Os -o pci.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

# ==============================================================================
# ПРОЦЕССОР И ПРЕРЫВАНИЯ
# ==============================================================================
gcc -m32 -ffreestanding -c src/cpu/idt.c -Werror -o idt.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/cpu/pic.c -Werror -o pic.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/cpu/paging.c -Werror -o paging.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/cpu/gdt.c -Werror -o gdt.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/cpu/faults_handler.c -Werror -Os -o faults_handler.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

nasm -f elf32 src/cpu/interrupts.asm -o interrupts.o
nasm -f elf32 src/cpu/gdt_rel.asm -o gdt_rel.o
nasm -f elf32 src/cpu/jmp_usrmode.asm -o jmp_usrmode.o

# ==============================================================================
# ЯДРО И ВНУТРЕННИЕ СЕРВИСЫ
# ==============================================================================
gcc -m32 -ffreestanding -c src/kernel/nf86krn.c -Werror -Os -o nf86krn.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/memory/memory.c -Werror -Os -o memory.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/memory/mm.c -Werror -Os -o mm.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/memory/pmm.c -Werror -Os -o pmm.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/memory/program.c -Werror -Os -o program.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/loader/loader.c -Werror -Os -o loader.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/syscall/syscall.c -Werror -Os -o syscall.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/kernel/services/panic/panic.c -Werror -Os -o panic.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/fs/fs.c -Werror -Os -o fs.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

gcc -m32 -ffreestanding -c src/fs/fluidfs.c -Werror -Os -o fluidfs.o \
    -I./src/cpu/include -I./src/drivers/include -I./src/kernel/include -I./src/lib/include -I./src/fs/include

# ==============================================================================
# ЛИНКОВКА 
# ==============================================================================
ld -m elf_i386 -T linker.ld -Map=kernel.map nf86krn.o vga_640_480.o video.o keyboard.o \
                 sata.o disk.o timer.o pci.o idt.o pic.o jmp_usrmode.o gdt_rel.o interrupts.o  gdt.o faults_handler.o panic.o memory.o \
                 mm.o pmm.o paging.o syscall.o fluidfs.o fs.o program.o loader.o -o nf86krn.elf

objcopy -O binary nf86krn.elf rootFS/nf86krn.bin

cd program
cd shell
make
make clean
#cd ..
#cd notepad
#make
#make clean
cd ..
cd ..

echo '=================== Disk assembly ==================='
./utilities/ffspack/ffspack.elf -c out.img -M 4 -bs 4096 -b boot.bin -mb rootFS/stage2.bin
./utilities/ffspack/ffspack.elf -d out.img -p rootFS/nf86krn.bin rootFS/setup.bin -o /system/
./utilities/ffspack/ffspack.elf -d out.img -p license.txt -o /
./utilities/ffspack/ffspack.elf -d out.img -p about.txt -o /
./utilities/ffspack/ffspack.elf -d out.img -p rootFS/shell.ndf rootFS/hello.ndf rootFS/notepad.bin -o /home/

qemu-system-i386 -m 4M -no-reboot \
                   -device ich9-ahci,id=ahci \
                   -drive file=out.img,if=none,id=disk \
                   -device ide-hd,bus=ahci.0,drive=disk

make clean
rm *.o
rm *bin
rm rootFS/*bin
rm *elf
#rm *img