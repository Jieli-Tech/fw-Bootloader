#ifndef  __UPGRADE_H__
#define  __UPGRADE_H__


#include "common.h"

struct compress_head {
    u16 crc_head;
    u16 crc_data;
    u32 originalSize;
    u32 run_addr;
    u32 arg;//ota_loader end addr
    char name[16];
};


void *get_sdk_swap_addr();
void uart_upgrade(const char *utrx, u32 baud);
u8 update_loop_run(u32 flash_ota_addr);
u8 jl_check_upgrade(u8 jlfs_err);
u32 upgrade_run(void *boot, struct compress_head *cmp_hd);



#endif

