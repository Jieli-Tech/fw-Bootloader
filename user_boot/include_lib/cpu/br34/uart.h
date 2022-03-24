#ifndef __UART_H__
#define __UART_H__

#include "common.h"



void uart_init(const char *tx_io, u32 baud);
void putchar(char a);
void uart_close(void);

void ut_device_mode(const char *ut_tx, const char *ut_rx, u32 baud);


#endif
