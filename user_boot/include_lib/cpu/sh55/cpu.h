#ifndef _DV15_
#define _DV15_

/***************************************************************************
 *                            sfr define
 ***************************************************************************/
#define ls_io_base      0x90000
#define ls_tmr_base     0x90400
#define ls_uart_base    0x90800
#define ls_spi_base     0x90c00
#define ls_sd_base      0x91000
#define ls_adda_base    0x91400
#define ls_clk_base     0x91800
#define ls_ana_base     0x91c00
#define ls_mtpwm_base   0x92000
#define ls_usb_base     0x92400
#define ls_alink_base   0x92800
#define ls_tdm_base     0x92c00
#define ls_plnk_base    0x93000
#define ls_oth_base     0x93c00

#define xsfr_base          0x1fc00                //xsb
#define hsfr_base          0x80000                //hsb
//#define psfr_base          0x90000               //port
//#define bsfr_base          0xa0000               //bt; wire less
//#define lsfr_base          0xb0000               //lsb

#define __RW      volatile       // read write
#define __RO      volatile const // only read
#define __WO      volatile       // only write

#define __u8      unsigned int   // u8  to u32 special for struct
#define __u16     unsigned int   // u16 to u32 special for struct
#define __u32     unsigned int

#define map_adr(grp, adr)  ((64 * grp + adr) *  4) //grp(0x0-0xff), adr(0x0-0x3f)

//================================================================================//
//
//     low speed sfr address define
//
//===============================================================================//

typedef struct {
    __RW __u16 OUT;
    __RW __u16 IN;
    __RW __u16 DIR;
    __RW __u16 DIE;
    __RW __u16 PU;
    __RW __u16 PD;
    __RW __u16 HD0;
    __RW __u16 HD1;
    __RW __u16 DIEH;
} JL_PORT_TypeDef;

#define JL_PORTA_BASE                     (ls_io_base + map_adr(0x00, 0x00))
#define JL_PORTA                          ((JL_PORT_TypeDef    *)JL_PORTA_BASE)

#define JL_PORTB_BASE                     (ls_io_base + map_adr(0x00, 0x09))
#define JL_PORTB                          ((JL_PORT_TypeDef    *)JL_PORTB_BASE)

#define JL_PORTD_BASE                     (ls_io_base + map_adr(0x00, 0x12))
#define JL_PORTD                          ((JL_PORT_TypeDef    *)JL_PORTD_BASE)

typedef struct {
    __RW __u16 IOMC0;
    __RW __u16 IOMC1;
    __WO __u16 IOMC2;
    __RW __u16 IOMC3;
    //__RW __u16 USB_IO_CON0;//nouse
    __RW __u16 DAC_IO_CON0;
} JL_IOMC_TypeDef;

#define JL_IOMC_BASE                     (ls_io_base + map_adr(0x00, 0x20))
#define JL_IOMC                          ((JL_IOMC_TypeDef    *)JL_IOMC_BASE)

typedef struct {
    __RW __u32 CON0;
    __RW __u32 CON1;
    __RW __u32 CON2;
    __RW __u32 CON3;
} JL_WAKEUP_TypeDef;

#define JL_WAKEUP_BASE               (ls_io_base + map_adr(0x00, 0x30))
#define JL_WAKEUP                    ((JL_WAKEUP_TypeDef            *)JL_WAKEUP_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 CNT;
    __RW __u16 PRD;
} JL_TMR0_TypeDef;

#define JL_TMR0_BASE                     (ls_tmr_base + map_adr(0x00, 0x00))
#define JL_TMR0                          ((JL_TMR0_TypeDef    *)JL_TMR0_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 CNT;
    __RW __u16 PRD;
} JL_TMR1_TypeDef;

#define JL_TMR1_BASE                     (ls_tmr_base + map_adr(0x00, 0x03))
#define JL_TMR1                          ((JL_TMR1_TypeDef    *)JL_TMR1_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 CNT;
    __RW __u16 PRD;
    __RW __u16 PWM0;
    __RW __u16 PWM1;
} JL_TMR2_TypeDef;

#define JL_TMR2_BASE                     (ls_tmr_base + map_adr(0x00, 0x08))
#define JL_TMR2                          ((JL_TMR2_TypeDef    *)JL_TMR2_BASE)

typedef struct {
    __RW __u16 CON0;
    __RW __u16 CON1;
    __RW __u16 CON2;
    __u32 RESERVED0[0x5 - 0x2 - 1];
    __RW __u16 GAT0;
} JL_CLK_TypeDef;

#define JL_CLK_BASE                     (ls_clk_base + map_adr(0x00, 0x00))
#define JL_CLK                          ((JL_CLK_TypeDef    *)JL_CLK_BASE)

typedef struct {
    __RO __u8  SRC;
} JL_RST_TypeDef;

#define JL_RST_BASE                     (ls_clk_base + map_adr(0x00, 0x07))
#define JL_RST                          ((JL_RST_TypeDef    *)JL_RST_BASE)

typedef struct {
    __WO __u32 CON0;
    __WO __u32 CON1;
} JL_PLL_TypeDef;

#define JL_PLL_BASE                       (ls_clk_base + map_adr(0x00, 0x08))
#define JL_PLL                            ((JL_PLL_TypeDef      *)JL_PLL_BASE)

typedef struct {
    __WO __u16 LDO_CON;
    __RW __u16 LVD_CON;
    __RW __u32 HTC_CON;
} JL_ANA_TypeDef;

#define JL_ANA_BASE                     (ls_ana_base + map_adr(0x00, 0x00))
#define JL_ANA                          ((JL_ANA_TypeDef    *)JL_ANA_BASE)

typedef struct {
    __RW __u16 DAC_CON0;
    __RW __u16 DAC_CON1;
    __WO __u16 DAC_TRM;
    __WO __u16 DAC_ADR;
    __WO __u16 DAC_LEN;
    __WO __u8  DAC_COP;
    __WO __u8  DAC_DTB;
    __RW __u16 ADC_CON;
    __WO __u16 ADC_ADR;
    __WO __u16 ADC_LEN;
    __RW __u8  RAM_SPD;
    __RW __u16 DAA_CON0;
    __RW __u16 DAA_CON1;
    __RW __u16 ADA_CON0;
    __RW __u16 ADA_CON1;
    __RW __u16 ADA_CON2;
} JL_ADDA_TypeDef;

#define JL_ADDA_BASE                     (ls_adda_base + map_adr(0x00, 0x00))
#define JL_ADDA                          ((JL_ADDA_TypeDef    *)JL_ADDA_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 DAT;
} JL_GPADC_TypeDef;

#define JL_GPADC_BASE                     (ls_adda_base + map_adr(0x00, 0x10))
#define JL_GPADC                          ((JL_GPADC_TypeDef    *)JL_GPADC_BASE)

typedef struct {
    __RW __u16 CON0;
    __RW __u16 CON1;
    __RW __u8  CON2;
    __RW __u8  CON3;
    __WO __u32 ADR0;
    __WO __u32 ADR1;
    __WO __u32 ADR2;
    __WO __u32 ADR3;
    __WO __u16 LEN;
} JL_ALNK_TypeDef;

#define JL_ALNK_BASE                     (ls_alink_base + map_adr(0x00, 0x00))
#define JL_ALNK                          ((JL_ALNK_TypeDef    *)JL_ALNK_BASE)

typedef struct {
    __RW __u32 CON0;
    __RW __u32 WCLK_DUTY;
    __RW __u32 WCLK_PRD;
    __RW __u32 DMA_BASE_ADR;
    __WO __u32 DMA_BUF_LEN;
    __WO __u32 SLOT_EN;
} JL_TDM_TypeDef;

#define JL_TDM_BASE                     (ls_tdm_base + map_adr(0x00, 0x00))
#define JL_TDM                          ((JL_TDM_TypeDef    *)JL_TDM_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 STA;//without this register
    __RW __u16 BUF;
    __WO __u16 ADR;
    __WO __u16 CNT;
    __WO __u16 BAUD;
} JL_SPI_TypeDef;

#define JL_SPI0_BASE                     (ls_spi_base + map_adr(0x00, 0x00))
#define JL_SPI0                          ((JL_SPI_TypeDef    *)JL_SPI0_BASE)

#define JL_SPI1_BASE                     (ls_spi_base + map_adr(0x00, 0x08))
#define JL_SPI1                          ((JL_SPI_TypeDef    *)JL_SPI1_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 BUF;
    __WO __u16 BAUD;
} JL_UT0_TypeDef;

#define JL_UT0_BASE                     (ls_uart_base + map_adr(0x00, 0x00))
#define JL_UT0                          ((JL_UT0_TypeDef    *)JL_UT0_BASE)

typedef struct {
    __RW __u32 HRXCNT;
    __WO __u32 OTCNT;
    __WO __u32 TXADR;        //26bit write only
    __WO __u32 TXCNT;
    __WO __u32 RXEADR;       //26bit write only
    __RW __u16 CON0;
    __RW __u16 CON1;
    __RW __u16 CON2;
    __RW __u8  BUF;
    __WO __u16 BAUD;         //16bit write only
    __WO __u32 RXSADR;       //26bit write only
    __RW __u32 RXCNT;
} JL_UT1_TypeDef;

#define JL_UT1_BASE                     (ls_uart_base + map_adr(0x00, 0x04))
#define JL_UT1                          ((JL_UT1_TypeDef    *)JL_UT1_BASE)

typedef struct {
    __RW __u16 CON0;
    __RW __u16 CON1;
    __RW __u16 CON2;
    __WO __u16 CPTR;
    __WO __u16 DPTR;
} JL_SD_TypeDef;

#define JL_SD0_BASE                     (ls_sd_base + map_adr(0x00, 0x00))
#define JL_SD0                          ((JL_SD_TypeDef    *)JL_SD0_BASE)

typedef struct {
    __RW __u16 TMR0_CON;
    __RW __u16 TMR1_CON;
    __RW __u16 TMR2_CON;
    __RW __u16 TMR3_CON;
    __RW __u16 TMR0_CNT;
    __RW __u16 TMR1_CNT;
    __RW __u16 TMR2_CNT;
    __RW __u16 TMR3_CNT;
    __RW __u16 TMR0_PR;
    __RW __u16 TMR1_PR;
    __RW __u16 TMR2_PR;
    __RW __u16 TMR3_PR;
    __RW __u16 CH0_CMP;
    __RW __u16 CH1_CMP;
    __RW __u16 CH2_CMP;
    __RW __u16 CH3_CMP;
    __RW __u16 PWMCON0;
    __RW __u32 PWMCON1;
} JL_MCPWM_TypeDef;

#define JL_MCPWM_BASE                     (ls_mtpwm_base + map_adr(0x00, 0x00))
#define JL_PWM                            ((JL_MCPWM_TypeDef    *)JL_MCPWM_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u8  SMR;
    __RW __u32 ADR;
    __RW __u32 LEN;
    __RW __u16 DOR;
    __RW __u32 CON1;

} JL_PLNK_TypeDef;

#define JL_PLNK_BASE                    (ls_plnk_base + map_adr(0x00, 0x00))
#define JL_PLNK                         ((JL_PLNK_TypeDef     *)JL_PLNK_BASE)


typedef struct {
    __WO __u16 FIFO;
    __RW __u16 REG;
} JL_CRC_TypeDef;

#define JL_CRC_BASE                     (ls_oth_base + map_adr(0x00, 0x00))
#define JL_CRC                          ((JL_CRC_TypeDef    *)JL_CRC_BASE)

typedef struct {
    __RW __u16 CON;
    __RW __u16 VAL;//value
} JL_PLLCNT_TypeDef;

#define JL_PLLCNT_BASE                  (ls_oth_base + map_adr(0x00, 0x05))
#define JL_PLLCNT                       ((JL_PLLCNT_TypeDef    *)JL_PLLCNT_BASE)

typedef struct {
    __RW __u16 CON;
} JL_PWR_TypeDef;

#define JL_PWR_BASE                     (ls_oth_base + map_adr(0x00, 0x07))
#define JL_PWR                          ((JL_PWR_TypeDef    *)JL_PWR_BASE)

typedef struct {
    __RW __u16 CON;
} JL_WDT_TypeDef;

#define JL_WDT_BASE                     (ls_oth_base + map_adr(0x00, 0x08))
#define JL_WDT                          ((JL_WDT_TypeDef    *)JL_WDT_BASE)

typedef struct {
    __RW __u16 CON;
} JL_IRFLT_TypeDef;

#define JL_IRFLT_BASE                     (ls_oth_base + map_adr(0x00, 0x09))
#define JL_IRFLT                          ((JL_IRFLT_TypeDef    *)JL_IRFLT_BASE)

typedef struct {
    __RW __u16 CON;
} JL_ISP_TypeDef;

#define JL_ISP_BASE                     (ls_oth_base + map_adr(0x00, 0x0a))
#define JL_ISP                          ((JL_ISP_TypeDef    *)JL_ISP_BASE)


typedef struct {
    __WO __u16 CON;
} JL_SSI_TypeDef;

#define JL_SSI_BASE                     (ls_oth_base + map_adr(0x00, 0x0b))
#define JL_SSI                          ((JL_SSI_TypeDef    *)JL_SSI_BASE)

typedef struct {
    __RW __u16 CON;
} JL_MODE_TypeDef;

#define JL_MODE_BASE                     (ls_oth_base + map_adr(0x00, 0x0c))
#define JL_MODE                          ((JL_MODE_TypeDef    *)JL_MODE_BASE)

typedef struct {
    __RW __u16 CON;
} JL_EFUSE_TypeDef;

#define JL_EFUSE_BASE                    (ls_oth_base + map_adr(0x00, 0x0d))
#define JL_EFUSE                         ((JL_EFUSE_TypeDef    *)JL_EFUSE_BASE)

typedef struct {
    __RW __u16 ID;
} JL_CHIP_TypeDef;

#define JL_CHIP_BASE                     (ls_oth_base + map_adr(0x00, 0x0e))
#define JL_CHIP                          ((JL_CHIP_TypeDef    *)JL_CHIP_BASE)

typedef struct {
    __RW __u32 IIC_CON;
    __RW __u32 IIC_BAUD;
    __RW __u8  IIC_BUF;
} JL_IIC_TypeDef;

#define JL_IIC_BASE                     (ls_oth_base + map_adr(0x00, 0x0f))
#define JL_IIC                          ((JL_IIC_TypeDef    *)JL_IIC_BASE)

typedef struct {
    __RW __u32 CON;
    __RO __u32 NUM;
} JL_GPCNT_TypeDef;

#define JL_GPCNT_BASE                   (ls_oth_base + map_adr(0x00, 0x12))
#define JL_GPCNT                        ((JL_GPCNT_TypeDef    *)JL_GPCNT_BASE)

typedef struct {
    __WO __u32 CON;
    __RW __u32 NUM;
} JL_LRCT_TypeDef;

#define JL_LRCT_BASE                    (ls_oth_base + map_adr(0x00, 0x14))
#define JL_LRCT                         ((JL_LRCT_TypeDef     *)JL_LRCT_BASE)

typedef struct {
    __RW __u8  CON ;
    __WO __u16 KEY ;
    __WO __u16 ADR ;
} JL_PERIENC_TypeDef;

#define JL_PERIENC_BASE                 (ls_oth_base + map_adr(0x00, 0x16))
#define JL_PERIENC                      ((JL_PERIENC_TypeDef *)JL_PERIENC_BASE)


typedef struct {
    __RW __u32 LP_CON;
    __RW __u32 WKUP_CON;
    __RW __u32 SPI_CON;
    __RW __u8  SPI_DAT;
} JL_P33_TypeDef;

#define JL_P33_BASE                     (ls_oth_base + map_adr(0x00, 0x20))
#define JL_P33                          ((JL_P33_TypeDef    *)JL_P33_BASE)

typedef struct {
    __WO __u16 TRI;
} JL_FPGA_TypeDef;

#define JL_FPGA_BASE                     (ls_oth_base + map_adr(0x00, 0xff))
#define JL_FPGA                          ((JL_FPGA_TypeDef    *)JL_FPGA_BASE)
/*
#define PA_OUT     	    (*(volatile u16 *)(ls_io_base + 0x00*4))         //
#define PA_IN     	    (*(volatile u16 *)(ls_io_base + 0x01*4))         //
#define PA_DIR    	    (*(volatile u16 *)(ls_io_base + 0x02*4))         //
#define PA_DIE    	    (*(volatile u16 *)(ls_io_base + 0x03*4))         //
#define PA_PU    	    (*(volatile u16 *)(ls_io_base + 0x04*4))         //
#define PA_PD    	    (*(volatile u16 *)(ls_io_base + 0x05*4))         //
#define PA_HD    	    (*(volatile u16 *)(ls_io_base + 0x06*4))         //

#define PB_OUT     	    (*(volatile u16 *)(ls_io_base + 0x08*4))         //
#define PB_IN     	    (*(volatile u16 *)(ls_io_base + 0x09*4))         //
#define PB_DIR    	    (*(volatile u16 *)(ls_io_base + 0x0a*4))         //
#define PB_DIE    	    (*(volatile u16 *)(ls_io_base + 0x0b*4))         //
#define PB_PU    	    (*(volatile u16 *)(ls_io_base + 0x0c*4))         //
#define PB_PD    	    (*(volatile u16 *)(ls_io_base + 0x0d*4))         //
#define PB_HD    	    (*(volatile u16 *)(ls_io_base + 0x0e*4))         //

#define PC_OUT     	    (*(volatile u16 *)(ls_io_base + 0x10*4))         //
#define PC_IN     	    (*(volatile u16 *)(ls_io_base + 0x11*4))         //
#define PC_DIR    	    (*(volatile u16 *)(ls_io_base + 0x12*4))         //
#define PC_DIE    	    (*(volatile u16 *)(ls_io_base + 0x13*4))         //
#define PC_PU    	    (*(volatile u16 *)(ls_io_base + 0x14*4))         //
#define PC_PD    	    (*(volatile u16 *)(ls_io_base + 0x15*4))         //
#define PC_HD    	    (*(volatile u16 *)(ls_io_base + 0x16*4))         //

#define IOMC0           (*(volatile u16 *)(ls_io_base + 0x20*4))         //
#define IOMC1           (*(volatile u16 *)(ls_io_base + 0x21*4))         //
#define IOMC2           (*(volatile u16 *)(ls_io_base + 0x22*4))         //
#define IOMC3           (*(volatile u16 *)(ls_io_base + 0x23*4))         //
#define TMR0_CON        (*(volatile u16 *)(ls_tmr_base + 0x00*4))         //
#define TMR0_CNT        (*(volatile u16 *)(ls_tmr_base + 0x01*4))         //
#define TMR0_PRD        (*(volatile u16 *)(ls_tmr_base + 0x02*4))         //

#define TMR2_CON        (*(volatile u16 *)(ls_tmr_base + 0x03*4))         //
#define TMR2_CNT        (*(volatile u16 *)(ls_tmr_base + 0x04*4))         //
#define TMR2_PRD        (*(volatile u16 *)(ls_tmr_base + 0x05*4))         //

#define TMR3_CON        (*(volatile u16 *)(ls_tmr_base + 0x08*4))         //
#define TMR3_CNT        (*(volatile u16 *)(ls_tmr_base + 0x09*4))         //
#define TMR3_PRD        (*(volatile u16 *)(ls_tmr_base + 0x0a*4))         //
#define TMR3_PWM0       (*(volatile u16 *)(ls_tmr_base + 0x0b*4))         //
#define TMR3_PWM1       (*(volatile u16 *)(ls_tmr_base + 0x0c*4))         //
#define CLK_CON0        (*(volatile u16 *)(ls_clk_base + 0x00*4))         //
#define CLK_CON1        (*(volatile u16 *)(ls_clk_base + 0x01*4))         //
//#define CLK_CON2        (*(volatile u16 *)(ls_clk_base + 0x02*4))         //
//#define CLK_CON3        (*(volatile u16 *)(ls_clk_base + 0x03*4))         //
//#define CLK_CON4        (*(volatile u16 *)(ls_clk_base + 0x04*4))         //
#define CLK_GAT0        (*(volatile u16 *)(ls_clk_base + 0x05*4))         //

#define PLL_CON         (*(volatile u32 *)(ls_clk_base + 0x08*4))         //
//#define PLL_CON1        (*(volatile u16 *)(ls_clk_base + 0x09*4))         //
//#define PLL_CON2        (*(volatile u16 *)(ls_clk_base + 0x0a*4))         //

#define LDO_CON		    (*(volatile u16 *)(ls_ana_base + 0x00*4))         //
#define LVD_CON         (*(volatile u16 *)(ls_ana_base + 0x01*4))         //
#define HTC_CON         (*(volatile u16 *)(ls_ana_base + 0x02*4))         //

#define DAC_CON         (*(volatile u16 *)(ls_adda_base + 0x00*4))         //
#define DAC_CON1        (*(volatile u16 *)(ls_adda_base + 0x01*4))         //
#define DAC_DIL         (*(volatile u16 *)(ls_adda_base + 0x02*4))         //
#define DAC_DIH         (*(volatile u16 *)(ls_adda_base + 0x03*4))         //
#define DAA_CON0        (*(volatile u16 *)(ls_adda_base + 0x04*4))         //
#define DAA_CON1        (*(volatile u16 *)(ls_adda_base + 0x05*4))         //

#define GPADC_CON       (*(volatile u16 *)(ls_adda_base + 0x08*4))         //
#define GPADC_DAT       (*(volatile u16 *)(ls_adda_base + 0x09*4))         //

#define SPI0_CON        (*(volatile u16 *)(ls_spi_base + 0x00*4))         //
#define SPI0_STA        (*(volatile u16 *)(ls_spi_base + 0x01*4))         //
#define SPI0_BUF        (*(volatile u16 *)(ls_spi_base + 0x02*4))         //
#define SPI0_ADR        (*(volatile u16 *)(ls_spi_base + 0x03*4))         //
#define SPI0_CNT        (*(volatile u16 *)(ls_spi_base + 0x04*4))         //
#define SPI0_BAUD       (*(volatile u16 *)(ls_spi_base + 0x05*4))         //

#define SPI1_CON        (*(volatile u16 *)(ls_spi_base + 0x08*4))         //
#define SPI1_STA        (*(volatile u16 *)(ls_spi_base + 0x09*4))         //
#define SPI1_BUF        (*(volatile u16 *)(ls_spi_base + 0x0a*4))         //
#define SPI1_ADR        (*(volatile u16 *)(ls_spi_base + 0x0b*4))         //
#define SPI1_CNT        (*(volatile u16 *)(ls_spi_base + 0x0c*4))         //
#define SPI1_BAUD       (*(volatile u16 *)(ls_spi_base + 0x0d*4))         //

#define UT0_CON         (*(volatile u16 *)(ls_uart_base + 0x00*4))         //
#define UT0_BUF         (*(volatile u16 *)(ls_uart_base + 0x01*4))         //
#define UT0_BAUD        (*(volatile u16 *)(ls_uart_base + 0x02*4))         //

#define UT1_HRXCNT      (*(volatile u32 *)(ls_uart_base + 0x04*4))
#define UT1_OTCNT       (*(volatile u32 *)(ls_uart_base + 0x05*4))
#define UT1_TXADR       (*(volatile u32 *)(ls_uart_base + 0x06*4))       //26bit write only;
#define UT1_TXCNT       (*(volatile u32 *)(ls_uart_base + 0x07*4))
#define UT1_RXEADR      (*(volatile u32 *)(ls_uart_base + 0x08*4))       //26bit write only;
#define UT1_CON0        (*(volatile u16 *)(ls_uart_base + 0x09*4))
#define UT1_CON1        (*(volatile u16 *)(ls_uart_base + 0x0a*4))
#define UT1_BUF         (*(volatile u8  *)(ls_uart_base + 0x0b*4))
#define UT1_BAUD        (*(volatile u16 *)(ls_uart_base + 0x0c*4))       //16bit write only;
#define UT1_RXSADR      (*(volatile u32 *)(ls_uart_base + 0x0d*4))       //26bit write only;
#define UT1_RXCNT       (*(volatile u32 *)(ls_uart_base + 0x0e*4))

#define SD0_CON0        (*(volatile u16 *)(ls_sd_base + 0x00*4))
#define SD0_CON1        (*(volatile u16 *)(ls_sd_base + 0x01*4))
#define SD0_CON2        (*(volatile u16 *)(ls_sd_base + 0x02*4))
#define SD0_CPTR        (*(volatile u16 *)(ls_sd_base + 0x03*4))
#define SD0_DPTR        (*(volatile u16 *)(ls_sd_base + 0x04*4))

#define MCTMR0_CON      (*(volatile u16 *)(ls_mtpwm_base + 0x00*4))
#define MCTMR0_CNT      (*(volatile u16 *)(ls_mtpwm_base + 0x01*4))
#define MCTMR0_PRD      (*(volatile u16 *)(ls_mtpwm_base + 0x02*4))
#define MCFPIN_CON      (*(volatile u32 *)(ls_mtpwm_base + 0x03*4))
#define MCCH0_CON0      (*(volatile u16 *)(ls_mtpwm_base + 0x04*4))
#define MCCH0_CON1      (*(volatile u16 *)(ls_mtpwm_base + 0x05*4))
#define MCCH0_CMP       (*(volatile u16 *)(ls_mtpwm_base + 0x06*4))
#define MCCH1_CON0      (*(volatile u16 *)(ls_mtpwm_base + 0x07*4))
#define MCCH1_CON1      (*(volatile u16 *)(ls_mtpwm_base + 0x08*4))       //16bit write only;
#define MCCH1_CMP       (*(volatile u16 *)(ls_mtpwm_base + 0x09*4))       //26bit write only;
#define MCCH2_CON0      (*(volatile u16 *)(ls_mtpwm_base + 0x0a*4))
#define MCCH2_CON1      (*(volatile u16 *)(ls_mtpwm_base + 0x0b*4))       //26bit write only;
#define MCCH2_CMP       (*(volatile u16 *)(ls_mtpwm_base + 0x0c*4))

#define CRC_FIFO        (*(volatile u16 *)(ls_oth_base + 0x00*4))         //
#define CRC_REG         (*(volatile u16 *)(ls_oth_base + 0x01*4))         //

#define RTC_BUF         (*(volatile u16 *)(ls_oth_base + 0x02*4))
#define RTC_CON0        (*(volatile u16 *)(ls_oth_base + 0x03*4))
#define RTC_CON1        (*(volatile u16 *)(ls_oth_base + 0x04*4))


#define PLLCNTCON       (*(volatile u16 *)(ls_oth_base + 0x05*4))
#define PLLCNTVAL       (*(volatile u16 *)(ls_oth_base + 0x06*4))

#define PWR_CON         (*(volatile u16 *)(ls_oth_base + 0x07*4))
#define WDT_CON         (*(volatile u16 *)(ls_oth_base + 0x08*4))
#define IRFLT_CON       (*(volatile u16 *)(ls_oth_base + 0x09*4))
#define ISP_CON         (*(volatile u16 *)(ls_oth_base + 0x0a*4))
#define SSI_CON         (*(volatile u16 *)(ls_oth_base + 0x0b*4))
#define MODE_CON        (*(volatile u16 *)(ls_oth_base + 0x0c*4))
#define EFUSE_CON       (*(volatile u16 *)(ls_oth_base + 0x0d*4))
#define CHIP_ID         (*(volatile u16 *)(ls_oth_base + 0x0e*4))
#define IIC_CON         (*(volatile u16 *)(ls_oth_base + 0x0f*4))
#define IIC_BAUD        (*(volatile u16 *)(ls_oth_base + 0x10*4))
#define IIC_BUF         (*(volatile u16 *)(ls_oth_base + 0x11*4))

#define FPGA_TRI        (*(volatile u16 *)(ls_oth_base + 0xff*4))
*/

//===============================================================================//
//
//      high speed sfr address define
//
//===============================================================================//
//............. 0x0000 - 0x01ff............// for csfr

//............. 0x0200 - 0x02ff............
typedef struct {
    __RW __u32 CON;
    __WO __u32 BAUD;
    __WO __u32 CODE;
    __WO __u32 BASE_ADR;
    __WO __u32 QU_CNT;
    __RW __u8  ENC_CON;
    __RW __u16 ENC_KEY;
    __WO __u32 UNENC_ADRH;
    __WO __u32 UNENC_ADRL;

} JL_SFC_TypeDef;

#define JL_SFC_BASE                     (hsfr_base + map_adr(0x02, 0x00))
#define JL_SFC                          ((JL_SFC_TypeDef    *)JL_SFC_BASE)

//............. 0x0400 - 0x04ff............
typedef struct {
    __RW __u32 CON;
    __RO __u32 HXSOFT_INT;
    __WO __u32 HXSOFT_SET;
    __WO __u32 HXSOFT_CLR;
    __RW __u32 DIF_UACNTL;
    __RW __u32 DIF_UACNTH;
    __RW __u32 DRD_UACNTL;
    __RW __u32 DRD_UACNTH;
    __RW __u32 DWR_UACNTL;
    __RW __u32 DWR_UACNTH;
} JL_HCORE_TypeDef;

#define JL_HCORE_BASE                   (hsfr_base + map_adr(0x04, 0x00))
#define JL_HCORE                        ((JL_HCORE_TypeDef    *)JL_HCORE_BASE)

typedef struct {
    __RW __u32 CON;
    __RW __u32 CP_ADRH;
    __RW __u32 CP_ADRL;
    __RW __u32 CP_BUF_LAST;
    __RW __u32 CPF_ADRH;
    __RW __u32 CPF_ADRL;
} JL_CACHE_TypeDef;

#define JL_CACHE_BASE                   (hsfr_base + map_adr(0x04, 0x10))
#define JL_CACHE                        ((JL_CACHE_TypeDef    *)JL_CACHE_BASE)

typedef struct {
    __RW __u32 BF_CON;
    __RW __u32 WR_EN;
    __RO __u32 MSG;
    __WO __u32 MSG_CLR;
    __RW __u32 CPU_PC_LIMH;
    __RW __u32 CPU_PC_LIML;
    __RW __u32 CPU_WR_LIMH;
    __RW __u32 CPU_WR_LIML;
    __RW __u32 PRP_WR_LIMH;
    __RW __u32 PRP_WR_LIML;
    __RO __u32 PRP_MMU_MSG;
    __RO __u32 PRP_MMU_MSG_CH;
    __RO __u32 PRP_WR_LIMIT_MSG;
    __RO __u32 PRP_WR_LIMIT_CH;
    __u32 RESERVED0[0x30 - 0x2d - 1];
    __RW __u32 CPU_RD_LIMH;
    __RW __u32 CPU_RD_LIML;
    __RW __u32 PRP_RD_LIMH;
    __RW __u32 PRP_RD_LIML;
    __u32 RESERVED1[0x38 - 0x33 - 1];
    __RO __u32 PRP_RD_LIMIT_MSG;
    __RO __u32 PRP_RD_LIMIT_CH;
} JL_DEBUG_TypeDef;

#define JL_DEBUG_BASE                   (hsfr_base + map_adr(0x04, 0x20))
#define JL_DEBUG                        ((JL_DEBUG_TypeDef  *)JL_DEBUG_BASE)

//............. 0x0500 - 0x05ff............
typedef struct {
    __RW __u32 CON0;
    __RW __u32 CON1;
    __RW __u32 CON2;
    __RW __u32 CON3;
    __RW __u32 IDAT_ADR;
    __RW __u32 IDAT_LEN;
    __RW __u32 ODAT_ADR;
    __RW __u32 ODAT_LEN;
    __RW __u32 FLTB_ADR;
    __WO __u32 ODAT_ADR_START;
    __WO __u32 ODAT_ADR_END;
    __WO __u32 STOP_FLAG;
    __RW __u32 INSR;
    __RW __u32 OUTSR;
    __RW __u32 PHASE;
} JL_SRC_TypeDef;

#define JL_SRC_BASE                     (hsfr_base + map_adr(0x05, 0x00))
#define JL_SRC                          ((JL_SRC_TypeDef		*)JL_SRC_BASE)

//............. 0x0600 - 0x06ff............

typedef struct {
    __RW __u32 CON0;
} JL_HMNG_TypeDef;

#define JL_HMNG_BASE                    (hsfr_base + map_adr(0x06, 0x00))
#define JL_HMNG                         ((JL_HMNG_TypeDef		*)JL_HMNG_BASE)

//#define hs_io_base      0x180000
//
//#define SFC_CON         (*(volatile u32 *)(hs_io_base + 0x0*4))
//#define SFC_BAUD        (*(volatile u32 *)(hs_io_base + 0x1*4))
//
//#define SFC_BASE_ADR    (*(volatile u32 *)(hs_io_base + 0x3*4))
//#define SFC_CODE        (*(volatile u32 *)(hs_io_base + 0x4*4))
//#define SFC_ECON        (*(volatile u32 *)(hs_io_base + 0x5*4))
//
//#define ENC_CON         (*(volatile u32 *)(ls_io_base + 0x10*4))
//#define ENC_KEY         (*(volatile u32 *)(ls_io_base + 0x11*4))
//#define ENC_ADR         (*(volatile u32 *)(ls_io_base + 0x12*4))
//#define ENC_UNENC_ADRL  (*(volatile u32 *)(ls_io_base + 0x13*4))
//#define ENC_UNENC_ADRH  (*(volatile u32 *)(ls_io_base + 0x14*4))




static inline int get_random()
{
    /* return 0x5678; */
    return *(int *)0x2000;//JL_RAND->R64L;
}


#endif	/*	_AD100_H	*/


