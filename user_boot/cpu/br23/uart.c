#include "uart.h"
#include "gpio.h"
#include "irq.h"
#include "printf.h"

#define LOG_TAG_CONST       UPGRADE
#define LOG_TAG             "[uart]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#include "log.h"
static u8 used_tx_io;

void putchar(char a)
{
    if (JL_UART1->CON0 & BIT(0)) {
        JL_UART1->BUF = a;
        __asm__ volatile("csync");
        while ((JL_UART1->CON0 & BIT(15)) == 0);
        JL_UART1->CON0 |= BIT(13);
    }
}

void uart_init(const char *tx_io, u32 baud)
{
    //PLL48M
    JL_CLOCK->CLK_CON2 &= ~BIT(10);
    JL_CLOCK->CLK_CON2 &= ~BIT(11);
    JL_IOMAP->CON3 &= ~(0xfff);
    used_tx_io = get_gpio(tx_io);
    if ((baud) && (used_tx_io < IO_PORT_MAX)) {
        gpio_output_channle(used_tx_io, CH1_UT1_TX);
        JL_UART1->BAUD = 48000000 / baud / 4 - 1;
        JL_UART1->CON0 = BIT(13) | BIT(12) | BIT(10) | BIT(0);
    }
}

void uart_close(void)
{
    if (JL_UART1->CON0 & BIT(0)) {
        JL_UART1->CON0 = 0;
        JL_UART1->CON1 = 0;
        gpio_set_pull_up(used_tx_io, 0);
        gpio_set_pull_down(used_tx_io, 0);
        gpio_set_die(used_tx_io, 0);
        gpio_set_direction(used_tx_io, 1);
    }
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
    JL_UART0->TXADR = (u32)buf;
    JL_UART0->TXCNT = len;
    __asm__ volatile("csync");
    while (!(JL_UART0->CON0 & BIT(15)));
    JL_UART0->CON0 |= BIT(13);
}

SET_INTERRUPT
void uart0_irq(void)
{
    if ((JL_UART0->CON0 & BIT(5)) && (JL_UART0->CON0 & BIT(11))) {
        JL_UART0->CON0 |= BIT(7);
        JL_UART0->CON0 |= BIT(10);
        JL_UART0->CON0 |= BIT(12);
        __asm__ volatile("csync");
        __asm__ volatile("csync");
        ut_up_rxlen = JL_UART0->HRXCNT;
        if (ut_up_rxlen >= 4) {
            set_new_data_flag(1);
        }
        JL_UART0->RXSADR = (u32)ut_up_buf;
        JL_UART0->RXEADR = (u32)ut_up_buf + UT_UP_BUF_SIZE;
        JL_UART0->RXCNT = UT_UP_BUF_SIZE;
    }
}

void ut_device_mode(const char *ut_tx, const char *ut_rx, u32 baud)
{
    log_info("\n**** ut up mode *******\n\n");
    u32 ut_tx_io = get_gpio(ut_tx);
    u32 ut_rx_io = get_gpio(ut_rx);
    if ((baud) && (ut_tx_io < IO_PORT_MAX) && (ut_rx_io < IO_PORT_MAX)) {
        gpio_output_channle(ut_tx_io, CH0_UT0_TX);
        gpio_uart_input_channel(ut_rx_io, 0);
        request_irq(IRQ_UART0_IDX, 0, uart0_irq, 0);
        JL_UART0->CON0 = BIT(13) | BIT(12) | BIT(10);
        JL_UART0->BAUD = UT_UP_BAUD_CLK / baud / 4 - 1;
        JL_UART0->RXSADR = (u32)ut_up_buf;
        JL_UART0->RXEADR = (u32)ut_up_buf + UT_UP_BUF_SIZE;
        JL_UART0->RXCNT = UT_UP_BUF_SIZE;
        JL_UART0->OTCNT = UT_UP_BAUD_CLK / 1000; //1ms
        JL_UART0->CON0 |= BIT(5);   //OT中断
        JL_UART0->CON0 |= BIT(6);   //启动DMA
        JL_UART0->CON0 |= BIT(13) | BIT(12) | BIT(10) | BIT(0);
    }
}

void ut_devic_mode_close(void)
{
    JL_UART0->CON0 = BIT(13) | BIT(12) | BIT(10);
}
