#include "common.h"
#include "norflash.h"
#include "delay.h"
#include "printf.h"


const struct flash_wp_arg wp[] = {
    {
        // W25Q64JV-0x909294
        .id = 0x909294,
        .split_mode = 1,
        .write_en_use_50h = 0,
        .res = 0,
        .numOfwp_array = 2,
        .wp_array[0].wp_addr = 1024,
        .wp_array[0].sr1 = 0x30,
        .wp_array[0].sr2 = 0x0,
        .wp_array[1].wp_addr = 2048,
        .wp_array[1].sr1 = 0x34,
        .wp_array[1].sr2 = 0x0,
    },
};
void flash_set_wp(u32 flash_id, u32 addr)//addr 是想要保护的地址
{
    const struct flash_wp_arg *p = NULL;
    for (int i = 0 ; i < ARRAY_SIZE(wp); i++) {
        if (flash_id == wp[i].id) {
            p = (const struct flash_wp_arg *)&wp[i];
        }
    }
    norflash_write_protect_config(addr * 1024, p);

}

