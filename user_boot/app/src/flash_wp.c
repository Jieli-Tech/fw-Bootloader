#include "common.h"
#include "norflash.h"
#include "delay.h"
#include "printf.h"

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
const struct flash_wp_arg wp[] = {
    {
        // 4_PS P25Q40HDWF-0x856013
        .id = 0x856013,
        .split_mode = 1,
        .write_en_use_50h = 0,
        .res = 0,
        .numOfwp_array = 2,
        .wp_array[0].wp_addr = 64,
        .wp_array[0].sr1 = 0x24,
        .wp_array[0].sr2 = 0x0,
        .wp_array[1].wp_addr = 128,
        .wp_array[1].sr1 = 0x28,
        .wp_array[1].sr2 = 0x0,
    },
};

void flash_set_wp(u32 flash_id, u32 addr)
{
    const struct flash_wp_arg *p = NULL;
    for (int i = 0 ; i < ARRAY_SIZE(wp); i++) {
        if (flash_id == wp[i].id) {
            p = (const struct flash_wp_arg *)&wp[i];
        }
    }
    norflash_write_protect_config(addr * 1024, p);

}

