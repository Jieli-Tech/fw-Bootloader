#define ALIGNE(x) 		            __attribute__((aligned(x)))
#define sec(x) 			            __attribute__((section(#x)))
#define SEC(x) 			            __attribute__((section(#x)))
#define sec_used(x) 	            __attribute__((section(#x),used))
#define SET(x)                      __attribute__((x))
#define _GNU_PACKED_	            __attribute__((packed))
#define _INLINE_                    __attribute__((always_inline))
#define SET_INTERRUPT               __attribute__((interrupt("")))


typedef unsigned char               u8;
typedef char                        s8;
typedef unsigned short              u16;
typedef signed short                s16;
typedef unsigned int                u32;
typedef signed int                  s32;
typedef unsigned long long          u64;
typedef signed long long            s64;

#define BIT(n)                      (1 << n)
