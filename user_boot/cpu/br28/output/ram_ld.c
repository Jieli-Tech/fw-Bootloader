// *INDENT-OFF*
//
/*
#-------------------------------------------------------+
#               br28 RAM mapping                        |
#-------------------------------------------0x1A0000 ---|       
#                                             (0x200)   |       INT_ADDR
#-------------------------------------------0X19FE00----|---------------
#              Maskrom_export_RAM             (0x200)   |       0X19FE00
#-------------------------------------------0x19FC00 ---|
#                                                       |
#                                                       |
#                                                       |
#                                                       |
#              (reserved)              (623K:0x9BC00)   |
#                                                       |
#                                                       |
#                                                       |
#                                                       |
#-------------------------------------------0x104000 ---|
#                                                       |
#              *uboot                   (16K:0x4000)    |
#                                                       |
#-------------------------------------------0x100000 ---|
*/

#include  "maskrom_stubs.ld"

#define SUPPORT_BP_DEBUG 0

#if SUPPORT_BP_DEBUG == 1
#include  "breakpoint_flash_stubs.ld"
/* 断点功能初始化 */

//******************移植注意点**********************************
/*放到sbc的ram*/
BP_BEG 		= 0x1a0000;
/* 需要计算是否能够容纳加入的调试程序 */
BP_SIZE 		= 0x2d0;
//******************移植注意点END**********************************

/* 断点功能end */
#endif /* #if SUPPORT_BP_DEBUG == 1 */

//from mask export
ISR_BASE       = _IRQ_MEM_ADDR;

MEMORY
{
	text_ram    :   ORIGIN = ENTRY_ADDR,  LENGTH = 16K
#if SUPPORT_BP_DEBUG == 1
	bp_ram(rwx) : ORIGIN = BP_BEG,  LENGTH = BP_SIZE
#endif /* #if SUPPORT_BP_DEBUG == 1 */
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

/*SUPPORT_BP_EXTENSIONS */
#if SUPPORT_BP_DEBUG == 1
/* 断点功能初始化 */

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

#endif /* #if SUPPORT_BP_DEBUG == 1 */
}


