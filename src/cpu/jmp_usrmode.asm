section .text
global jmp_usrmode

jmp_usrmode:
    mov ebx, [esp + 4]
    mov edx, [esp + 8]
    mov ax, 0x0f
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
 
    mov eax, edx
    push 0x0f
    push eax
    pushf
    push 0x07
    push ebx
    iret
