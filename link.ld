ENTRY(_start)

SECTIONS {
    . = 0x10000;        /* VersatilePB loads kernel at 0x10000 */

    .text : {
        *(.text*)
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
