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
#include "mask_api.h"

#define LOG_TAG_CONST       BOOT
#define LOG_TAG             "[main]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#include "log.h"

#ifdef __DEBUG
__attribute__((weak))
void exception_analyze(u32 *sp)
{
    printf("%s() %s %d\n", __func__, __FILE__, __LINE__);
    unsigned int reti = sp[16];
    unsigned int rete = sp[17];
    unsigned int retx = sp[18];
    unsigned int rets = sp[19];
    unsigned int psr  = sp[20];
    unsigned int icfg = sp[21];
    unsigned int usp  = sp[22];
    unsigned int ssp  = sp[23];
    log_error("reti %x\n", reti);
    log_error("rets %x\n", rets);
    log_error("ssp %x\n", ssp);
    log_error("usp %x\n", usp);
    //Dump R0 ~ R15
    for (int i = 0; i < 16; ++i) {
        printf("R%d: %x\n", i, sp[i]);
    }
    while (1);
}
#endif

int main(void)
{

#ifdef __CPU_br23
    u32 spi_get_port();
    if (spi_get_port() == 0) {
        JL_PORTA->PU &= ~BIT(13);
        JL_PORTA->PD &= ~BIT(13);
        JL_PORTA->DIE &= ~BIT(13);
    } else {
        JL_PORTD->PU &= ~BIT(3);
        JL_PORTD->PD &= ~BIT(3);
        JL_PORTD->DIE &= ~BIT(3);
    }
#endif // __CPU_br23

#ifdef __DEBUG
    uboot_mask_init(putchar, exception_analyze);
#endif


#ifdef __DEBUG
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
    uart_init(uttx, ut_buad);
    /* uart_init("USBDP", 1000000); //debug串口 */
    /* printf("debug IO:%s,buad:%d\n",uttx,ut_buad); */

#else

    sys_clk_init(24000000, 48000000);
    // uart_init("PB02", 1000000);

#endif

    irq_init();

    log_info("******************  BootLoader *****************\n\n");

    arch_init();


    u8 err = jlfs_mount();//非0表示fs初始化失败，可能是升级未完成
    if (err) {
        log_error("jlfs_mount err:%d\n", err);
    }


#if(USE_UPGRADE_MAGIC)
    if (uboot_check_upgrade_magic() || err) {
        user_check_upgrade(err);
    }
#endif

#if 0
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

