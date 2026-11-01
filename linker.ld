ENTRY(kmain)			/* Точка входа в ядро */
SECTIONS {
	. = 0x200000;		/* Ядро будет загружено в 0x10000*/	

    .text : { *(.text*) }
	.data : { *(.data*) }
    .bss  : { *(.bss*)  }
}
