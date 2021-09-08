#ifndef __PRINTF_H__
#define __PRINTF_H__

#include "common.h"

int puts(const char *out);
int printf(const char *format, ...);
void printf_buf(u8 *buf, u32 len);
void put_buf(u8 *buf, u32 len);


#endif
