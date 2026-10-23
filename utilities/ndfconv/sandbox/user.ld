ENTRY(_start)

SECTIONS 
{
    . = 0x000000;

    .text : 
    {
        *.entry.o(.text)
        *(.text)        
        *(.text*)           
    }

    .rodata :
    {
        *(.rodata)
        *(.rodata*)
    }

    .data :
    {
        *(.data)         
        *(.data*)          
    }

    .bss :
    {
        *(.bss)
        *(.bss*)
    }
} 