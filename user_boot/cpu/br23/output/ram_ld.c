// *INDENT-OFF*
//
#include  "maskrom_stubs.ld"

#ifdef SUPPORT_BP_DEBUG
#include  "breakpoint_flash_stubs.ld"
#endif/* #ifdef SUPPORT_BP_DEBUG */

MEMORY
{
	text_ram    :   ORIGIN = ENTRY_ADDR,  LENGTH = 20k
#ifdef SUPPORT_BP_DEBUG
	bp_ram(rwx) : ORIGIN = BP_BEG,  LENGTH = BP_SIZE
#endif/* #ifdef SUPPORT_BP_DEBUG */
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
    .version ALIGN(32) (NOLOAD):
	{
        *(.uboot.version)
        . = ALIGN(32);
	} > version_ram

/* 断点功能初始化 */
#ifdef SUPPORT_BP_DEBUG

    . = ORIGIN(bp_ram);
	.bp_code :SUBALIGN(4)
    {
        *(.bp_ram_code)
        *(.bp_ram_data)
        *(.bp_ram_const)
        . = ALIGN(4);
    } > bp_ram

    .bp_ram_bss(NOLOAD) :SUBALIGN(4)
    {
        *(.bp_ram_bss)
    } > bp_ram

    _text_begin = ADDR(.text);
    _text_size = SIZEOF(.text);

    _bp_bss_begin = ADDR(.bp_ram_bss);
    _bp_bss_size  = SIZEOF(.bp_ram_bss);

    _bp_begin = ADDR(.bp_code);
    _bp_size  = SIZEOF(.bp_code);


//******************移植注意点**********************************
    _bp_store_addr = _text_begin + _text_size;
//******************移植注意点END**********************************
/* 断点功能end */
#endif/* #ifdef SUPPORT_BP_DEBUG */
}


