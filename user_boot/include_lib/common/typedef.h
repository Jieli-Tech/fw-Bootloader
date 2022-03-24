#ifndef  __TYPEDEF_H__
#define  __TYPEDEF_H__




#define ALIGNE(x) 		            __attribute__((aligned(x)))
#define sec(x) 			            __attribute__((section(#x)))
#define SEC(x) 			            __attribute__((section(#x)))
#define sec_used(x) 	            __attribute__((section(#x),used))
#define SET(x)                      __attribute__((x))
#define _GNU_PACKED_	            __attribute__((packed))
#define _INLINE_                    __attribute__((always_inline))
#define SET_INTERRUPT               __attribute__((interrupt("")))
#define AT(x)                       __attribute__((section(#x)))
#define _WEAK_	                    __attribute__((weak))

typedef unsigned char               u8;
typedef char                        s8;
typedef unsigned short              u16;
typedef signed short                s16;
typedef unsigned int                u32;
typedef signed int                  s32;
typedef unsigned long long          u64;
typedef signed long long            s64;

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;

#define BIT(n)                      (1 << n)

#define FALSE    0
#define TRUE    1
#define false    0
#define true    1
#ifndef NULL
#define NULL    0
#endif // NULL
#endif  /*TYPEDEF_H*/
