section .text
global _start
extern main

_start:
    ;mov eax, 1
    ;mov ebx, 2
    ;mov ecx, 'a'
    ;int 0x80
    call main

_halt:
    hlt
    jmp _halt