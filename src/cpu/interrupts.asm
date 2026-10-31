extern c_faults_handler
extern syscall_handler

extern timer_handler
extern keyboard_handler
extern disk_handler

global isr64

%macro PUSHALL 0
    pushad
    mov eax, cr0
    push eax
    mov eax, cr2
    push eax
    mov eax, cr3
    push eax
    mov eax, cr4
    push eax
%endmacro

%macro EXCEPTIONS_NO_ERR_CODE 1
global isr%1
isr%1:
    push 0
    PUSHALL
    jmp faults_handler
%endmacro

%macro EXCEPTIONS_ERR_CODE 1
global isr%1
isr%1:
    push %1
    PUSHALL
    jmp faults_handler
%endmacro

%macro IRQINT 2
global isr%1
isr%1:
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

IRQINT 32, timer_handler
IRQINT 33, keyboard_handler
IRQINT 46, disk_handler

EXCEPTIONS_ERR_CODE         8
EXCEPTIONS_ERR_CODE         13
EXCEPTIONS_ERR_CODE         14

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
    mov eax, esp
    push eax
    call c_faults_handler