#ifndef  __NORFLASH_H__
#define  __NORFLASH_H__

#include "common.h"


#define WINBOND_WRITE_ENABLE                                0x06
#define WINBOND_JEDEC_ID                                    0x9F
#define WINBOND_UUID                                        0x4B
#define WINBOND_READ_SR1                                    0x05
#define WINBOND_READ_SR2                                    0x35
#define WINBOND_WRITE_SR1                                   0x01
#define WINBOND_WRITE_SR2                                   0x31
#define WINBOND_FAST_READ                                   0x0B
#define WINBOND_FAST_READ_DUAL_OUTPUT                       0x3B
#define WINBOND_FAST_READ_QUAD_OUTPUT                       0x6B
#define WINBOND_FAST_READ_DUAL_IO                           0xBB
#define WINBOND_FAST_READ_QUAD_IO                           0xEB
#define WINBOND_CONTINUOUS_READ_NORMAL_MODE                 0x00
#define WINBOND_CONTINUOUS_READ_ENHANCE_MODE                0x20
#define WINBOND_PAGE_PROGRAM                                0x02
#define WINBOND_PAGE_ERASE                                  0x81
#define WINBOND_SECTOR_ERASE                                0x20
#define WINBOND_BLOCK_ERASE                                 0xD8
#define WINBOND_CHIP_ERASE                                  0xC7

struct flash_info_t {
    u8 uuid[16];
    u32 id;
    u32 capacity;
};
struct flash_wp_arg {
    u32 id;
    u8 split_mode: 1;		//0 连续写，1，分开写
    u8 write_en_use_50h: 1;	//0:06H		1:50H
    u8 res: 2;
    u8 numOfwp_array: 4;			//写保护参数的个数
    struct {
        u8 sr1;
        u8 sr2;
        u16 wp_addr;	//写保护结束地址,单位K
    } wp_array[15];
};
struct flash_info_t *get_flash_info(void);
u32 get_flash_capacity(void);

void norflash_init(u8 width, u8 clk_div, u8 read_mode);
void norflash_read(u8 *buf, u32 addr, u32 len);
u8 norflash_write(u8 *buf, u32 addr, u32 len);
u8 norflash_eraser(u32 eraser_cmd, u32 addr);
int norflash_write_protect_config(u32 addr, const struct flash_wp_arg *p);//addr传0地址时，解除写保护
void flash_set_wp(u32 flash_id, u32 addr);
u32 spi_get_port();
u16 norflash_cmd_custom(void);

#define     SPI_MODE_MASK   0x7

#define     SPI_2WIRE_MODE  0
#define     SPI_ODD_MODE    1
#define     SPI_DUAL_MODE   2
#define     SPI_QUAD_MODE   4

#define     FLASH_MODE_MASK (0x3<<2)

#define     FAST_READ_OUTPUT_MODE   (0)
#define     FAST_READ_IO_MODE       (1)
#define     FAST_READ_IO_CONTINUOUS_READ_MODE   (2)

#define     SPI_CS_IDLE       1
#define     SPI_CS_ACTIVE     0

#define     SFC_DEC_MODE    BIT(2)
#define     SPI_DEC_MODE    BIT(1)
#define     SPI_CRC_INIT    BIT(0)

#endif

