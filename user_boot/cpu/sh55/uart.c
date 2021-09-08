#include "uart.h"
#include "gpio.h"
#include "irq.h"
#include "printf.h"
#include "clock.h"

static u8 used_tx_io;

void putchar(char a)
{
#ifdef APP_DEBUG
    if (JL_UT0->CON & BIT(0)) {
        JL_UT0->BUF = a;
        __asm__ volatile("csync");
        while ((JL_UT0->CON & BIT(15)) == 0);
        JL_UT0->CON |= BIT(13);
    }
#endif
}

void uart_init(const char *tx_io, u32 baud)
{
    //PLL48M
    JL_CLK->CON0 &= ~BIT(8);
    JL_CLK->CON0 &= ~BIT(9);
    JL_IOMC->IOMC0 &= ~(BIT(5) | BIT(6));
    JL_IOMC->IOMC0 &= ~BIT(4);

#ifdef APP_DEBUG
    used_tx_io = get_gpio(tx_io);
    if ((baud) && (used_tx_io < IO_PORT_MAX)) {
        if (used_tx_io < IO_GROUP_NUM) {
            gpio_output_channle(used_tx_io, CH1_UT0_TX);
        } else {
            gpio_output_channle(used_tx_io, CH3_UT0_TX);
        }
        JL_UT0->BAUD = 48000000 / baud / 4 - 1;
        JL_UT0->CON = BIT(13) | BIT(12) | BIT(0);
    }
#endif
}

void uart_close(void)
{
#ifdef APP_DEBUG
    if (JL_UT0->CON & BIT(0)) {
        JL_UT0->CON = 0;
        gpio_set_pull_up(used_tx_io, 0);
        gpio_set_pull_down(used_tx_io, 0);
        gpio_set_die(used_tx_io, 0);
        gpio_set_direction(used_tx_io, 1);
    }
#endif
}



#define UT_UP_BAUD_CLK      48000000
#define UT_UP_BUF_SIZE      4128    //4096 + 32
static u8 ut_up_buf[UT_UP_BUF_SIZE] ALIGNE(32);
static u32 ut_up_rxlen = 0;

extern void set_new_data_flag(int flag);

u8 *get_ut_up_buf_p(void)
{
    return ut_up_buf;
}

u32 get_ut_up_rxlen(void)
{
    return ut_up_rxlen;
}

void ut_up_tx_data(u8 *buf, u32 len)
{
    JL_UT1->TXADR = (u32)buf;
    JL_UT1->TXCNT = len;
    __asm__ volatile("csync");
    while (!(JL_UT1->CON0 & BIT(15)));
    JL_UT1->CON0 |= BIT(13);
}

SET_INTERRUPT
void uart1_irq(void)
{
    if ((JL_UT1->CON0 & BIT(5)) && (JL_UT1->CON0 & BIT(11))) {
        JL_UT1->CON0 |= BIT(7);
        JL_UT1->CON0 |= BIT(10);
        JL_UT1->CON0 |= BIT(12);
        __asm__ volatile("csync");
        __asm__ volatile("csync");
        ut_up_rxlen = JL_UT1->HRXCNT;
        if (ut_up_rxlen >= 4) {
            set_new_data_flag(1);
        }
        JL_UT1->RXSADR = (u32)ut_up_buf;
        JL_UT1->RXEADR = (u32)ut_up_buf + UT_UP_BUF_SIZE;
        JL_UT1->RXCNT = UT_UP_BUF_SIZE;
    }
}

void ut_device_mode(const char *ut_tx, const char *ut_rx, u32 baud)
{
    log_info("\n**** ut up mode *******\n\n");
    u32 ut_tx_io = get_gpio(ut_tx);
    u32 ut_rx_io = get_gpio(ut_rx);
    if ((baud) && (ut_tx_io < IO_PORT_MAX) && (ut_rx_io < IO_PORT_MAX)) {
        if (ut_tx_io < IO_GROUP_NUM) {
            gpio_output_channle(ut_tx_io, CH0_UT1_TX);
        } else {
            gpio_output_channle(ut_tx_io, CH2_UT1_TX);
        }
        gpio_uart_input_channel(ut_rx_io, 1);
        request_irq(IRQ_UART1_IDX, 0, uart1_irq, 0);
        JL_UT1->CON0 = BIT(13) | BIT(12) | BIT(10);
        JL_UT1->BAUD = UT_UP_BAUD_CLK / baud / 4 - 1;
        JL_UT1->RXSADR = (u32)ut_up_buf;
        JL_UT1->RXEADR = (u32)ut_up_buf + UT_UP_BUF_SIZE;
        JL_UT1->RXCNT = UT_UP_BUF_SIZE;
        JL_UT1->OTCNT = UT_UP_BAUD_CLK / 1000; //lsb_clk/1000=1ms
        JL_UT1->CON0 |= BIT(5);   //OT中断
        JL_UT1->CON0 |= BIT(7) | BIT(6); //启动DMA
        JL_UT1->CON0 |= BIT(13) | BIT(12) | BIT(10) | BIT(1) | BIT(0);
    }
}

