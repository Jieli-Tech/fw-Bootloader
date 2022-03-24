#ifndef  __MASK_API_H__
#define  __MASK_API_H__

struct psram_config {
    u8 enable_psram;
    u8 psram_mode;
    u16 psram_div;
};

struct arch_info_t {
    struct psram_config psram;
};

//void mask_init(void *exp_hook);
void exception_analyze(u32 *sp);

void mask_init(void *f, void *m, void *stack_free, void *stack_malloc,
               void *suppress_tick_and_sleep,
               void *StartFirstTask,
               void *exp_hook, void *i_hook,
               void *pchar, void *log_e);
static void uboot_mask_init(void *pchar, void *exp_hook)
{
    mask_init(NULL, NULL, NULL, NULL,
              NULL,
              NULL,
              exp_hook, NULL,
              putchar, NULL);
}
void chip_reset();
#endif  /*MASK_API_H*/

