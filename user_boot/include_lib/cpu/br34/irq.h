#ifndef  __IRQ_H__
#define  __IRQ_H__

#include "common.h"


#define IRQ_TIME0_IDX      4   	//0
#define IRQ_TIME1_IDX      5   	//0
#define IRQ_TIME2_IDX      6   	//0
#define IRQ_TIME3_IDX      7   	//0
#define IRQ_SPI0_IDX       14		//0
#define IRQ_SPI1_IDX       15		//0
#define IRQ_UART0_IDX      18		//0
#define IRQ_UART1_IDX      19		//0
#define IRQ_UART2_IDX      20		//0

#define IRQ_USB_SOF_IDX    8   	//1
#define IRQ_USB_CTRL_IDX   9  	//1

#define IRQ_SOFT0_IDX      60

extern u32 _IRQ_MEM_ADDR[];

#define IRQ_MEM_ADDR    (_IRQ_MEM_ADDR)


void local_irq_disable();

void local_irq_enable();

void irq_init(void);

void HWI_Install(unsigned char index, unsigned int isr, unsigned char priority);

#define     request_irq(idx,ip,hdl,arg) HWI_Install(idx,(int)hdl,ip)


static inline int core_num(void)
{
    return 0;
}

static inline int int_cli(void)
{
    int msg;
    asm volatile("cli %0" : "=r"(msg) :);
    return msg;
}

static inline void int_sti(int msg)
{
    asm volatile("sti %0" :: "r"(msg));
}


#define __asm_csync() \
    do { \
		asm volatile("csync;"); \
    } while (0)

#define OS_ENTER_CRITICAL()  \
	do { \
		local_irq_disable(); \
        __asm_csync(); \
	}while(0)


#define OS_EXIT_CRITICAL() \
	do { \
		local_irq_enable(); \
	}while(0)

#endif
