#ifndef __UART_H__
#define __UART_H__

#include "common.h"


void uart_init(const char *tx_io, u32 baud);
void putchar(char a);
void uart_close(void);


#endif
