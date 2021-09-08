#ifndef  __GPIO_H__
#define  __GPIO_H__

#include "common.h"

enum {
    CH0_UT0_TX,
    CH0_UT1_TX,
    CH0_T0_PWM_OUT,
    CH0_T1_PWM_OUT,
    CH0_RTOSH_CLK,
    CH0_BTOSC_CLK,
    CH0_PLL_12M,
    CH0_UT2_TX,
    CH0_T4_PWM_OUT = 0xc,
    CH0_T5_PWM_OUT,
    CH0_WLC_INT_FREQ,
    CH0_T3_PWM_OUT,

    CH1_UT0_TX = 0x10,
    CH1_UT1_TX,
    CH1_T0_PWM_OUT,
    CH1_WLC_INT_STATUS,
    CH1_RTOSL_CLK,
    CH1_BTOSC_CLK,
    CH1_SPDIF_DO,
    CH1_UT2_TX,
    CH1_T4_PWM_OUT = 0x1c,
    CH1_T5_PWM_OUT,
    CH1_T2_PWM_OUT,
    CH1_T3_PWM_OUT,

    CH2_UT1_RTS = 0x20,
    CH2_UT1_TX,
    CH2_WLC_INT_ACTIVE,
    CH2_T1_PWM_OUT,
    CH2_PLNK_SCLK,
    CH2_BTOSC_CLK,
    CH2_PLL_24M,
    CH2_UT2_TX,
    CH2_T4_PWM_OUT = 0x2c,
    CH2_T5_PWM_OUT,
    CH2_T2_PWM_OUT,
    CH2_T3_PWM_OUT,
};



#define IO_GROUP_NUM        16

#define IO_PORTA_00                 (IO_GROUP_NUM * 0 + 0)
#define IO_PORTA_01                 (IO_GROUP_NUM * 0 + 1)
#define IO_PORTA_02                 (IO_GROUP_NUM * 0 + 2)
#define IO_PORTA_03                 (IO_GROUP_NUM * 0 + 3)
#define IO_PORTA_04                 (IO_GROUP_NUM * 0 + 4)
#define IO_PORTA_05                 (IO_GROUP_NUM * 0 + 5)
#define IO_PORTA_06                 (IO_GROUP_NUM * 0 + 6)
#define IO_PORTA_07                 (IO_GROUP_NUM * 0 + 7)
#define IO_PORTA_08                 (IO_GROUP_NUM * 0 + 8)
#define IO_PORTA_09                 (IO_GROUP_NUM * 0 + 9)
#define IO_PORTA_10                 (IO_GROUP_NUM * 0 + 10)
#define IO_PORTA_11                 (IO_GROUP_NUM * 0 + 11)
#define IO_PORTA_12                 (IO_GROUP_NUM * 0 + 12)
#define IO_PORTA_13                 (IO_GROUP_NUM * 0 + 13)
#define IO_PORTA_14                 (IO_GROUP_NUM * 0 + 14)
#define IO_PORTA_15                 (IO_GROUP_NUM * 0 + 15)

#define IO_PORTB_00                 (IO_GROUP_NUM * 1 + 0)
#define IO_PORTB_01                 (IO_GROUP_NUM * 1 + 1)
#define IO_PORTB_02                 (IO_GROUP_NUM * 1 + 2)
#define IO_PORTB_03                 (IO_GROUP_NUM * 1 + 3)
#define IO_PORTB_04                 (IO_GROUP_NUM * 1 + 4)
#define IO_PORTB_05                 (IO_GROUP_NUM * 1 + 5)
#define IO_PORTB_06                 (IO_GROUP_NUM * 1 + 6)
#define IO_PORTB_07                 (IO_GROUP_NUM * 1 + 7)
#define IO_PORTB_08                 (IO_GROUP_NUM * 1 + 8)
#define IO_PORTB_09                 (IO_GROUP_NUM * 1 + 9)
#define IO_PORTB_10                 (IO_GROUP_NUM * 1 + 10)
#define IO_PORTB_11                 (IO_GROUP_NUM * 1 + 11)
#define IO_PORTB_12                 (IO_GROUP_NUM * 1 + 12)
#define IO_PORTB_13                 (IO_GROUP_NUM * 1 + 13)
#define IO_PORTB_14                 (IO_GROUP_NUM * 1 + 14)
#define IO_PORTB_15                 (IO_GROUP_NUM * 1 + 15)

#define IO_PORTC_00                 (IO_GROUP_NUM * 2 + 0)
#define IO_PORTC_01                 (IO_GROUP_NUM * 2 + 1)
#define IO_PORTC_02                 (IO_GROUP_NUM * 2 + 2)
#define IO_PORTC_03                 (IO_GROUP_NUM * 2 + 3)
#define IO_PORTC_04                 (IO_GROUP_NUM * 2 + 4)
#define IO_PORTC_05                 (IO_GROUP_NUM * 2 + 5)
#define IO_PORTC_06                 (IO_GROUP_NUM * 2 + 6)
#define IO_PORTC_07                 (IO_GROUP_NUM * 2 + 7)
#define IO_PORTC_08                 (IO_GROUP_NUM * 2 + 8)
#define IO_PORTC_09                 (IO_GROUP_NUM * 2 + 9)
#define IO_PORTC_10                 (IO_GROUP_NUM * 2 + 10)
#define IO_PORTC_11                 (IO_GROUP_NUM * 2 + 11)
#define IO_PORTC_12                 (IO_GROUP_NUM * 2 + 12)
#define IO_PORTC_13                 (IO_GROUP_NUM * 2 + 13)
#define IO_PORTC_14                 (IO_GROUP_NUM * 2 + 14)
#define IO_PORTC_15                 (IO_GROUP_NUM * 2 + 15)

#define IO_PORTD_00                 (IO_GROUP_NUM * 3 + 0)
#define IO_PORTD_01                 (IO_GROUP_NUM * 3 + 1)
#define IO_PORTD_02                 (IO_GROUP_NUM * 3 + 2)
#define IO_PORTD_03                 (IO_GROUP_NUM * 3 + 3)
#define IO_PORTD_04                 (IO_GROUP_NUM * 3 + 4)
#define IO_PORTD_05                 (IO_GROUP_NUM * 3 + 5)
#define IO_PORTD_06                 (IO_GROUP_NUM * 3 + 6)
#define IO_PORTD_07                 (IO_GROUP_NUM * 3 + 7)


#define IO_MAX_NUM                  (IO_PORTD_07+1)

#define IO_PORT_DP                  61
#define IO_PORT_DM                  62

#define IO_PORT_MAX                 (IO_PORT_DM + 1)

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

#define __gpio_mask(gpio) (BIT(gpio % IO_GROUP_NUM))

struct gpio_reg *gpio2reg(u32 gpio);

u32 usb_read(u32 gpio);
void usb_set_direction(u32 gpio, u32 value);
void usb_output(u32 gpio, u32 value);
void usb_set_pull_up(u32 gpio, u32 value);
void usb_set_pull_down(u32 gpio, u32 value);
void usb_set_die(u32 gpio, u32 value);
void usb_set_dieh(u32 gpio, u32 value);
void usb_iomode(u32 enable);

u32 gpio_read(u32 gpio);
void gpio_write(u32 gpio, u32 value);
void gpio_set_direction(u32 gpio, u32 dir);
void gpio_set_pull_up(u32 gpio, u32 value);
void gpio_set_pull_down(u32 gpio, u32 value);
void gpio_set_hd0(u32 gpio, u32 value);
void gpio_set_hd(u32 gpio, u32 value);
void gpio_set_die(u32 gpio, u32 value);
void gpio_set_dieh(u32 gpio, u32 value);
void gpio_output_channle(u32 gpio, u32 chx_xx);
void gpio_uart_input_channel(u32 gpio, u32 ut_num);
u32 get_gpio(const char *p);

#endif
