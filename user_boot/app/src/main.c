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
#include "norflash.h"

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

__attribute__((noreturn))
int main(void)
{

#ifdef __DEBUG
    uboot_mask_init(putchar, exception_analyze);
#endif

    u8 val[16];
    u8 *ptr = jlfs_get_isd_cfg_ptr();
    memset(val, 0, sizeof(val));
    dec_isd_cfg_ini("PLL_SRC", val, ptr);
    if (strcmp((char *)val, "LRC") == 0) {
        sys_clk_init(0, 48000000);
    } else {
        sys_clk_init(24000000, 48000000);
    }

#ifdef __DEBUG
    u32 ut_buad = 0;
    char uttx[8] = {0};
    memset(uttx, 0, sizeof(uttx));
    dec_isd_cfg_ini("UTTX", uttx, ptr);
    dec_isd_cfg_ini("UTBD", &ut_buad, ptr);

    uart_init(uttx, ut_buad);
    /* uart_init("USBDP", 1000000); //debug串口 */
    /* printf("debug IO:%s,buad:%d\n",uttx,ut_buad); */
#endif

    irq_init();

    log_info("******************  BootLoader *****************\n\n");

    arch_init();

#if 0//写保护示例
    struct  flash_info_t *flash_info = get_flash_info();
    u32 flash_id = flash_info->id;
    flash_set_wp(flash_id, 128);
#endif

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
        asm("nop");
    }

    return 0;
}

