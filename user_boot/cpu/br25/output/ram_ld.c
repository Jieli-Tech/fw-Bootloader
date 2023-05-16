// *INDENT-OFF*
//
#include  "maskrom_stubs.ld"

MEMORY
{
	text_ram    :   ORIGIN = ENTRY_ADDR,  LENGTH = 32k
	version_ram :   ORIGIN = 0x8000000,  LENGTH = 128
}

ENTRY(_start);
EXTERN(lib_uboot_version);

SECTIONS
{
    . = ORIGIN(text_ram);
    .text :
    {
        *(.start*)
        *(.text*)
        *(.data*)
        *(.rodata*)
        . = ALIGN(32);
    } > text_ram

    .bss ALIGN(32) (NOLOAD):
    {
        *(.stack*)
        *(.bss)
        *(.usb_ep0)
        *(.usb_config_var)
        . = ALIGN(32);
    } > text_ram

    .bss_noinit ALIGN(32) (NOLOAD):
    {
        *(.bss_noinit)
        . = ALIGN(32);
    } > text_ram


    bss_begin  = ADDR(.bss);
    bss_size   = SIZEOF(.bss);

	. = ORIGIN(version_ram);
    .version ALIGN(32):
	{
        *(.uboot.version)
        . = ALIGN(32);
	} > version_ram


}


