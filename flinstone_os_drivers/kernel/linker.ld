OUTPUT_FORMAT(elf32-i386)
ENTRY(kmain)
SECTIONS {
    . = 0x00100000;
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss COMMON : { *(.bss COMMON) }
}