#ifndef  __MASK_API_H__
#define  __MASK_API_H__

struct arch_info_t {
    u8 enable_psram;
    u8 psram_mode;
    u16 psram_div;
    u32 hsb_div: 4;
    u32 sdram_ref_clk: 28;

    u32 sdram_clk;
    u32 sdram_size;
    u32 sdram_refresh_time;
    u32 sdram_refresh_cycles;

    u32 sdram_trfc: 8;
    u32 sdram_trp: 8;
    u32 sdram_trcd: 8;
    u32 sdram_trrd : 8;

    u32 sdram_twtr : 8;
    u32 sdram_trtw : 8;
    u32 sdram_twr  : 8;
    u32 sdram_trc  : 8;

    u32 sdram_mode: 2;
    u32 sdram_colum: 1;
    u32 sdram_cl: 4;
    u32 sdram_wlcnt: 4;
    u32 sdram_rlcnt: 4;
    u32 sdram_d_dly: 1;
    u32 sdram_q_dly: 1;
    u32 sdram_phase: 2;
    u32 sdram_ve_sel: 1;
    u32 enable_sdram: 1;
    u32 sdram_dq_dly_trm: 4;
    u32 sdram_dqs_dly_trm: 4;
    u32 dummy : 3;

    u32 sdram_udqs_dly_trm: 6;
    u32 sdram_ldqs_dly_trm: 6;
    u32 sdram_dq_dly_inner_trm: 4;
    u32 dummy_1 : 32 - 6 - 6 - 4;
};

//void mask_init(void *exp_hook);
void exception_analyze(u32 *sp);

static void uboot_mask_init(void *pchar, void *exp_hook)
{
}
void chip_reset();
#endif  /*MASK_API_H*/

