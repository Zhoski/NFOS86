section .text
global isr8
global isr13
global isr14

%macro ISRERRCODE 2
%1:
    push %2
    jmp faults_handler
%endmacro

extern c_faults_handler

ISRERRCODE isr8, 8
ISRERRCODE isr13, 13
ISRERRCODE isr14, 14

faults_handler:
    call c_faults_handler