#ifndef HWI_H
#define HWI_H

#include "common.h"

//=================================================
#define IRQ_EMUEXCPT_IDX   		0		//0
#define IRQ_EXCEPTION_IDX  		1		//0
#define IRQ_SYSCALL_IDX    		2		//0
#define IRQ_TICK_TMR_IDX   		3		//0
#define IRQ_TIME0_IDX      		4   	//0
#define IRQ_TIME1_IDX      		5   	//0
#define IRQ_TIME2_IDX      		6   	//0
#define IRQ_TIME3_IDX      		7   	//0
#define IRQ_TIME4_IDX      		62   	//0
#define IRQ_TIME5_IDX      		63   	//0

#define IRQ_UART0_IDX      		19		//0
#define IRQ_UART1_IDX      		20		//0
#define IRQ_UART2_IDX      		21		//0

#define IRQ_SPI0_IDX       		15		//0
#define IRQ_SPI1_IDX       		16		//0
#define IRQ_SPI2_IDX       		37		//0

#define IRQ_SD0_IDX        		17		//0
#define IRQ_SD1_IDX        		18		//0

#define IRQ_IIC_IDX        		23		//0

#define IRQ_USB_SOF_IDX    		8   //1
#define IRQ_USB_CTRL_IDX   		9  	//1
#define IRQ_USB1_SOF_IDX    	53   //1
#define IRQ_USB1_CTRL_IDX   	54  	//1

#define IRQ_PORT_IDX		   	14		//0
#define IRQ_GPC_IDX		   		26		//0
#define IRQ_MCTMRX_IDX		   	33		//1
#define IRQ_CTM_IDX			   	36		//1
#define IRQ_OSA_IDX			   	28
#define IRQ_LRCT_IDX		   	44       //1
#define IRQ_RDEC0_IDX		   	32		//1
#define IRQ_SBC_IDX			   	46		//1
#define IRQ_AES_IDX        		31

#define IRQ_AUDIO_IDX      		13		//1
#define IRQ_ALINK0_IDX     		11  	//1
#define IRQ_ALINK1_IDX     		12  	//1

#define IRQ_BT_CLKN_IDX    		41		//2
#define IRQ_BT_DBG_IDX     		42		//1
#define IRQ_BLE_RX_IDX     		29		//2
#define IRQ_BLE_EVENT_IDX  		45		//1

#define IRQ_WL_LOFC_IDX    		43		//2
#define IRQ_BREDR_IDX      		40		//2

#define IRQ_SRC_IDX       		58		//1
#define IRQ_EQ_IDX         		60		//1
#define IRQ_DMA_COPY_IDX        48		//1

#define IRQ_IMD_IDX	       		65		//1
#define IRQ_JPG_IDX	       		55		//1

#define IRQ_SOFT0_IDX      		120
#define IRQ_SOFT1_IDX      		121
#define IRQ_SOFT2_IDX      		122
#define IRQ_SOFT3_IDX      		123
#define IRQ_SOFT4_IDX      		124
#define IRQ_SOFT5_IDX      		125
#define IRQ_SOFT6_IDX      		126
#define IRQ_SOFT7_IDX      		127

#define IRQ_MEM_ADDR    (0x1c7fe00)
#define ISR_ENTRY		IRQ_MEM_ADDR

#define MAX_IRQ_ENTRY_NUM   64
static inline int core_num(void)
{
//  u32 num;
//  asm volatile ("%0 = cnum" : "=r"(num) :);
//  return num;
    return 0;
}

static inline int current_cpu_id()
{
    return 0;
}

void interrupt_init();

void local_irq_enable();
void local_irq_disable();

//---------------------------------------------//
// interrupt cli/sti
//---------------------------------------------//

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

// #define INSTALL_HWI(a,b,c)\
//         HWI_Install(a,b,c)

void HWI_Install(unsigned int index, unsigned int isr, unsigned int priority);

void bit_clr_ie(unsigned char index);
void bit_set_ie(unsigned char index);

void irq_init();
void request_irq(u8 index, u8 priority, void (*handler)(void), u8 cpu_id);
void reg_set_ip(unsigned char index, unsigned char priority, u8 cpu_id);

void ENABLE_INT(void) ;
#define enable_int ENABLE_INT
void DISABLE_INT(void) ;
#define disable_int DISABLE_INT
#ifdef IRQ_TIME_COUNT_EN
void irq_handler_enter(int irq);

void irq_handler_exit(int irq);

void irq_handler_times_dump();


#else

#define irq_handler_enter(irq)      do { }while(0)
#define irq_handler_exit(irq)       do { }while(0)
#define irq_handler_times_dump()    do { }while(0)

#endif

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

