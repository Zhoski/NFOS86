section .text
global jmp_usrmode

jmp_usrmode:
    mov ebx, [esp + 4]
    mov edx, [esp + 8]
    mov ax, (1 * 8) | 7
    mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax

    mov eax, edx
    push (1 * 8) | 7
    push eax
    pushf
    push (0 * 8) | 7
    push ebx
    iret
