section .text
global gdt_reload_addr

gdt_reload_addr:
    mov eax, [esp + 4]
    lgdt [eax]

    jmp 0x8:reload

reload:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov es, ax
    mov gs, ax
    ret