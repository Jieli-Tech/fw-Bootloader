#ifndef  __IRQ_H__
#define  __IRQ_H__

#include "common.h"

#define IRQ_EMUEXCPT_IDX   0
#define IRQ_EXCEPTION_IDX  1
#define IRQ_SYSCALL_IDX    2
#define IRQ_TICKTMR_IDX    3
#define IRQ_TIME0_IDX      4
#define IRQ_TIME1_IDX      5
#define IRQ_TIME2_IDX      6

#define IRQ_SPI0_IDX       16
#define IRQ_SPI1_IDX       17
#define IRQ_UART0_IDX      10
#define IRQ_UART1_IDX      11
#define IRQ_USB_SOF_IDX     21
#define IRQ_USB_CTRL_IDX    22

extern u32 _IRQ_MEM_ADDR[];

#define IRQ_MEM_ADDR    (_IRQ_MEM_ADDR)


void local_irq_disable();

void local_irq_enable();

// void rom_irq_init(void);
// void mask_init(void *exp_hook);
void mask_init(void *exp_hook, void *pchar);

void irq_init(void);

void HWI_Install(unsigned char index, unsigned int isr, unsigned char priority);

#define     request_irq(idx,ip,hdl,arg) HWI_Install(idx,(int)hdl,ip)

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
