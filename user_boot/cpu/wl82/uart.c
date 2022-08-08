#include "uart.h"
#include "gpio.h"
#include "irq.h"

#define LOG_TAG_CONST       UPGRADE
#define LOG_TAG             "[uart]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#include "log.h"

#define UART_BAUD_CLK      48000000
static u8 used_tx_io;

//打印使用UART2，用户只能使用UART0和UART1
#define DEBUG_UART  JL_UART2

__attribute__((always_inline_when_const_args))
void putchar(char a)
{
    if (DEBUG_UART->CON0 & BIT(0)) {
        if (a == '\n') {
            DEBUG_UART->BUF = '\r';
            __asm__ volatile("csync");
            while ((DEBUG_UART->CON0 & BIT(15)) == 0);
            DEBUG_UART->CON0 |= BIT(13);
        }
        DEBUG_UART->BUF = a;
        __asm__ volatile("csync");
        while ((DEBUG_UART->CON0 & BIT(15)) == 0);
        DEBUG_UART->CON0 |= BIT(13);
    }
}


static void uart_set(JL_UART_TypeDef *UART, u32 baud, u32 txio, u32 rxio)//设置IO、占据问题、输出outch、波特率，不设置DMA和中断使能
{
    SFR(JL_CLOCK->CLK_CON1, 10, 2, 1);//串口模块使用pll48
    if (UART == JL_UART0) {
        JL_IOMAP->CON1 &= ~(0x0f << 8);//uart0 output_ch_0
        JL_IOMAP->CON1 |= 0 << 8;//uart0
        JL_IOMAP->CON3 &= ~(0X7 << (4 * 0));//不占据IO
        gpio_output_channle(txio, CH0_UT0_TX);
        if (rxio != (u32) - 1) {
            gpio_uart_rx_input(rxio, 0, 0);
        }
    } else if (UART == JL_UART1) {
        JL_IOMAP->CON3 &= ~(0x0f << 20);//uart1 output_ch_1
        JL_IOMAP->CON3 |= 0x01 << 20;//uart1 output_ch_1
        JL_IOMAP->CON3 &= ~(0X7 << (4 * 1));//不占据IO
        gpio_output_channle(txio, CH1_UT1_TX);
        if (rxio != (u32) - 1) {
            gpio_uart_rx_input(rxio, 1, 1);
        }
    } else {
        JL_IOMAP->CON3 &= ~(0x0f << 24);//uart2 output_ch_2
        JL_IOMAP->CON3 |= 0x07 << 24;//uart2 output_ch_2
        JL_IOMAP->CON3 &= ~(0X7 << (4 * 2));//不占据IO
        gpio_output_channle(txio, CH2_UT2_TX);
        if (rxio != (u32) - 1) {
            gpio_uart_rx_input(rxio, 2, 2);
        }
    }
    UART->CON0 = 0;
    UART->CON0 &= ~BIT(0);
    UART->CON0 |= BIT(13) | BIT(12) | BIT(10);
    if ((UART_BAUD_CLK / baud) % 4 < (UART_BAUD_CLK / baud) % 3) {
        UART->CON0 &= ~BIT(4);
        UART->BAUD = (UART_BAUD_CLK / baud) / 4 - 1;
    } else {
        UART->CON0 |= BIT(4);
        UART->BAUD = (UART_BAUD_CLK / baud) / 3 - 1;
    }
    UART->CON0 |= BIT(13) | BIT(12) | BIT(10) | BIT(0);
}


static const u32 gpio_regs[] = {
    (u32) JL_PORTA,
    (u32) JL_PORTB,
    (u32) JL_PORTC,
    (u32) JL_PORTD,
    (u32) JL_PORTE,
    (u32) JL_PORTF,
    (u32) JL_PORTG,
    (u32) JL_PORTH,
};

#define __gpio_mask(gpio) \
	BIT((gpio) % IO_GROUP_NUM)

static struct gpio_reg *gpio2reg(u32 gpio)
{
    if (gpio > IO_MAX_NUM) {
        return NULL;
    }
    return (struct gpio_reg *)gpio_regs[gpio / IO_GROUP_NUM];
}
void uart_init(const char *tx_pin, u32 baud)
{
    u32 io = get_gpio(tx_pin);
    if (io < IO_MAX_NUM) {
        uart_set(DEBUG_UART, baud, io, (u32) - 1);
        used_tx_io = io;
    }
}
void uart_close(void)
{
    if (DEBUG_UART->CON0 & BIT(0)) {
        DEBUG_UART->CON0 = 0;
        gpio_set_pull_up(used_tx_io, 0);
        gpio_set_pull_down(used_tx_io, 0);
        gpio_set_die(used_tx_io, 0);
        gpio_set_direction(used_tx_io, 1);
        u32 ch = DEBUG_UART == JL_UART0 ? CH0_UT0_TX : \
                 (DEBUG_UART == JL_UART1 ? CH1_UT1_TX : CH2_UT2_TX);
        gpio_clear_output_channle(used_tx_io, ch);
    }
}



#define UT_UP_BUF_SIZE      4128    //4096 + 32
static u8 ut_up_buf[UT_UP_BUF_SIZE] ALIGNE(32);
static volatile u32 ut_up_rxlen = 0;//使用volatile型
static volatile u8 ut_new_data = 0;//使用volatile型

static u8 ut1_up_buf[UT_UP_BUF_SIZE] ALIGNE(32);
static volatile u32 ut1_up_rxlen = 0;//使用volatile型
static volatile u8 ut1_new_data = 0;//使用volatile型

static u32 user_uart0_tx, user_uart0_rx;
static u32 user_uart1_tx, user_uart1_rx;

extern void set_new_data_flag(int flag);

u8 *get_ut_up_buf_p(void)
{
    return ut_up_buf;
}
u8 *get_uart1_buf(void)
{
    return ut1_up_buf;
}
u32 get_ut_up_rxlen(void)
{
    return ut_up_rxlen;
}
u32 get_uart1_rxlen(void)
{
    return ut1_up_rxlen;
}
void ut_up_tx_data(u8 *buf, u32 len)
{
    JL_UART0->TXADR = (u32)buf;
    JL_UART0->TXCNT = len;
    __asm__ volatile("csync");
    while (!(JL_UART0->CON0 & BIT(15)));
    JL_UART0->CON0 |= BIT(13);
}
void uart1_tx_data(u8 *buf, u32 len)
{
    JL_UART1->TXADR = (u32)buf;
    JL_UART1->TXCNT = len;
    __asm__ volatile("csync");
    while (!(JL_UART1->CON0 & BIT(15)));
    JL_UART1->CON0 |= BIT(13);
}

SET_INTERRUPT
static void uart0_irq(void)
{
    if (JL_UART0->CON0 & (BIT(15) | BIT(2))) {
        JL_UART0->CON0 |= BIT(13);
    }
    if (JL_UART0->CON0 & (BIT(11) | BIT(15))) {
        JL_UART0->CON0 |= BIT(7);
        __asm__ volatile("csync");
        JL_UART0->CON0 |= BIT(10) | BIT(12);
        __asm__ volatile("csync");
        ut_up_rxlen = JL_UART0->HRXCNT;
        if (ut_up_rxlen >= 1) {
            set_new_data_flag(1);
            ut_new_data = 1;
        }
        if (ut_up_rxlen < UT_UP_BUF_SIZE) {
            ut_up_buf[ut_up_rxlen] = 0;
        }
        JL_UART0->RXSADR = (u32)ut_up_buf;
        JL_UART0->RXEADR = (u32)ut_up_buf + UT_UP_BUF_SIZE;
        JL_UART0->RXCNT = UT_UP_BUF_SIZE;
    }
    __asm__ volatile("csync");
}
SET_INTERRUPT
static void uart1_irq(void)
{
    if (JL_UART1->CON0 & (BIT(15) | BIT(2))) {
        JL_UART1->CON0 |= BIT(13);
    }
    if (JL_UART1->CON0 & (BIT(11) | BIT(15))) {
        JL_UART1->CON0 |= BIT(7);
        __asm__ volatile("csync");
        JL_UART1->CON0 |= BIT(10) | BIT(12);
        __asm__ volatile("csync");
        ut1_up_rxlen = JL_UART1->HRXCNT;
        if (ut1_up_rxlen >= 1) {
            set_new_data_flag(1);
            ut1_new_data = 1;
        }
        if (ut1_up_rxlen < UT_UP_BUF_SIZE) {
            ut1_up_buf[ut1_up_rxlen] = 0;
        }
        JL_UART1->RXSADR = (u32)ut1_up_buf;
        JL_UART1->RXEADR = (u32)ut1_up_buf + UT_UP_BUF_SIZE;
        JL_UART1->RXCNT = UT_UP_BUF_SIZE;
    }
    __asm__ volatile("csync");
}
void ut_device_mode(const char *ut_tx, const char *ut_rx, u32 baud)//ut_tx ut_rx PA-PH任意IO
{
    u32 ut_tx_io = get_gpio(ut_tx);
    u32 ut_rx_io = get_gpio(ut_rx);
    if ((baud) && (ut_tx_io <= IO_PORT_USB_DMB) && (ut_rx_io <= IO_PORT_USB_DMB)) {
        if (ut_tx_io >= IO_PORT_USB_DMA) {
            printf("err uart0 cannot use usb0 port ,please use uart1_usb0_init()\n");
            return;
        }
        user_uart0_tx = ut_tx_io;
        user_uart0_rx = ut_rx_io;
        uart_set(JL_UART0, baud, ut_tx_io, ut_rx_io);
        request_irq(IRQ_UART0_IDX, 0, uart0_irq, 0);
        JL_UART0->RXSADR = (u32)ut_up_buf;
        JL_UART0->RXEADR = (u32)ut_up_buf + UT_UP_BUF_SIZE;
        JL_UART0->RXCNT = UT_UP_BUF_SIZE;
        JL_UART0->OTCNT = UART_BAUD_CLK / 1000; //1ms
        JL_UART0->CON0 |= BIT(5) | BIT(3); //OT中断,RX中断
        JL_UART0->CON0 |= BIT(6);   //启动DMA
        JL_UART0->CON0 &= ~BIT(1);  //8bit
        JL_UART0->CON0 |= BIT(13) | BIT(12) | BIT(10) | BIT(0);
    }
}

void ut_devic_mode_close(void)
{
    JL_UART0->CON0 = BIT(13) | BIT(12) | BIT(10);
    JL_IOMAP->CON3 &= ~(0X7 << (4 * 0));//不占据IO
    gpio_set_pull_up(user_uart0_tx, 0);
    gpio_set_pull_down(user_uart0_tx, 0);
    gpio_set_die(user_uart0_tx, 0);
    gpio_set_direction(user_uart0_tx, 1);
    gpio_set_pull_up(user_uart0_rx, 0);
    gpio_set_pull_down(user_uart0_rx, 0);
    gpio_set_die(user_uart0_rx, 0);
    gpio_set_direction(user_uart0_rx, 1);
    gpio_clear_output_channle(user_uart0_tx, CH0_UT0_TX);
    /* gpio_uart_clear_rx_input(user_uart0_rx, 0, 0); */
}

void user_uart1_usb0_init(u32 baud)
{
    u32 txd = IO_PORT_USB_DPA, rxd = IO_PORT_USB_DMA;;
    user_uart1_tx = txd;
    user_uart1_rx = rxd;

    SFR(JL_CLOCK->CLK_CON1, 10, 2, 1);//串口模块使用pll48
    usb_iomode(0, 1);
    gpio_set_die(txd, 1);
    gpio_set_pull_up(txd, 1);
    gpio_set_pull_down(txd, 0);
    gpio_direction_output(txd, 1);

    gpio_set_die(rxd, 1);
    gpio_set_pull_up(rxd, 1);
    gpio_set_pull_down(rxd, 0);
    gpio_direction_input(rxd);

    JL_IOMAP->CON3 &= ~(0X7 << (4 * 1));
    JL_IOMAP->CON3 |= BIT(4 * 1 + 3);//占据IO
    JL_IOMAP->CON1 &= ~(0x3 << 2);
    JL_IOMAP->CON1 |= (0x3 << 2);//USBA
    request_irq(IRQ_UART1_IDX, 0, uart1_irq, 0);
    JL_UART1->CON0 = BIT(13) | BIT(12) | BIT(10);
    if ((UART_BAUD_CLK / baud) % 4 < (UART_BAUD_CLK / baud) % 3) {
        JL_UART1->CON0 &= ~BIT(4);
        JL_UART1->BAUD = (UART_BAUD_CLK / baud) / 4 - 1;
    } else {
        JL_UART1->CON0 |= BIT(4);
        JL_UART1->BAUD = (UART_BAUD_CLK / baud) / 3 - 1;
    }
    JL_UART1->RXSADR = (u32)ut1_up_buf;
    JL_UART1->RXEADR = (u32)ut1_up_buf + UT_UP_BUF_SIZE;
    JL_UART1->RXCNT = UT_UP_BUF_SIZE;
    JL_UART1->OTCNT = UART_BAUD_CLK / 1000; //1ms
    JL_UART1->CON0 |= BIT(5) | BIT(3); //OT中断,RX中断
    JL_UART1->CON0 |= BIT(6);   //启动DMA
    JL_UART1->CON0 &= ~BIT(1);  //8bit
    JL_UART1->CON0 |= BIT(13) | BIT(12) | BIT(10) | BIT(0);
}
void user_uart1_usb0_close()
{
    if (JL_UART1->CON0 & BIT(0)) {
        JL_UART1->CON0 = BIT(13) | BIT(12) | BIT(10);
        JL_IOMAP->CON1 &= ~(0x3 << 2);
        JL_IOMAP->CON3 &= ~(0X7 << (4 * 1));//不占据IO
        gpio_set_die(user_uart1_tx, 1);
        gpio_set_pull_up(user_uart1_tx, 0);
        gpio_set_pull_down(user_uart1_tx, 0);
        gpio_direction_input(user_uart1_tx);
        gpio_set_die(user_uart1_rx, 1);
        gpio_set_pull_up(user_uart1_rx, 0);
        gpio_set_pull_down(user_uart1_rx, 0);
    }
}
void user_uart1_usb0_tx_data(u8 *buf, u32 len)
{
    JL_UART1->TXADR = (u32)buf;
    JL_UART1->TXCNT = len;
    __asm__ volatile("csync");
    while (!(JL_UART1->CON0 & BIT(15)));
    JL_UART1->CON0 |= BIT(13);
}

void uart_test_task()
{
    void wdt_disable(void);
    wdt_disable();//关闭看门狗
    printf("========uart_test_task=========\n");
#if 1
    ut_device_mode("PA08", "PA09", 460800);
    while (1) {
        if (ut_new_data) {
            ut_new_data = 0;
            printf("---> ut_up_rxlen = %d \n", get_ut_up_rxlen());
            printf("---> recv = %s \n", get_ut_up_buf_p());
            ut_up_tx_data(get_ut_up_buf_p(), get_ut_up_rxlen());
        }
    }
#else
    user_uart1_usb0_init(460800);//USBA
    while (1) {
        if (ut1_new_data) {
            ut1_new_data = 0;
            printf("---> ut_up_rxlen = %d \n", get_uart1_rxlen());
            printf("---> recv = %s \n", get_uart1_buf());
            user_uart1_usb0_tx_data(get_uart1_buf(), get_uart1_rxlen());
        }
    }
#endif
}

