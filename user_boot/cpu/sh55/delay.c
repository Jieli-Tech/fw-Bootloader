#include "delay.h"

void udelay(u32 usec)
{
    JL_TMR0->CON = BIT(6);
    JL_TMR0->CNT = 0;
    JL_TMR0->PRD = 48 * usec; //1us
    JL_TMR0->CON = BIT(0) | BIT(6);
    while ((JL_TMR0->CON & BIT(7)) == 0);
    JL_TMR0->CON = BIT(6);
}

void mdelay(u32 msec)
{
    for (int i = 0; i < msec; i++) {
        udelay(1000);
    }
}

void delay(volatile u32 t)
{
    while (t--) {
        asm("nop");
    }
}


volatile unsigned long jiffies = 10;
