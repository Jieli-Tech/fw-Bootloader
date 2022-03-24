#ifndef  __UPGRADE_H__
#define  __UPGRADE_H__


#include "common.h"


void *get_sdk_swap_addr();
void uart_upgrade(const char *utrx, u32 baud);
u8 update_loop_run(u32 flash_ota_addr);
u8 jl_check_upgrade(u8 jlfs_err);


#endif

