section .text
global jmp_usrmode

jmp_usrmode:
    mov ax, (4 * 8) | 3
    mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax

    push (4 * 8) | 3
    push 0x3FFFF0
    pushf
    push (3 * 8) | 3
    push 0x300000
    iret
