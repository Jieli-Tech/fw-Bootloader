#ifndef __DELAY_H__
#define __DELAY_H__

#include "common.h"

void rc_udelay(u32 usec);

void udelay(u32 usec);

void mdelay(u32 msec);

void delay(volatile u32 t);

#endif
