//*********************************************************************************//
// Module name : csfr.h                                                            //
// Description : j32CPU core sfr define                                            //
// By Designer : zequan_liu                                                        //
// Dat changed :                                                                   //
//*********************************************************************************//

#ifndef __J32CPU_CSFR__
#define __J32CPU_CSFR__
#define     core_num()  0
#define __RW      volatile       // read write
#define __RO      volatile const // only read
#define __WO      volatile       // only write

#define __u8      unsigned int   // u8  to u32 special for struct
#define __u16     unsigned int   // u16 to u32 special for struct
#define __u32     unsigned int

//---------------------------------------------//
// j32CPU define
//---------------------------------------------//

#define j32CPU_sfr_base     0x80000
#define j32CPU_sfr_offset   0x10000  // multi_core used

#define j32CPU_cpu_base    (j32CPU_sfr_base + 0x00)
#define j32CPU_mpu_base    (j32CPU_sfr_base + 0x80)

#define j32CPU(n)          ((JL_TypeDef_j32CPU     *)(j32CPU_sfr_base + j32CPU_sfr_offset*n))
#define j32CPU_mpu(n)      ((JL_TypeDef_j32CPU_MPU *)(j32CPU_mpu_base + j32CPU_sfr_offset*n))

//---------------------------------------------//
// j32CPU core sfr
//---------------------------------------------//

typedef struct {
    /* 00 */  __RO __u32 DR00;
    /* 01 */  __RO __u32 DR01;
    /* 02 */  __RO __u32 DR02;
    /* 03 */  __RO __u32 DR03;
    /* 04 */  __RO __u32 DR04;
    /* 05 */  __RO __u32 DR05;
    /* 06 */  __RO __u32 DR06;
    /* 07 */  __RO __u32 DR07;
    /* 08 */  __RO __u32 DR08;
    /* 09 */  __RO __u32 DR09;
    /* 0a */  __RO __u32 DR10;
    /* 0b */  __RO __u32 DR11;
    /* 0c */  __RO __u32 DR12;
    /* 0d */  __RO __u32 DR13;
    /* 0e */  __RO __u32 DR14;
    /* 0f */  __RO __u32 DR15;

    /* 10 */  __RO __u32 SR00;
    /* 11 */  __RO __u32 SR01;
    /* 12 */  __RO __u32 SR02;
    /* 13 */  __RO __u32 SR03;
    /* 14 */  __RO __u32 SR04;
    /* 15 */  __RO __u32 SR05;
    /* 16 */  __RO __u32 SR06;
    /* 17 */  __RO __u32 SR07;
    /* 18 */  __RO __u32 SR08;
    /* 19 */  __RO __u32 SR09;
    /* 1a */  __RO __u32 SR10;
    /* 1b */  __RO __u32 SR11;
    /* 1c */  __RO __u32 SR12;
    /* 1d */  __RO __u32 SR13;
    /* 1e */  __RO __u32 SR14;
    /* 1f */  __RO __u32 SR15;

    /* 20 */  __RW __u32 BPCON;
    /* 21 */  __RW __u32 BSP;
    /* 22 */  __RW __u32 BP0;
    /* 23 */  __RW __u32 BP1;
    /* 24 */  __RW __u32 BP2;
    /* 25 */  __RW __u32 BP3;
    /*    */  __RO __u32 REV_30_25[0x30 - 0x25 - 1];

    /* 30 */  __RW __u32 PMU_CON;
    /*    */  __RO __u32 REV_34_30[0x34 - 0x30 - 1];
    /* 34 */  __RW __u32 EMU_CON;
    /* 35 */  __RW __u32 EMU_MSG;
    /*    */  __RO __u32 REV_38_35[0x38 - 0x35 - 1];
    /* 38 */  __RW __u8  TTMR_CON;
    /* 39 */  __RW __u32 TTMR_CNT;
    /* 3a */  __RW __u32 TTMR_PRD;
    /*    */  __RO __u32 REV_3e_3a[0x3e - 0x3a - 1];
    /* 3e */  __RW __u32 BANK_CON;
    /* 3f */  __RW __u32 BANK_NUM;

    /* 40 */  __RW __u32 ICFG00;
    /* 41 */  __RW __u32 ICFG01;
    /* 42 */  __RW __u32 ICFG02;
    /* 43 */  __RW __u32 ICFG03;
    /* 44 */  __RW __u32 ICFG04;
    /* 45 */  __RW __u32 ICFG05;
    /* 46 */  __RW __u32 ICFG06;
    /* 47 */  __RW __u32 ICFG07;
    /* 48 */  __RW __u32 ICFG08;
    /* 49 */  __RW __u32 ICFG09;
    /* 4a */  __RW __u32 ICFG10;
    /* 4b */  __RW __u32 ICFG11;
    /* 4c */  __RW __u32 ICFG12;
    /* 4d */  __RW __u32 ICFG13;
    /* 4e */  __RW __u32 ICFG14;
    /* 4f */  __RW __u32 ICFG15;

    /* 50 */  __RW __u32 ICFG50;
    /* 51 */  __RW __u32 ICFG51;
    /* 52 */  __RW __u32 ICFG52;
    /* 53 */  __RW __u32 ICFG53;
    /* 54 */  __RW __u32 ICFG54;
    /* 55 */  __RW __u32 ICFG55;
    /* 56 */  __RW __u32 ICFG56;
    /* 57 */  __RW __u32 ICFG57;
    /* 58 */  __RW __u32 ICFG58;
    /* 59 */  __RW __u32 ICFG59;
    /* 5a */  __RW __u32 ICFG5a;
    /* 5b */  __RW __u32 ICFG5b;
    /* 5c */  __RW __u32 ICFG5c;
    /* 5d */  __RW __u32 ICFG5d;
    /* 5e */  __RW __u32 ICFG5e;
    /* 5f */  __RW __u32 ICFG5f;

    /* 60 */  __RO __u32 IPND0;
    /* 61 */  __RO __u32 IPND1;
    /* 62 */  __RO __u32 IPND2;
    /* 63 */  __RO __u32 IPND3;
    /* 64 */  __RO __u32 IPND4;
    /* 65 */  __RO __u32 IPND5;
    /* 66 */  __RO __u32 IPND6;
    /* 67 */  __RO __u32 IPND7;
    /* 68 */  __WO __u32 ILAT_SET;
    /* 69 */  __WO __u32 ILAT_CLR;
    /*    */  __RO __u32 REV_70_69[0x70 - 0x69 - 1];

} JL_TypeDef_j32CPU;

#undef __RW
#undef __RO
#undef __WO

#undef __u8
#undef __u16
#undef __u32

#endif

//*********************************************************************************//
//                                                                                 //
//                               end of this module                                //
//                                                                                 //
//*********************************************************************************//





