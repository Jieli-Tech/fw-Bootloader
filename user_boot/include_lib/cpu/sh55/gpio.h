#ifndef  __GPIO_H__
#define  __GPIO_H__

#include "common.h"

typedef enum {
    CH0_CH0_PWM = 0,
    CH0_CH1_PWM,
    CH0_T2_PWM_OUT0,
    CH0_T2_PWM_OUT1,
    CH0_SPI0_DO,
    CH0_UT1_TX,
    CH0_IN_CH3_SEL,
    CH0_PLL24M,

    CH1_CH2_PWM = 8,
    CH1_CH3_PWM,
    CH1_T2_PWM_OUT0,
    CH1_T2_PWM_OUT1,
    CH1_UT0_TX,
    CH1_UT1_TX,
    CH1_CLKOUT,
    CH1_PLL24M,
    CH2_CH0_PWM = 16,
    CH2_CH1_PWM,
    CH2_T2_PWM_OUT0,
    CH2_T2_PWM_OUT1,
    CH2_SPI0_DO,
    CH2_UT1_TX,
    CH2_IN_CH3_SEL,
    CH2_PLL24M,
    CH3_CH2_PWM = 24,
    CH3_CH3_PWM,
    CH3_T2_PWM_OUT0,
    CH3_T2_PWM_OUT1,
    CH3_UT0_TX,
    CH3_UT1_TX,
    CH3_CLKOUT,
    CH3_PLL24M,
} OUTCH_SEL_T;

typedef enum {
    DACN_OUTCH0 = 0,
    DACN_OUTCH1 = 1,
    DACN_OUTIO = 2,
    DACP_OUTCH0 = 3,
    DACP_OUTCH1 = 4,
    DACP_OUTIO = 5,
} DACX_MODE_SEL;

#define IO_GROUP_NUM 		16

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

#define IO_PORTD_00 				(IO_GROUP_NUM * 2 + 0)
#define IO_PORTD_01 				(IO_GROUP_NUM * 2 + 1)
#define IO_PORTD_02 				(IO_GROUP_NUM * 2 + 2)
#define IO_PORTD_03 				(IO_GROUP_NUM * 2 + 3)
#define IO_PORTD_04 				(IO_GROUP_NUM * 2 + 4)

#define IO_MAX_NUM 					(IO_PORTD_04+1)
#define IO_PORT_MAX					(IO_PORTD_04 + 1)

#define GPIOA                       (IO_GROUP_NUM * 0)
#define GPIOB                       (IO_GROUP_NUM * 1)
#define GPIOD                       (IO_GROUP_NUM * 2)

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

#define DACN_IO_DIR 	BIT(2)
#define DACN_IO_PD		BIT(4)
#define DACN_IO_PU 		BIT(6)
#define DACN_IO_DIE 	BIT(9)

#define DACP_IO_DIR 	BIT(3)
#define DACP_IO_PD		BIT(5)
#define DACP_IO_PU 		BIT(7)
#define DACP_IO_DIE 	BIT(10)







#define __gpio_mask(gpio) (BIT(gpio % IO_GROUP_NUM))

struct gpio_reg *gpio2reg(u32 gpio);

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
