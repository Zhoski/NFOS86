extern c_faults_handler
extern syscall_handler

extern timer_handler
extern keyboard_handler
extern disk_handler

global isr64

%macro ISRERRCODE 2
global %1
%1:
    push %2
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    mov eax, cr0
    push eax
    mov eax, cr2
    push eax
    mov eax, cr3
    push eax
    mov eax, cr4
    push eax
    mov eax, esp
    push eax

    jmp faults_handler
%endmacro

%macro IRQINT 2
global %1
%1:
    cli

    pushad
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov es, ax
    mov ds, ax

    call %2

    mov al, 0x20
    out 0x20, al
    out 0xA0, al

    pop gs
    pop fs
    pop es
    pop ds
    popad

    iret
%endmacro

IRQINT isr32, timer_handler
IRQINT isr33, keyboard_handler
IRQINT isr46, disk_handler

ISRERRCODE isr8,   8
ISRERRCODE isr13, 13
ISRERRCODE isr14, 14

isr64: 
    cli                        
    push edi
    push esi
    push ebp
    push edx
    push ecx
    push ebx 

    push ds
    push es
    push fs
    push gs
  
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax
  
    mov ax, 0x10
    mov ds, ax
    mov es, ax 

    call syscall_handler
    add esp, 4
    
    pop ebx
    pop ecx
    pop edx     
    pop esi
    pop edi
    
    pop gs
    pop fs
    pop es
    pop ds
 
    pop ebx
    pop ecx
    pop edx
    pop ebp
    pop esi
    pop edi
    
    sti                         
    iret 

faults_handler:
    call c_faults_handler