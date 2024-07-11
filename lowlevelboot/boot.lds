OUTPUT_ARCH( "riscv" )

ENTRY( _start )

SECTIONS
{
    . = 0x80000000;
    .text :
    {
        KEEP(*(.text))
    }

    .data :
    {
        KEEP(*(.data))
    }
}
