#ifndef  __WDT_H__
#define  __WDT_H__

#include "common.h"

enum WDT_TIME {
    WDT_1ms = 0,
    WDT_2ms,
    WDT_4ms,
    WDT_8ms,
    WDT_16ms,
    WDT_32ms,
    WDT_64ms,
    WDT_128ms,
    WDT_256ms,
    WDT_512ms,
    WDT_1s,
    WDT_2s,
    WDT_4s,
    WDT_8s,
    WDT_16s,
    WDT_32s,
};


void wdt_init(enum WDT_TIME level);

void wdt_clr();


#endif  /*WDT_H*/

