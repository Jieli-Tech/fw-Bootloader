#ifndef  __GPIO_H__
#define  __GPIO_H__

#include "common.h"


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

#define IO_PORTB_00                 (IO_GROUP_NUM * 1 + 0)
#define IO_PORTB_01                 (IO_GROUP_NUM * 1 + 1)
#define IO_PORTB_02                 (IO_GROUP_NUM * 1 + 2)
#define IO_PORTB_03                 (IO_GROUP_NUM * 1 + 3)
#define IO_PORTB_04                 (IO_GROUP_NUM * 1 + 4)
#define IO_PORTB_05                 (IO_GROUP_NUM * 1 + 5)
#define IO_PORTB_06                 (IO_GROUP_NUM * 1 + 6)
#define IO_PORTB_07                 (IO_GROUP_NUM * 1 + 7)
#define IO_PORTB_08                 (IO_GROUP_NUM * 1 + 8)

#define IO_PORTC_00                 (IO_GROUP_NUM * 2 + 0)
#define IO_PORTC_01                 (IO_GROUP_NUM * 2 + 1)
#define IO_PORTC_02                 (IO_GROUP_NUM * 2 + 2)
#define IO_PORTC_03                 (IO_GROUP_NUM * 2 + 3)
#define IO_PORTC_04                 (IO_GROUP_NUM * 2 + 4)
#define IO_PORTC_05                 (IO_GROUP_NUM * 2 + 5)

#define IO_PORTD_00                 (IO_GROUP_NUM * 3 + 0)
#define IO_PORTD_01                 (IO_GROUP_NUM * 3 + 1)
#define IO_PORTD_02                 (IO_GROUP_NUM * 3 + 2)
#define IO_PORTD_03                 (IO_GROUP_NUM * 3 + 3)
#define IO_PORTD_04                 (IO_GROUP_NUM * 3 + 4)
#define IO_PORTD_05                 (IO_GROUP_NUM * 3 + 5)
#define IO_PORTD_06                 (IO_GROUP_NUM * 3 + 6)
#define IO_PORTD_07                 (IO_GROUP_NUM * 3 + 7)

#define IO_PORTP_00                 (IO_GROUP_NUM * 4 + 0)

#define IO_MAX_NUM                  (IO_PORTP_00 + 1)

#define USB_IO_OFFSET               0
#define IO_PORT_DP                  (IO_MAX_NUM + USB_IO_OFFSET)
#define IO_PORT_DM                  (IO_MAX_NUM + USB_IO_OFFSET + 1)

#define P33_IO_OFFSET               2
#define IO_CHGFL_DET                (IO_MAX_NUM + P33_IO_OFFSET + 0)
#define IO_VBGOK_DET                (IO_MAX_NUM + P33_IO_OFFSET + 1)
#define IO_VBTCH_DET                (IO_MAX_NUM + P33_IO_OFFSET + 2)
#define IO_LDOIN_DET                (IO_MAX_NUM + P33_IO_OFFSET + 3)

#define IO_PORT_MAX                 (IO_LDOIN_DET + 1)


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
u32 get_gpio(const char *p);

u32 *gpio2crossbar_outreg(u32 gpio);
u32 gpio2crossbar_inport(u32 gpio);

#endif
