#ifndef __COMMON_H__
#define __COMMON_H__

#include "string.h"
#include "stdarg.h"
#include "cpu.h"
#include "csfr.h"
#include "typedef.h"
#include "device_errno.h"



#define SFR(sfr, start, len, dat)   (sfr = (sfr & ~((~(0xffffffff << (len))) << (start))) | (((dat) & (~(0xffffffff << (len)))) << (start)))

extern volatile unsigned long jiffies ;
#define time_after(a,b)					((long)(b) - (long)(a) < 0)
#define time_before(a,b)				time_after(b,a)

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif


#ifdef __PRINTF_DEBUG
#define APP_DEBUG    1
#else
#define APP_DEBUG    0
#endif

#if APP_DEBUG
#define log_info			printf
#define log_info_hexdump    printf_buf
#else
#define log_info(...)
#define log_info_hexdump(a,b)
#endif

#define ASSERT(a,...)   \
		do { \
			if(!(a)){ \
				log_info("file:%s, line:%d", __FILE__, __LINE__); \
				log_info("ASSERT-FAILD: "#a" "__VA_ARGS__); \
                while(1);\
			} \
		}while(0);


void wdt_clr();

#endif

