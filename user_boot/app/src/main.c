#include "common.h"
#include "clock.h"
#include "irq.h"
#include "wdt.h"
#include "uart.h"
#include "printf.h"
#include "setup.h"
#include "dec.h"
#include "jlfs.h"
#include "delay.h"
#include "upgrade.h"
#include "user.h"



int main(void)
{
#if 1
    u8 val[16];
    u8 pll_scr = 0;
    u8 *ptr = jlfs_get_isd_cfg_ptr();
    memset(val, 0, sizeof(val));
    dec_isd_cfg_ini("PLL_SRC", val, ptr);
    if (strcmp((char *)val, "LRC") == 0) {
        pll_scr = 2;
    }
    u32 ut_buad = 0;
    char uttx[8] = {0};
    memset(uttx, 0, sizeof(uttx));
    dec_isd_cfg_ini("UTTX", uttx, ptr);
    dec_isd_cfg_ini("UTBD", &ut_buad, ptr);

    if (pll_scr == 2) {
        sys_clk_init(0, 48000000);
    } else {
        sys_clk_init(24000000, 48000000);
    }
    /* uart_init(uttx, ut_buad); */
    uart_init("PA05", 1000000); //debug串口
    /* printf("debug IO:%s,buad:%d\n",uttx,ut_buad); */

#else

    sys_clk_init(24000000, 48000000);
    uart_init("PA05", 1000000);

#endif

    log_info("\n******************  user uboot  *****************\n\n");

    irq_init();
    arch_init();

    u8 err = jlfs_mount();//非0表示fs初始化失败，可能是升级未完成
    printf("jlfs_mount err:%d\n", err);

#if 1
    JL_PORTA->DIR |= BIT(6);
    JL_PORTA->DIE |= BIT(6);
    JL_PORTA->PU |= BIT(6);
    udelay(100);
    if (((JL_PORTA->IN & BIT(6)) == 0) || err) {
        user_check_upgrade(err);
    }
#endif

    jl_check_upgrade(err);

    sfc_mode_boot();

__boot_failed:

    while (1) {
        wdt_clr();
    }

    return 0;
}

