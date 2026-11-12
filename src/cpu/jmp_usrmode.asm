section .text
global jmp_usrmode

jmp_usrmode:
    mov ebx, [esp + 4]
    mov edx, [esp + 8]
    mov ax, 0x0F ;(4 * 8) | 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
 
    mov eax, edx
    push 0xF ;(4 * 8) | 3
    push eax
    pushf
    push 0x07 ;(3 * 8) | 3
    push ebx
    iret
