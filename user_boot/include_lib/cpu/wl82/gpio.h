/**
 * @file gpio.h
 * @brief
 * @author @zh-jieli.com
 * @version 1.0.0
 * @date 2018-10-11
 */

#ifndef  __GPIO_H__
#define  __GPIO_H__

#include "common.h"

#define     sfc_boot_port()  (JL_IOMC->CON0 & BIT(16))

#define IO_GROUP_NUM 		16

#define IO_PORTA_BASE 		(ls_io_base + 0x00*4)

#define IO_PORTA_00 				(IO_GROUP_NUM * 0 + 0)
#define IO_PORTA_01 				(IO_GROUP_NUM * 0 + 1)
#define IO_PORTA_02 				(IO_GROUP_NUM * 0 + 2)
#define IO_PORTA_03 				(IO_GROUP_NUM * 0 + 3)
#define IO_PORTA_04 				(IO_GROUP_NUM * 0 + 4)
#define IO_PORTA_05 				(IO_GROUP_NUM * 0 + 5)
#define IO_PORTA_06 				(IO_GROUP_NUM * 0 + 6)
#define IO_PORTA_07 				(IO_GROUP_NUM * 0 + 7)
#define IO_PORTA_08 				(IO_GROUP_NUM * 0 + 8)
#define IO_PORTA_09 				(IO_GROUP_NUM * 0 + 9)
#define IO_PORTA_10 				(IO_GROUP_NUM * 0 + 10)
#define IO_PORTA_11 				(IO_GROUP_NUM * 0 + 11)
#define IO_PORTA_12 				(IO_GROUP_NUM * 0 + 12)
#define IO_PORTA_13 				(IO_GROUP_NUM * 0 + 13)
#define IO_PORTA_14 				(IO_GROUP_NUM * 0 + 14)
#define IO_PORTA_15 				(IO_GROUP_NUM * 0 + 15)

#define IO_PORTB_00 				(IO_GROUP_NUM * 1 + 0)
#define IO_PORTB_01 				(IO_GROUP_NUM * 1 + 1)
#define IO_PORTB_02 				(IO_GROUP_NUM * 1 + 2)
#define IO_PORTB_03 				(IO_GROUP_NUM * 1 + 3)
#define IO_PORTB_04 				(IO_GROUP_NUM * 1 + 4)
#define IO_PORTB_05 				(IO_GROUP_NUM * 1 + 5)
#define IO_PORTB_06 				(IO_GROUP_NUM * 1 + 6)
#define IO_PORTB_07 				(IO_GROUP_NUM * 1 + 7)
#define IO_PORTB_08 				(IO_GROUP_NUM * 1 + 8)
#define IO_PORTB_09 				(IO_GROUP_NUM * 1 + 9)
#define IO_PORTB_10 				(IO_GROUP_NUM * 1 + 10)
#define IO_PORTB_11 				(IO_GROUP_NUM * 1 + 11)
#define IO_PORTB_12 				(IO_GROUP_NUM * 1 + 12)
#define IO_PORTB_13 				(IO_GROUP_NUM * 1 + 13)
#define IO_PORTB_14 				(IO_GROUP_NUM * 1 + 14)
#define IO_PORTB_15 				(IO_GROUP_NUM * 1 + 15)

#define IO_PORTC_00 				(IO_GROUP_NUM * 2 + 0)
#define IO_PORTC_01 				(IO_GROUP_NUM * 2 + 1)
#define IO_PORTC_02 				(IO_GROUP_NUM * 2 + 2)
#define IO_PORTC_03 				(IO_GROUP_NUM * 2 + 3)
#define IO_PORTC_04 				(IO_GROUP_NUM * 2 + 4)
#define IO_PORTC_05 				(IO_GROUP_NUM * 2 + 5)
#define IO_PORTC_06 				(IO_GROUP_NUM * 2 + 6)
#define IO_PORTC_07 				(IO_GROUP_NUM * 2 + 7)
#define IO_PORTC_08 				(IO_GROUP_NUM * 2 + 8)
#define IO_PORTC_09 				(IO_GROUP_NUM * 2 + 9)
#define IO_PORTC_10 				(IO_GROUP_NUM * 2 + 10)
#define IO_PORTC_11 				(IO_GROUP_NUM * 2 + 11)
#define IO_PORTC_12 				(IO_GROUP_NUM * 2 + 12)
#define IO_PORTC_13 				(IO_GROUP_NUM * 2 + 13)
#define IO_PORTC_14 				(IO_GROUP_NUM * 2 + 14)
#define IO_PORTC_15 				(IO_GROUP_NUM * 2 + 15)

#define IO_PORTD_00 				(IO_GROUP_NUM * 3 + 0)
#define IO_PORTD_01 				(IO_GROUP_NUM * 3 + 1)
#define IO_PORTD_02 				(IO_GROUP_NUM * 3 + 2)
#define IO_PORTD_03 				(IO_GROUP_NUM * 3 + 3)
#define IO_PORTD_04 				(IO_GROUP_NUM * 3 + 4)
#define IO_PORTD_05 				(IO_GROUP_NUM * 3 + 5)
#define IO_PORTD_06 				(IO_GROUP_NUM * 3 + 6)
#define IO_PORTD_07                 (IO_GROUP_NUM * 3 + 7)
#define IO_PORTD_08                 (IO_GROUP_NUM * 3 + 8)
#define IO_PORTD_09                 (IO_GROUP_NUM * 3 + 9)
#define IO_PORTD_10                 (IO_GROUP_NUM * 3 + 10)
#define IO_PORTD_11                 (IO_GROUP_NUM * 3 + 11)
#define IO_PORTD_12                 (IO_GROUP_NUM * 3 + 12)
#define IO_PORTD_13                 (IO_GROUP_NUM * 3 + 13)
#define IO_PORTD_14                 (IO_GROUP_NUM * 3 + 14)
#define IO_PORTD_15                 (IO_GROUP_NUM * 3 + 15)



#define IO_PORTE_00                 (IO_GROUP_NUM * 4 + 0)
#define IO_PORTE_01                 (IO_GROUP_NUM * 4 + 1)
#define IO_PORTE_02                 (IO_GROUP_NUM * 4 + 2)
#define IO_PORTE_03                 (IO_GROUP_NUM * 4 + 3)
#define IO_PORTE_04                 (IO_GROUP_NUM * 4 + 4)
#define IO_PORTE_05                 (IO_GROUP_NUM * 4 + 5)
#define IO_PORTE_06                 (IO_GROUP_NUM * 4 + 6)
#define IO_PORTE_07                 (IO_GROUP_NUM * 4 + 7)
#define IO_PORTE_08                 (IO_GROUP_NUM * 4 + 8)
#define IO_PORTE_09                 (IO_GROUP_NUM * 4 + 9)
#define IO_PORTE_10                 (IO_GROUP_NUM * 4 + 10)
#define IO_PORTE_11                 (IO_GROUP_NUM * 4 + 11)
#define IO_PORTE_12                 (IO_GROUP_NUM * 4 + 12)
#define IO_PORTE_13                 (IO_GROUP_NUM * 4 + 13)
#define IO_PORTE_14                 (IO_GROUP_NUM * 4 + 14)
#define IO_PORTE_15                 (IO_GROUP_NUM * 4 + 15)

#define IO_PORTF_00                 (IO_GROUP_NUM * 5 + 0)
#define IO_PORTF_01                 (IO_GROUP_NUM * 5 + 1)
#define IO_PORTF_02                 (IO_GROUP_NUM * 5 + 2)
#define IO_PORTF_03                 (IO_GROUP_NUM * 5 + 3)
#define IO_PORTF_04                 (IO_GROUP_NUM * 5 + 4)
#define IO_PORTF_05                 (IO_GROUP_NUM * 5 + 5)
#define IO_PORTF_06                 (IO_GROUP_NUM * 5 + 6)
#define IO_PORTF_07                 (IO_GROUP_NUM * 5 + 7)
#define IO_PORTF_08                 (IO_GROUP_NUM * 5 + 8)
#define IO_PORTF_09                 (IO_GROUP_NUM * 5 + 9)
#define IO_PORTF_10                 (IO_GROUP_NUM * 5 + 10)
#define IO_PORTF_11                 (IO_GROUP_NUM * 5 + 11)
#define IO_PORTF_12                 (IO_GROUP_NUM * 5 + 12)
#define IO_PORTF_13                 (IO_GROUP_NUM * 5 + 13)
#define IO_PORTF_14                 (IO_GROUP_NUM * 5 + 14)
#define IO_PORTF_15                 (IO_GROUP_NUM * 5 + 15)

#define IO_PORTG_00                 (IO_GROUP_NUM * 6 + 0)
#define IO_PORTG_01                 (IO_GROUP_NUM * 6 + 1)
#define IO_PORTG_02                 (IO_GROUP_NUM * 6 + 2)
#define IO_PORTG_03                 (IO_GROUP_NUM * 6 + 3)
#define IO_PORTG_04                 (IO_GROUP_NUM * 6 + 4)
#define IO_PORTG_05                 (IO_GROUP_NUM * 6 + 5)
#define IO_PORTG_06                 (IO_GROUP_NUM * 6 + 6)
#define IO_PORTG_07                 (IO_GROUP_NUM * 6 + 7)
#define IO_PORTG_08                 (IO_GROUP_NUM * 6 + 8)
#define IO_PORTG_09                 (IO_GROUP_NUM * 6 + 9)
#define IO_PORTG_10                 (IO_GROUP_NUM * 6 + 10)
#define IO_PORTG_11                 (IO_GROUP_NUM * 6 + 11)
#define IO_PORTG_12                 (IO_GROUP_NUM * 6 + 12)
#define IO_PORTG_13                 (IO_GROUP_NUM * 6 + 13)
#define IO_PORTG_14                 (IO_GROUP_NUM * 6 + 14)
#define IO_PORTG_15                 (IO_GROUP_NUM * 6 + 15)

#define IO_PORTH_00                 (IO_GROUP_NUM * 7 + 0)
#define IO_PORTH_01                 (IO_GROUP_NUM * 7 + 1)
#define IO_PORTH_02                 (IO_GROUP_NUM * 7 + 2)
#define IO_PORTH_03                 (IO_GROUP_NUM * 7 + 3)
#define IO_PORTH_04                 (IO_GROUP_NUM * 7 + 4)
#define IO_PORTH_05                 (IO_GROUP_NUM * 7 + 5)
#define IO_PORTH_06                 (IO_GROUP_NUM * 7 + 6)
#define IO_PORTH_07                 (IO_GROUP_NUM * 7 + 7)
#define IO_PORTH_08                 (IO_GROUP_NUM * 7 + 8)
#define IO_PORTH_09                 (IO_GROUP_NUM * 7 + 9)
#define IO_PORTH_10                 (IO_GROUP_NUM * 7 + 10)
#define IO_PORTH_11                 (IO_GROUP_NUM * 7 + 11)
#define IO_PORTH_12                 (IO_GROUP_NUM * 7 + 12)
#define IO_PORTH_13                 (IO_GROUP_NUM * 7 + 13)
#define IO_PORTH_14                 (IO_GROUP_NUM * 7 + 14)
#define IO_PORTH_15 				(IO_GROUP_NUM * 7 + 15)

#define IO_PORTI_00 				(IO_GROUP_NUM * 8 + 0)
#define IO_PORTI_01 				(IO_GROUP_NUM * 8 + 1)
#define IO_PORTI_02 				(IO_GROUP_NUM * 8 + 2)
#define IO_PORTI_03 				(IO_GROUP_NUM * 8 + 3)
#define IO_PORTI_04 				(IO_GROUP_NUM * 8 + 4)
#define IO_PORTI_05 				(IO_GROUP_NUM * 8 + 5)
#define IO_PORTI_06 				(IO_GROUP_NUM * 8 + 6)
#define IO_PORTI_07 				(IO_GROUP_NUM * 8 + 7)
#define IO_PORTI_08 				(IO_GROUP_NUM * 8 + 8)
#define IO_PORTI_09 				(IO_GROUP_NUM * 8 + 9)
#define IO_PORTI_10 				(IO_GROUP_NUM * 8 + 10)
#define IO_PORTI_11 				(IO_GROUP_NUM * 8 + 11)
#define IO_PORTI_12 				(IO_GROUP_NUM * 8 + 12)
#define IO_PORTI_13 				(IO_GROUP_NUM * 8 + 13)
#define IO_PORTI_14 				(IO_GROUP_NUM * 8 + 14)
#define IO_PORTI_15 				(IO_GROUP_NUM * 8 + 15)

#define IO_MAX_NUM 					(IO_PORTI_15+1)

#define IO_PORT_PR_00               (IO_MAX_NUM + 0)
#define IO_PORT_PR_01               (IO_MAX_NUM + 1)
#define IO_PORT_PR_02               (IO_MAX_NUM + 2)
#define IO_PORT_PR_03               (IO_MAX_NUM + 3)

#define IO_PR_MAX                   (IO_PORT_PR_03 + 1)

#define IO_PORT_USB_DPA              (IO_PR_MAX + 0)
#define IO_PORT_USB_DMA              (IO_PR_MAX + 1)
#define IO_PORT_USB_DPB              (IO_PR_MAX + 2)
#define IO_PORT_USB_DMB              (IO_PR_MAX + 3)

#define IO_PORTA					IO_PORTA_00
#define IO_PORTB					IO_PORTB_00
#define IO_PORTC					IO_PORTC_00
#define IO_PORTD					IO_PORTD_00
#define IO_PORTE					IO_PORTE_00
#define IO_PORTF					IO_PORTF_00
#define IO_PORTG					IO_PORTG_00
#define IO_PORTH					IO_PORTH_00

#define USB_IO_OFFSET               4
#define IO_PORT_DP                  (IO_MAX_NUM + USB_IO_OFFSET)
#define IO_PORT_DM                  (IO_MAX_NUM + USB_IO_OFFSET + 1)

#define GPIOA                       (IO_GROUP_NUM * 0)
#define GPIOB                       (IO_GROUP_NUM * 1)
#define GPIOC                       (IO_GROUP_NUM * 2)
#define GPIOD                       (IO_GROUP_NUM * 3)
#define GPIOE                       (IO_GROUP_NUM * 4)
#define GPIOF                       (IO_GROUP_NUM * 5)
#define GPIOG                       (IO_GROUP_NUM * 6)
#define GPIOH                       (IO_GROUP_NUM * 7)
#define GPIOR                       (IO_MAX_NUM)
#define GPIOUSB                     (IO_MAX_NUM + USB_IO_OFFSET)

enum {
    INPUT_CH0,
    INPUT_CH1,
    INPUT_CH2,
    INPUT_CH3,
};

enum gpio_op_mode {
    GPIO_SET = 1,
    GPIO_AND,
    GPIO_OR,
    GPIO_XOR,
};
enum gpio_direction {
    GPIO_OUT = 0,
    GPIO_IN = 1,
};
enum gpio_out_channle {
    CH0_UT0_TX,
    CH0_UT1_TX,
    CH0_T0_PWM_OUT,
    CH0_T1_PWM_OUT,
    CH0_RTOSL_CLK,
    CH0_BTOSC_CLK,
    CH0_PLL_12M,
    CH0_UT2_TX,
    CH0_CH0_PWM_H,
    CH0_CH0_PWM_L,
    CH0_CH1_PWM_H,
    CH0_CH1_PWM_L,
    CH0_CH3_PWM_H,
    CH0_CH3_PWM_L,
    CH0_PLNK0_SCLK_OUT,
    CH0_T5_PWM_OUT,

    CH1_UT0_TX = 0x10,
    CH1_UT1_TX,
    CH1_T0_PWM_OUT,
    CH1_T1_PWM_OUT,
    CH1_RTOSL_CLK,
    CH1_BTOSC_CLK,
    CH1_PLL_24M,
    CH1_UT2_TX,
    CH1_CH0_PWM_H,
    CH1_CH0_PWM_L,
    CH1_CH1_PWM_H,
    CH1_CH1_PWM_L,
    CH1_CH3_PWM_H,
    CH1_CH3_PWM_L,
    CH1_PLNK1_SCLK_OUT,
    CH1_T4_PWM_OUT,

    CH2_UT1_RTS = 0x20,
    CH2_UT1_TX,
    CH2_T0_PWM_OUT,
    CH2_T1_PWM_OUT,
    CH2_RTOSL_CLK,
    CH2_BTOSC_CLK,
    CH2_PLL_24M,
    CH2_UT2_TX,
    CH2_CH0_PWM_H,
    CH2_CH0_PWM_L,
    CH2_CH1_PWM_H,
    CH2_CH1_PWM_L,
    CH2_CH2_PWM_H,
    CH2_CH2_PWM_L,
    CH2_T2_PWM_OUT,
    CH2_T3_PWM_OUT,

    CH3_UT1_RTS = 0x30,
    CH3_UT1_TX,
    CH3_T0_PWM_OUT,
    CH3_T1_PWM_OUT,
    CH3_RTOSL_CLK,
    CH3_BTOSC_CLK,
    CH3_PLL_24M,
    CH3_UT2_TX,
    CH3_CH0_PWM_H,
    CH3_CH0_PWM_L,
    CH3_CH1_PWM_H,
    CH3_CH1_PWM_L,
    CH3_CH2_PWM_H,
    CH3_CH2_PWM_L,
    CH3_T2_PWM_OUT,
    CH3_T3_PWM_OUT,
};

struct gpio_reg {
    volatile unsigned int out;
    volatile unsigned int in;
    volatile unsigned int dir;
    volatile unsigned int die;
    volatile unsigned int pu;
    volatile unsigned int pd;
    volatile unsigned int hd0;
    volatile unsigned int hd;
    volatile unsigned int dieh;
};

struct gpio_platform_data {
    unsigned int gpio;
};

#define GPIO_PLATFORM_DATA_BEGIN(data) \
	static const struct gpio_platform_data data = { \


#define GPIO_PLATFORM_DATA_END() \
	};
#if 0
#define     IO_DEBUG_0(i,x)       {JL_PORT##i->DIR &= ~BIT(x), JL_PORT##i->OUT &= ~BIT(x);}
#define     IO_DEBUG_1(i,x)       {JL_PORT##i->DIR &= ~BIT(x), JL_PORT##i->OUT |= BIT(x);}
#define     IO_DEBUG_TOGGLE(i,x)  {JL_PORT##i->DIR &= ~BIT(x), JL_PORT##i->OUT ^= BIT(x);}


#else
#define     IO_DEBUG_0(i,x)         {}
#define     IO_DEBUG_1(i,x)         {}
#define     IO_DEBUG_TOGGLE(i,x)    {}

#endif
/**
 * @brief usb_iomode
 *
 * @param usb_id : 0，USB0；1，USB1
 * 	      enable : 1，使能；0，关闭
 */
void usb_iomode(u8 usb_id, u32 enable);
/**
 * @brief gpio_direction_input
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param dir 1，输入；0，输出
 *
 * @return
 */
int gpio_set_direction(u32 gpio, u32 dir);


/**
 * @brief gpio_direction_input
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 *
 * @return
 */
int gpio_direction_input(u32 gpio);

/**
 * @brief gpio_dir
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 * @param start [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param len  [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param dat 1，输入；0，输出
 *
 * @return
 */
u32 gpio_dir(u32 gpio, u32 start, u32 len, u32 dat, enum gpio_op_mode op);

/**
 * @brief gpio_direction_output
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，输出1；0，输出0
 *
 * @return
 */
int gpio_direction_output(u32 gpio, int value);

/**
 * @brief gpio_out
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 * @param start [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param len  [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param dat 1，输入；0，输出
 *
 * @return
 */
u32 gpio_out(u32 gpio, u32 start, u32 len, u32 dat);

/**
 * @brief gpio_set_pull_up
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，上拉；0，不上拉
 *
 * @return
 */
int gpio_set_pull_up(u32 gpio, int value);


/**
 * @brief gpio_set_pu
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 * @param start [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param len  [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param dat 1，上拉；0，不上拉
 *
 * @return
 */
u32 gpio_set_pu(u32 gpio, u32 start, u32 len, u32 dat, enum gpio_op_mode op);

/**
 * @brief gpio_set_pull_down
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，下拉；0，不下拉
 *
 * @return
 */
int gpio_set_pull_down(u32 gpio, int value);

/**
 * @brief gpio_set_pd
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 * @param start [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param len  [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param dat 1，下拉；0，不下拉
 *
 * @return
 */
u32 gpio_set_pd(u32 gpio, u32 start, u32 len, u32 dat, enum gpio_op_mode op);

/**
 * @brief gpio_set_hd0
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，增强输出；0，不增强输出
 *
 * @return
 */
u32 gpio_set_hd0(u32 gpio, u32 value);

/**
 * @brief gpio_set_hd
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，增强输出；0，不增强输出
 *
 * @return
 */
int gpio_set_hd(u32 gpio, int value);

/**
 * @brief gpio_set_die
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，IO普通输入；0，IO模拟输入
 *
 * @return
 */
int gpio_set_die(u32 gpio, int value);

/**
 * @brief gpio_set_dieh
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，IO普通输入；0，IO模拟输入
 *
 * @return
 */
int gpio_set_dieh(u32 gpio, int value);

/**
 * @brief gpio_die
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 * @param start [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param len  [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param dat 1，IO普通输入；0，IO模拟输入
 *
 * @return
 */
u32 gpio_die(u32 gpio, u32 start, u32 len, u32 dat, enum gpio_op_mode op);

/**
 * @brief gpio_dieh
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 * @param start [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param len  [0-15]，GPIOR[0-3]，GPIOUSB[0-1]
 * @param dat 1，IO普通输入；0，IO模拟输入
 *
 * @return
 */
u32 gpio_dieh(u32 gpio, u32 start, u32 len, u32 dat, enum gpio_op_mode op);

/**
 * @brief gpio_set_output_channle
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param clk 参考枚举CHx_UTx_TX，如CH0_UT0_TX
 *
 * @return
 */
u32 gpio_output_channle(u32 gpio, enum gpio_out_channle clk);

/**
 * @brief gpio_clear_output_channle
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param clk 参考枚举CHx_UTx_TX，如CH0_UT0_TX
 *
 * @return
 */
u32 gpio_clear_output_channle(u32 gpio, enum gpio_out_channle clk);

/**
 * @brief gpio_read
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 *
 * @return
 */
int gpio_read(u32 gpio);

/**
 * @brief gpio_in
 *
 * @param gpio [GPIOA GPIOB GPIOC GPIOD GPIOR GPIOUSB]
 *
 * @return
 */
u32 gpio_in(u32 gpio);
/**
 * @brief gpio_write
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 * @param value 1，输出1；0，输出0
 *
 * @return
 */
u32 gpio_write(u32 gpio, u32 value);

/**
 * @brief gpio_wakeup0 use IN_CHNL0_SEL
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 *
 * @return
 */
u32 gpio_wakeup0(u32 gpio);

/**
 * @brief gpio_irflt_in use IN_CHNL1_SEL
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 *
 * @return
 */
u32 gpio_irflt_in(u32 gpio);

/**
 * @brief gpio_cap_mux use IN_CHNL2_SEL
 *
 * @param gpio 参考宏IO_PORTx_xx，如IO_PORTA_00
 *
 * @return
 */
u32 gpio_cap_mux(u32 gpio);


/**
 * @brief gpio_uart_rx_input
 *
 * @param gpio
 * @param ut
 * @param ch
 *
 * @return
 */
u32 gpio_uart_rx_input(u32 gpio, u32 ut, u32 ch);

/**
 * @brief gpio_uart_rx_input
 *
 * @param gpio
 * @param ut
 * @param ch
 *
 * @return
 */
u32 gpio_uart_clear_rx_input(u32 gpio, u32 ut, u32 ch);

/**
 * @brief
 *
 * @return
 */
u32 gpio_close_uart0(void);

/**
 * @brief
 *
 * @return
 */
u32 gpio_close_uart1(void);

/**
 * @brief
 *
 * @return
 */
u32 gpio_close_uart2(void);

/**
 * @brief gpio_set_uart0
 *
 * @param ch 0:3 选择对应IO br22
 *         |ch|tx|rx|
 *         |- |- |- |
 *         |0|PA5_TX|PA6_RX|
 *         |1|PB7_TX|PB8_RX|
 *         |2|PA7_TX|PA8_RX|
 *         |3|预留|预留|
 *         |-1|关闭对应的IO口串口功能|no|
 *
 * @return
 */
u32 gpio_set_uart0(u32 ch);
/**
 * @brief gpio_set_uart1
 *
 * @param ch 0:3 选择对应IO  br22
 *         |ch|tx|rx|
 *         |- |- |- |
 *         |0|PB5_TX|PB6_RX|
 *         |1|预留|预留|
 *         |2|PA1_TX|PA2_RX|
 *         |3|USBDP |USBDM |
 *         |-1|关闭对应的IO口串口功能|no|
 *
 * @return
 */
u32 gpio_set_uart1(u32 ch);
/**
 * @brief gpio_set_uart2
 *
 * @param ch 0:3 选择对应IO  br22
 *         |ch|tx|rx|
 *         |- |- |- |
 *         |0|PA3_TX|PA4_RX|
 *         |1|预留|预留|
 *         |2|预留|预留|
 *         |3|PA9_TX|PA10_RX|
 *         |-1|关闭对应的IO口串口功能|no|
 *
 * @return
 */
u32 gpio_set_uart2(u32 ch);

enum {
    IRFLT_LSB,
    IRFLT_RC,
    IRFLT_OSC,
    IRFLT_PLL48M,
};
enum {
    IRFLT_DIV1,
    IRFLT_DIV2,
    IRFLT_DIV4,
    IRFLT_DIV8,
    IRFLT_DIV16,
    IRFLT_DIV32,
    IRFLT_DIV64,
    IRFLT_DIV128,
    IRFLT_DIV256,
    IRFLT_DIV512,
    IRFLT_DIV1024,
    IRFLT_DIV2048,
    IRFLT_DIV4096,
    IRFLT_DIV8192,
    IRFLT_DIV16384,
    IRFLT_DIV32768,
};
/* u32 irflt_config(u32 osc, u32 div); */

/**
 * @brief gpio_irflt_to_timer
 *
 * @param t: [0-3]
 *
 * @return
 */
u32 gpio_irflt_to_timer(u32 t);


u32 get_gpio(const char *p);

enum OUTPUT_CH_SIGNAL {
    OUTPUT_CH_SIGNAL_MC_PWM0_H = 0,
    OUTPUT_CH_SIGNAL_MC_PWM0_L,
    OUTPUT_CH_SIGNAL_MC_PWM1_H,
    OUTPUT_CH_SIGNAL_MC_PWM1_L,
    OUTPUT_CH_SIGNAL_MC_PWM2_H,
    OUTPUT_CH_SIGNAL_MC_PWM2_L,
    OUTPUT_CH_SIGNAL_MC_PWM3_H,
    OUTPUT_CH_SIGNAL_MC_PWM3_L,
    OUTPUT_CH_SIGNAL_TIMER0_PWM,
    OUTPUT_CH_SIGNAL_TIMER1_PWM,
    OUTPUT_CH_SIGNAL_TIMER2_PWM,
    OUTPUT_CH_SIGNAL_TIMER3_PWM,
    OUTPUT_CH_SIGNAL_TIMER4_PWM,
    OUTPUT_CH_SIGNAL_TIMER5_PWM,
    OUTPUT_CH_SIGNAL_CLOCK_OUT0,
    OUTPUT_CH_SIGNAL_CLOCK_OUT1,
    OUTPUT_CH_SIGNAL_CLOCK_OUT2,
    OUTPUT_CH_SIGNAL_CLOCK_OUT3,
};

enum INPUT_CH_TYPE {
    INPUT_CH_TYPE_GP_ICH = 0,
    INPUT_CH_TYPE_TIME0_PWM = 14,
    INPUT_CH_TYPE_TIME1_PWM,
};

enum INPUT_CH_SIGNAL {
    //ICH_CON0
    INPUT_CH_SIGNAL_TIMER0_CIN = 0,
    INPUT_CH_SIGNAL_TIMER0_CAPTURE,
    INPUT_CH_SIGNAL_TIMER1_CIN,
    INPUT_CH_SIGNAL_TIMER1_CAPTURE,
    INPUT_CH_SIGNAL_TIMER2_CIN,
    INPUT_CH_SIGNAL_TIMER2_CAPTURE,
    INPUT_CH_SIGNAL_TIMER3_CIN,
    INPUT_CH_SIGNAL_TIMER3_CAPTURE,

    //ICH_CON1
    INPUT_CH_SIGNAL_TIMER4_CIN,
    INPUT_CH_SIGNAL_TIMER4_CAPTURE,
    INPUT_CH_SIGNAL_TIMER5_CIN,
    INPUT_CH_SIGNAL_TIMER5_CAPTURE,
    INPUT_CH_SIGNAL_WAKEUP,
    INPUT_CH_SIGNAL_IRFLT,
    INPUT_CH_SIGNAL_CAP,
    INPUT_CH_SIGNAL_CLK_PIN,

    //ICH_CON2
    INPUT_CH_SIGNAL_WLC_EXT_ACT,
    INPUT_CH_SIGNAL_LCD_TE,
    INPUT_CH_SIGNAL_MC_CKPIN0,
    INPUT_CH_SIGNAL_MC_CKPIN1,
    INPUT_CH_SIGNAL_MC_CKPIN2,
    INPUT_CH_SIGNAL_MC_CKPIN3,
    INPUT_CH_SIGNAL_MC_FPIN0,
    INPUT_CH_SIGNAL_MC_FPIN1,

    //ICH_CON3
    INPUT_CH_SIGNAL_MC_FPIN2,
    INPUT_CH_SIGNAL_MC_FPIN3,
    INPUT_CH_SIGNAL_RESERVE0,
    INPUT_CH_SIGNAL_RESERVE1,
    INPUT_CH_SIGNAL_RESERVE2,
    INPUT_CH_SIGNAL_RESERVE3,
    INPUT_CH_SIGNAL_RESERVE4,
    INPUT_CH_SIGNAL_RESERVE5,
};


//=================================================================================//
//@brief: CrossBar 获取某IO的输出映射寄存器
//@input:
// 		gpio: 需要输出外设信号的IO口; 如IO_PORTA_00
//@return:
// 		输出映射寄存器地址; 如&(JL_OMAP->PA0_OUT)
//=================================================================================//
u32 *gpio2crossbar_outreg(u32 gpio);

//=================================================================================//
//@brief: CrossBar 获取某IO的输入映射序号
//@input:
// 		gpio: 需要输出外设信号的IO口; 如IO_PORTA_00
//@return:
// 		输出映射序号; 如PA0_IN
//=================================================================================//
u32 gpio2crossbar_inport(u32 gpio);

//=================================================================================//
//@brief: CrossBar 输出设置 API, 将指定IO口设置为某个外设的输出
//@input:
// 		gpio: 需要输出外设信号的IO口;
// 		fun_index: 需要输出到指定IO口的外设信号, 可以输出外设信号列表请查看io_omap.h文件;
// 		dir_ctl: IO口方向由外设控制使能, 常设为1;
// 		data_ctl: IO口电平状态由外设控制使能, 常设为1;
//@return:
// 		1)0: 执行正确;
//		2)-EINVAL: 传参出错;
//@note: 所映射的IO需要在设置IO状态为输出配置;
//@example: 将UART0的Tx信号输出到IO_PORTA_05口:
// 			gpio_direction_output(IO_PORTA_05, 1); //设置IO为输出状态
//			gpio_set_fun_output_port(IO_PORTA_05, FO_UART0_TX, 1, 1); //将UART0的Tx信号输出到IO_PORTA_05口
//=================================================================================//
int gpio_set_fun_output_port(u32 gpio, u32 fun_index, u8 dir_ctl, u8 data_ctl);

//=================================================================================//
//@brief: CrossBar 输出设置 API, 将指定IO释放外设控制, 变为普通IO;
//@input:
// 		gpio: 需要释放外设控制IO口, 释放后变为普通IO模式;
//@return:
// 		1)0: 执行正确;
//		2)-EINVAL: 传参出错;
//@note:
//@example: 将IO_PORTA_05口被某一外设控制状态释放:
// 			gpio_disable_fun_output_port(IO_PORTA_05);
//=================================================================================//
int gpio_disable_fun_output_port(u32 gpio);

//=================================================================================//
//@brief: Output Channel输出设置 API, 将指定IO口设置为某个外设的输出
//@input:
// 		gpio: 需要输出外设信号的IO口;
// 		signal: 将enum OUTPUT_CH_SIGNAL列表中需要输出到指定IO口的外设信号, 可以输出的外设信号列表请查看gpio.h文件的enum OUTPUT_CH_SIGNAL枚举项;
//@return:  默认为0, 出错内部触发ASSERT;
//@note: 所映射的IO需要在设置IO状态为输出配置;
//@example: 将OUTPUT_CH_SIGNAL_MC_PWM0_H的Tx信号输出到IO_PORTA_05口:
// 			gpio_direction_output(IO_PORTA_05, 1); //设置IO为输出状态
//			gpio_och_sel_output_signal(IO_PORTA_05, OUTPUT_CH_SIGNAL_MC_PWM0_H); //将OUTPUT_CH_SIGNAL_MC_PWM0_H信号输出到IO_PORTA_05口
//=================================================================================//
int gpio_och_sel_output_signal(u32 gpio, enum OUTPUT_CH_SIGNAL signal);

//=================================================================================//
//@brief: Output Channel 输出设置 API, 将指定IO释放外设控制, 变为普通IO;
//@input:
// 		gpio: 需要释放外设控制IO口, 释放后变为普通IO模式;
// 		signal: 将enum OUTPUT_CH_SIGNAL列表中需要取消输出的外设信号, 外设信号列表请查看gpio.h文件的enum OUTPUT_CH_SIGNAL枚举项;;
//@return:  默认为0, 无出错处理;
//@note:
//@example: 将OUTPUT_CH_SIGNAL_MC_PWM0_H取消输出IO_PORTA_05:
// 			gpio_och_disable_output_signal(IO_PORTA_05, OUTPUT_CH_SIGNAL_MC_PWM0_H);
//=================================================================================//
int gpio_och_disable_output_signal(u32 gpio, enum OUTPUT_CH_SIGNAL signal);

//=================================================================================//
//@brief: Input Channel 输入设置 API, 将某个外设的输入设置为从某个IO输入
//@input:
// 		gpio: 需要输入外设信号的IO口;
// 		signal: 需要从指定IO输入的外设信号, 可以输入的外设信号列表请查看gpio.h文件enum INPUT_CH_SIGNAL枚举项;
//      type: INPUT_CH 类型, 常设为INPUT_CH_TYPE_GP_ICH;
//@return:  默认为0, 出错内部触发ASSERT;
//@note: 所映射的IO需要在设置IO状态为输入配置;
//@example: 将INPUT_CH_SIGNAL_TIMER0_CIN信号设置为IO_PORTA_05口输入:
//			gpio_set_die(IO_PORTA_05, 1); 		//数字输入使能
//			gpio_set_pull_up(IO_PORTA_05, 1);  //上拉输入使能
//			gpio_direction_input(IO_PORTA_05);  //设置IO为输入状态
//			gpio_ich_sel_iutput_signal(IO_PORTA_05, INPUT_CH_SIGNAL_TIMER0_CIN, INPUT_CH_TYPE_GP_ICH); //将INPUT_CH_SIGNAL_TIMER0_CIN信号设置为IO_PORTA_05口输入
//=================================================================================//
int gpio_ich_sel_iutput_signal(u32 gpio, enum INPUT_CH_SIGNAL signal, enum INPUT_CH_TYPE type);

//=================================================================================//
//@brief: Input Channel 输入设置 API, 将某个外设信号释放IO口控制, 变为普通IO;
//@input:
// 		gpio: 需要取消输入外设信号的IO口;
// 		signal: 需要取消输入的外设信号, 外设信号列表请查看gpio.h文件enum INPUT_CH_SIGNAL枚举项;
//      type: INPUT_CH 类型, 常设为INPUT_CH_TYPE_GP_ICH;
//@return:  默认为0, 无出错处理;
//@note:
//@example: 将外设信号INPUT_CH_SIGNAL_TIMER0_CIN释放由某个IO输入:
// 			gpio_ich_disable_iutput_signal(IO_PORTA_05, INPUT_CH_SIGNAL_TIMER0_CIN, INPUT_CH_TYPE_GP_ICH);
//=================================================================================//
int gpio_ich_disable_iutput_signal(u32 gpio, enum INPUT_CH_SIGNAL signal, enum INPUT_CH_TYPE type);


//=================================================================================//
//@brief: gpio_set_outputchannel 将某个外设信号释放IO口控制
//@input:
// 		gpio: 需要取消输入外设信号的IO口;
// 		ch: 0 - 3
//@return:  0成功，非0出错;
//@note:
int gpio_set_outputchannel(u32 gpio, u8 ch);


//=================================================================================//
//@brief: gpio_latch_en_by_name 锁存输出某个IO信号
//@input:
// 		port: 需要锁存某个IO：如"PC01"
// 		en: 1锁存，0不锁存
//@return: 无
//@note:
void gpio_latch_en_by_name(char *port, u8 en);//"PC01"

#endif  /*GPIO_H*/

