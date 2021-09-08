// hid_demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <ctime>
#include <hidsdi.h>
#include <setupapi.h>
#include<typeinfo>


#define HID_UART_MODE_SEL    0


#define OVERLAP_MODE         0                                                 /* 是否要启用重叠异步模式打开HID设备文件 */

typedef unsigned char               u8;
typedef char                        s8;
typedef unsigned short              u16;
typedef signed short                s16;
typedef unsigned int                u32;
typedef signed int                  s32;
typedef unsigned long long          u64;
typedef signed long long            s64;

#define BIT(n)                      (1 << n)
u32 erase_unit_size = 256;
u32 flash_eoffset_size = 0;
HANDLE                    dev_Handle;

typedef struct {

#define JL_SU_CMD_DEVICE_INIT   0xC0
#define JL_SU_CMD_DEVICE_CHECK  0xC1
#define JL_SU_CMD_ERASE         0xC2
#define JL_SU_CMD_WRITE         0xC3
#define JL_SU_CMD_FLASH_CRC     0xC4
#define JL_SU_CMD_EX_KEY        0xC5
#define JL_SU_CMD_REBOOT        0xCA

    u16 rand;
    u8 rsp_status;
    u8 cmd;        // 操作命令
    
    
    union {
        struct {
            u32 data_length;
            u8  data[0];
        } ex_key;

        struct {
            u32 address;		// 烧写FLASH地址
            u16 data_length;
            u16 data_crc;       // 烧写数据校验码
            u8  data[0];		// 烧写数据
        } write;

        struct {
            u32 address;    //需要对齐
#define JL_ERASE_TYPE_PAGE   1
#define JL_ERASE_TYPE_SECTOR 2
#define JL_ERASE_TYPE_BLOCK  3
            u32  type;
        } erase;

        struct {
            u32 address;
            u32 len;
            u32 block_size;
            u16 crc[0];
        } crc_list;

        struct {          
            u32 upgrade_addr;
            u32 upgrade_len;
            u32 upgrade_eoffset;
            u8 file_name[16];  
            u16 flash_alignsize;
            u8 mode;
        }  init;

        struct {
            u8 vid[4];
            u8 pid[16];            
            u32 sdk_id;
        }device_check;

    };

} JL_SECTOR_COMMAND_ITEM;
#define     MAX_BUFFER_SIZE     64



JL_SECTOR_COMMAND_ITEM *g_ops;
u16 communication_key=0;

void printf_buf(const u8 * p, DWORD len)
{
    printf("%p %d\n", p, len);
    for (u32 i = 0; i < len; i++) {
        printf("%02x ",  p[i]);
        if (i && ((i + 1) % 16 == 0)) {
            putchar('\n');
        }
    }
    putchar('\n');
}

void enc_data(u8* p, u32 l)
{
    if (communication_key) {
        for (int i = 0; i < l; i+=2) {
            p[i] ^= communication_key;
            p[i+1] ^= communication_key >> 8;
        }
    }
}

BOOL WriteDevice(LPCVOID lpBuffer, DWORD dwSize)
{
    DWORD dwRet;
    BOOL bRet;

	if (dwSize <= 64) {
#if HID_UART_MODE_SEL
		BYTE wBuffer[65] = { 0 };
		memcpy(&wBuffer[1], lpBuffer, dwSize < sizeof(wBuffer) ? dwSize : sizeof(wBuffer));
		enc_data(&wBuffer[1], 64);
#else
		BYTE wBuffer[64] = { 0 };
		memcpy(&wBuffer[0], lpBuffer, dwSize < sizeof(wBuffer) ? dwSize : sizeof(wBuffer));
		enc_data(&wBuffer[0], 64);
#endif
		bRet = WriteFile(dev_Handle, wBuffer, sizeof(wBuffer), &dwRet, NULL);

	}
	else {
		enc_data((u8 *)lpBuffer, dwSize);
		bRet = WriteFile(dev_Handle, lpBuffer, dwSize, &dwRet, NULL);
	}
#if HID_UART_MODE_SEL
#else
	PurgeComm(dev_Handle, PURGE_TXCLEAR | PURGE_RXCLEAR);//清一下发送和接收的缓存区
#endif

    //printf("WriteFile:%d --  %d   %d\n", dwSize, bRet, GetLastError());
    //printf_buf((const u8*)&wBuffer[0], dwSize);
	
    return bRet;
}

BOOL ReadDevice(LPVOID lpBuffer, DWORD dwSize)
{
    DWORD dwRet;
    BOOL bRet;

#if HID_UART_MODE_SEL
	BYTE wBuffer[65] = { 0 };
	bRet = ReadFile(dev_Handle, wBuffer, sizeof(wBuffer), &dwRet, NULL);
	enc_data(&wBuffer[1], 64);
	memcpy(lpBuffer, &wBuffer[1], dwSize);
#else
	BYTE wBuffer[64] = { 0 };
	int offset = 0;
	DWORD ev;
	while(offset < dwSize) {
		WaitCommEvent(dev_Handle, &ev, 0);
		bRet = ReadFile(dev_Handle, &wBuffer[offset], sizeof(wBuffer), &dwRet, NULL);
		offset += dwRet;
	}
	enc_data(&wBuffer[0], 64);
	memcpy(lpBuffer, &wBuffer[0], dwSize);
#endif

    //printf("ReadFile:%d--%d %d\n", bRet, dwSize, GetLastError());
    //printf_buf((const u8*)lpBuffer, dwSize);
    return bRet;
}

u16 chip_crc16(const u8 *ptr, u32 len)
{
    u16 crc_ta[16] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    };
    u16 crc;
    u8 da;

    crc = 0;
    while (len-- != 0) {
        da = crc >> 12;
        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr / 16)];

        da = crc >> 12;

        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr & 0x0f)];
        ptr++;
    }

    return (crc);
}
u16 chip_crc16_with_init(u16 crc,const u8* ptr, u32 len)
{
    u16 crc_ta[16] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    };
    //u16 crc;
    u8 da;

   // crc = 0;
    while (len-- != 0) {
        da = crc >> 12;
        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr / 16)];

        da = crc >> 12;

        crc <<= 4;
        crc ^= crc_ta[da ^ (*ptr & 0x0f)];
        ptr++;
    }

    return (crc);
}
u16 get_random()
{
    return rand();//0x1234;
}

u16 jl_ex_key()
{
    memset(g_ops, 0xff, 64);
    g_ops->cmd = JL_SU_CMD_EX_KEY;
    g_ops->rand = get_random();
    g_ops->ex_key.data_length = MAX_BUFFER_SIZE-0x8;// get_random() % 4 + 24;
    for (int i = 0; i < g_ops->ex_key.data_length; i++) {
        g_ops->ex_key.data[i] = (u8)get_random();
    }

    u16 key = chip_crc16_with_init(g_ops->rand,g_ops->ex_key.data, g_ops->ex_key.data_length) & 0xf0f0;
    printf("keyf0f0 0x%x\n", key);
    WriteDevice(g_ops, MAX_BUFFER_SIZE);

    ReadDevice(g_ops, MAX_BUFFER_SIZE);
    key |= chip_crc16_with_init(g_ops->rand,g_ops->ex_key.data, g_ops->ex_key.data_length) & 0x0f0f;
    printf("key0f0f 0x%x\n", key);
    return key;
}
u32 jl_device_init(char *name)
{
    g_ops->cmd = JL_SU_CMD_DEVICE_INIT;
    g_ops->rand = get_random();
    strcpy_s((char *)g_ops->init.file_name,16, name);
    g_ops->init.mode = 0;
    WriteDevice(g_ops, sizeof(*g_ops));

    ReadDevice(g_ops, sizeof(*g_ops));
    printf("%s() 0x%x %d %d %d\n", __FUNCTION__, g_ops->init.upgrade_addr, g_ops->init.upgrade_len,g_ops->init.flash_alignsize, g_ops->init.upgrade_eoffset);
    erase_unit_size = g_ops->init.flash_alignsize;
    flash_eoffset_size = g_ops->init.upgrade_eoffset;

    return g_ops->init.upgrade_addr;
 }
void jl_device_check(u32 sdk_id,u8 pid[],u8 vid[])
{
    g_ops->cmd = JL_SU_CMD_DEVICE_CHECK;
    g_ops->rand = get_random();
    g_ops->device_check.sdk_id = sdk_id;
    WriteDevice(g_ops, sizeof(*g_ops));
    ReadDevice(g_ops, sizeof(*g_ops));
    memcpy(pid, g_ops->device_check.pid, 16);
    memcpy(vid, g_ops->device_check.vid, 4);
    printf("%s() 0x%x pid:%s vid:%s\n", __FUNCTION__, sdk_id, pid, vid);
}
void _jl_crc_list(u32 addr,u32 block_count,u16 crc_list[])
{
    //printf("_jl_crc_list  @ %x %d %d\n", addr, block_count, block_count);
    g_ops->cmd = JL_SU_CMD_FLASH_CRC;
    g_ops->rand = get_random();
    g_ops->crc_list.address = addr;
    g_ops->crc_list.block_size = erase_unit_size;
    g_ops->crc_list.len = block_count * g_ops->crc_list.block_size;

    WriteDevice(g_ops, sizeof(*g_ops));

    ReadDevice(g_ops, 16 + block_count * 2);

    for (int i = 0; i < block_count; i++) {
        crc_list[i] = g_ops->crc_list.crc[i];
        //printf("%02x ", g_ops->crc_list.crc[i]);
    }
 }
void jl_crc_list(u32 addr, u32 len, u16 crc_list[])
{
    u32 block_count = len / erase_unit_size + !!(len % erase_unit_size);

    printf("jl_crc_list  @ 0x%x %d %d\n", addr, len, block_count);

    u32 offset = 0;
    u32 index=0;
    while (block_count)
    {
        u32 cnt = block_count > (MAX_BUFFER_SIZE - 0x10)/sizeof(u16) ? \
            (MAX_BUFFER_SIZE - 0x10) / sizeof(u16) : block_count;

        _jl_crc_list(addr + offset, cnt, &crc_list[index]);
        offset += erase_unit_size * cnt;
        index += cnt;
        block_count -= cnt;
    }
}

void _jl_erase(u32 addr, u32 len)
{
    printf("erase  @ 0x%x %d\n", addr, len);
    g_ops->cmd = JL_SU_CMD_ERASE;
    g_ops->rand = get_random();
    g_ops->erase.address = addr;
    if (len == 64 * 1024) {
        g_ops->erase.type = JL_ERASE_TYPE_BLOCK;
    }
    else if (len == 4096) {
        g_ops->erase.type = JL_ERASE_TYPE_SECTOR;
    }
    else if (len == 256) {
        g_ops->erase.type = JL_ERASE_TYPE_PAGE;
    }
    WriteDevice(g_ops, sizeof(*g_ops));
    ReadDevice(g_ops, sizeof(*g_ops));
    if (g_ops->rsp_status) {
        printf("erase failed @ 0x%x\n", g_ops->erase.address);
    }
}

void jl_erase(u32 addr, u32 len)
{
    printf("erase  @ 0x%x %d\n", addr, len);
    if (len == 256) {
        _jl_erase(addr,len);
    }else {
        while (len) {
            _jl_erase(addr, 4096);
            addr += 4096;
            len -= 4096;
        }
    }
}
#if 0
void jl_erase(u32 addr, u32 len)
{
    printf("erase  @ 0x%x %d\n", addr,len);
    g_ops->cmd = JL_SU_CMD_ERASE;
    g_ops->rand = get_random();
    g_ops->erase.address = addr;
    if (len == 64 * 1024) {
        g_ops->erase.type = JL_ERASE_TYPE_BLOCK;
    }
    else if (len == 4096) {
        g_ops->erase.type = JL_ERASE_TYPE_SECTOR;
    }
    else if (len == 256) {
        g_ops->erase.type = JL_ERASE_TYPE_PAGE;
    }
    WriteDevice(g_ops, sizeof(*g_ops));
    ReadDevice(g_ops, sizeof(*g_ops));
    if (g_ops->rsp_status) {
        printf("erase failed @ 0x%x\n", g_ops->erase.address);
    }
}
#endif
void jl_rboot(void)
{
    g_ops->cmd = JL_SU_CMD_REBOOT;
    g_ops->rand = get_random();
    WriteDevice(g_ops, sizeof(*g_ops));
    //该指令没有ack
}
void _jl_write(u8* buf, u32 addr, u32 len)
{
    //printf("_jl_write  @ %x %d\n", addr, len);
    g_ops->cmd = JL_SU_CMD_WRITE;
    g_ops->rand = get_random();
    g_ops->write.address = addr;
    g_ops->write.data_length = len;
    memcpy(g_ops->write.data, buf,g_ops->write.data_length);
    g_ops->write.data_crc = chip_crc16(g_ops->write.data, g_ops->write.data_length);

    WriteDevice(g_ops, 0xc + g_ops->write.data_length);
    ReadDevice(g_ops, sizeof(*g_ops));
    if (g_ops->rsp_status) {
        printf("_jl_write failed @ 0x%x\n", g_ops->write.address);
    }
}


void jl_write(u8* buf, u32 addr, u32 len)
{
    printf("jl_write  @ 0x%x %d\n", addr, len);
    u32 offset = 0;
    while (len)
    {
#if HID_UART_MODE_SEL
        u32 cnt = len > MAX_BUFFER_SIZE -  0xc? \
            MAX_BUFFER_SIZE -  0xc: len;
#else
		u32 cnt = len > erase_unit_size ? erase_unit_size : len;
#endif

        _jl_write(buf + offset, addr + offset, cnt);
        offset += cnt;
        len -= cnt;
    }
}
 
int main(int argc,char *argv[])
{
    srand(time(NULL));
    dev_Handle = NULL;
    printf("argc %d %s %s\n",argc, argv[0], argv[1]);

#if HID_UART_MODE_SEL

	int                       deviceNo = 0;
	bool                      result;

	GUID                      hidGuid;
	ULONG                     requiredLength;
	SP_DEVICE_INTERFACE_DATA  devInfoData;


    devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    printf("Begin to list all HID device...\r\n\r\n");

    /* HidD_GetHidGuid routine returns the device interfaceGUID for HIDClass devices - from MS */
    HidD_GetHidGuid(&hidGuid);

    printf("Get HID Guid: Data1[0x%X]. Data2[0x%X]. Data3[0x%X]. Data4[0x%X%X%X%X%X%X%X%X]\r\n", hidGuid.Data1, hidGuid.Data2, hidGuid.Data3,
        hidGuid.Data4[0], hidGuid.Data4[1], hidGuid.Data4[2], hidGuid.Data4[3], hidGuid.Data4[4], hidGuid.Data4[5], hidGuid.Data4[6], hidGuid.Data4[7]);

    /* SetupDiGetClassDevs返回一个包含本机上所有被请求的设备信息的设备信息集句柄 */
    /* SetupDiGetClassDevs function returns a handle to a device information set that contains requested device information elements for a local computer - from MS */
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        printf("Fatal Error: SetupDiGetClassDevs Fail!!!\r\n");
        return 1;
    }

    SetLastError(NO_ERROR);                                                    /* 首先清空错误代码，以便于后面的使用 */

    while (1) {
        /* The SetupDiEnumDeviceInterfaces function enumerates the device interfaces that are contained in a device information set - from MS */
        result = SetupDiEnumInterfaceDevice(hDevInfo, 0, &hidGuid, deviceNo, &devInfoData);

        if ((result == false) || (GetLastError() == ERROR_NO_MORE_ITEMS)) {    /* 出现ERROR_NO_MORE_ITEMS错误表示已经找完了所有的设备 */
            printf("No More Item Left!!!\r\n\r\n");
            break;
        }
        else {
         //   printf("----Get devInfoData: cbSize[%d]. Flags[0x%.2X]. InterfaceClassGuid([0x%X] [0x%X] [0x%X] [0x%X%X%X%X%X%X%X%X])\r\n",
         //       devInfoData.cbSize, devInfoData.Flags, devInfoData.InterfaceClassGuid.Data1, devInfoData.InterfaceClassGuid.Data2, devInfoData.InterfaceClassGuid.Data3,
         //       devInfoData.InterfaceClassGuid.Data4[0], devInfoData.InterfaceClassGuid.Data4[1], devInfoData.InterfaceClassGuid.Data4[2], devInfoData.InterfaceClassGuid.Data4[3],
         //       devInfoData.InterfaceClassGuid.Data4[4], devInfoData.InterfaceClassGuid.Data4[5], devInfoData.InterfaceClassGuid.Data4[6], devInfoData.InterfaceClassGuid.Data4[7]);
        }

        /* The SetupDiGetDeviceInterfaceDetail function returns details about a device interface - From MS */

        requiredLength = 0;                                                                                       /* 先将变量置零，以便于下一步进行获取 */
        SetupDiGetInterfaceDeviceDetail(hDevInfo, &devInfoData, NULL, 0, &requiredLength, NULL);                  /* 第一次调用，为了获取requiredLength */
        PSP_INTERFACE_DEVICE_DETAIL_DATA devDetail = (SP_INTERFACE_DEVICE_DETAIL_DATA*)malloc(requiredLength);   /* 根据获取到的长度申请动态内存 */
        devDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);                                              /* 先对变量进行部分初始化 */
        result = SetupDiGetInterfaceDeviceDetail(hDevInfo, &devInfoData, devDetail, requiredLength, NULL, NULL);  /* 第二次调用，为了获取devDetail */

        if (result == false) {
            printf("Fatal Error: SetupDiGetInterfaceDeviceDetail fail!!!\r\n");
            free(devDetail);
            SetupDiDestroyDeviceInfoList(hDevInfo);
            return 1;
        }else {
            //printf("----Get devDetail: cbSize[%d] DevicePath[%s]\r\n", devDetail->cbSize, devDetail->DevicePath);
        }

        if (OVERLAP_MODE == 1) {
            dev_Handle = CreateFile(devDetail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        }
        else {
            dev_Handle = CreateFile(devDetail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        }

        free(devDetail);

        if (dev_Handle == INVALID_HANDLE_VALUE) {                               /* 系统会将部分HID设备设置成独占模式 */
            printf("CreateFile fail!!! dev maybe in_use. continue to try next one...\r\n");
            ++deviceNo;
            continue;
        }

        _HIDD_ATTRIBUTES hidAttributes;

        result = HidD_GetAttributes(dev_Handle, &hidAttributes);                /* 获取HID设备的属性 */

        if (result == false) {
            printf("Fatal Error: HidD_GetAttributes fail!!!\r\n");
            CloseHandle(dev_Handle);
            SetupDiDestroyDeviceInfoList(hDevInfo);
            return 1;
        }
        else {          
            if ((hidAttributes.ProductID == 0x4155) && (hidAttributes.VendorID == 0x4c4a)) {
                printf("find hid succ  \n");
                break;
            }
        }

        CloseHandle(dev_Handle);
        ++deviceNo;
    }
    /* The SetupDiDestroyDeviceInfoList function deletes a device information set and frees all associated memory - From MS */
    SetupDiDestroyDeviceInfoList(hDevInfo);

	

#else

	int ret = 0;
 
    char com_name[20] = { 0 };
    //sprintf_s(com_name, sizeof(com_name), "\\\\.\\COM%s", "4");//固定uart号
    char uart_port_num[10];
    printf("please input com num:"); 
    gets_s(uart_port_num,10);
    sprintf_s(com_name, sizeof(com_name), "\\\\.\\COM%s", uart_port_num);//手动输入

	//dev_Handle = CreateFileA("\\\\.\\COM7", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    dev_Handle = CreateFileA(com_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (dev_Handle == INVALID_HANDLE_VALUE) {
		ret = GetLastError();
		printf("ERROR: Failed to open %s, error code = 0x%x(%d)", com_name, ret, ret);
		while (1);
	}
	printf("串口打开创建成功 ！\n");
	DCB dcb;
	BOOL bRet = GetCommState(dev_Handle, &dcb);//先获取com口的默认配置
	if (!bRet) {
		ret = GetLastError();
		printf("ERROR: GetCommState failed, error code = 0x%x(%d)", ret, ret);
		while (1);
	}
	dcb.BaudRate = 1000000;		// 波特率
	dcb.StopBits = ONESTOPBIT;  // 停止位
	dcb.ByteSize = 8;		    // 数据位
	dcb.Parity = NOPARITY;		// 校验位
	bRet = SetCommState(dev_Handle, &dcb);//再设置回去
	if (!bRet) {
		ret = GetLastError();
		printf("ERROR: SetCommState failed, error code = 0x%x(%d)", ret, ret);
		while (1);
	}
	PurgeComm(dev_Handle, PURGE_TXCLEAR| PURGE_RXCLEAR);//清一下发送和接收的缓存区
	SetCommMask(dev_Handle, EV_RXCHAR);
	printf("完成参数配置 ！\n");

#endif

    communication_key = 0;
    {
        u32* p = new u32[(4096+32) / 4];
        g_ops = (JL_SECTOR_COMMAND_ITEM * )p;
    }
   
    communication_key =  jl_ex_key();
    printf("communication_key 0x%x --- %d\n", communication_key,sizeof(*g_ops));

    u32 app_zone_addr = jl_device_init((char *)"app_dir_head");
    u8 pid[16 + 1] = { 0 };
    u8 vid[4 + 1] = { 0 };
    jl_device_check(0x12345678, pid,vid);

    FILE* fp;
    int r;

    ///关于升级文件的说明，如果你的下载目录下ini有EOFFSET=1;（见说明生成0k/4k文件)，打开宏。 如果没有则使用jl_isd.bin
#if 1
    if (flash_eoffset_size == 4096) {
        printf("4K .bin\n");
        r = fopen_s(&fp, "jl_isd_4K.bin", "rb");
    } else {
        printf("0K .bin\n");
        r = fopen_s(&fp, "jl_isd_0K.bin", "rb");
    }
#else
    r = fopen_s(&fp,"jl_isd.bin", "rb");
#endif

    if (fp == NULL) {
        printf("open file error");
        return -1;
    }
	
#if 0 //升级 uboot 的操作

	u32 uboot_zone_addr = jl_device_init((char *)"uboot_zone");
    u32 abs_64K_len = 65536 - flash_eoffset_size;
	u8* uboot_file_buf = new u8[abs_64K_len];//1024*64
	fseek(fp, 0, SEEK_SET);
	fread(uboot_file_buf, 1, abs_64K_len, fp);

	printf_buf(uboot_file_buf, 64);

	jl_erase(app_zone_addr, erase_unit_size);
	jl_erase(65536, abs_64K_len);
	for (int i = 0; i < abs_64K_len /erase_unit_size; i ++) {
		jl_write(&uboot_file_buf[i * erase_unit_size], 65536 + i * erase_unit_size, erase_unit_size);
	}
	
	u32 uboot_block_count = abs_64K_len /erase_unit_size;
	u16* uboot_chip_crc_list = new u16[uboot_block_count];
    u16* uboot_file_crc_list = new u16[uboot_block_count];
	jl_crc_list(65536, abs_64K_len, uboot_chip_crc_list);
	
	u32 uboot_offset = 0;
    u32 uboot_len = abs_64K_len;
    while (uboot_len){
        u32 uboot_cnt = uboot_len > erase_unit_size ? erase_unit_size : uboot_len;
        uboot_file_crc_list[uboot_offset / erase_unit_size] = chip_crc16(&uboot_file_buf[uboot_offset], uboot_cnt);
        uboot_len -= uboot_cnt;
        uboot_offset += erase_unit_size;
    }
	if (memcmp(uboot_chip_crc_list, uboot_file_crc_list, uboot_block_count * 2) != 0) {
		printf("uboot write 64k addr err\n");
		return -1;
	}
	
	jl_erase(0 + flash_eoffset_size, abs_64K_len);
    
	for (int i = 1; i < abs_64K_len /erase_unit_size; i++) {
		jl_write(&uboot_file_buf[i * erase_unit_size], i * erase_unit_size + flash_eoffset_size, erase_unit_size);
	}
	jl_write(&uboot_file_buf[0], 0 + flash_eoffset_size, erase_unit_size);
	
    jl_crc_list(0 + flash_eoffset_size, abs_64K_len, uboot_chip_crc_list);
	if (memcmp(uboot_chip_crc_list, uboot_file_crc_list, uboot_block_count * 2) != 0) {
		printf("uboot write 0 addr err\n");
		return -1;
	}

	app_zone_addr = jl_device_init((char *)"app_dir_head");

#endif    

    fseek(fp, 0, SEEK_END);
    u32 file_size = ftell(fp)- app_zone_addr;
    u32 block_count = file_size / erase_unit_size + !!(file_size % erase_unit_size);
    file_size = block_count * erase_unit_size;
    
    u8* file_buf = new u8[file_size];

    memset(file_buf, 0xff, file_size);

    fseek(fp, app_zone_addr, SEEK_SET);
    fread(file_buf, 1, file_size, fp);
    
    u16* chip_crc_list = new u16[block_count];
    u16* file_crc_list = new u16[block_count];

    jl_crc_list(0 + flash_eoffset_size, file_size, chip_crc_list);
   
    u32 offset = 0;
    u32 len = file_size;
    while (len){
        u32 cnt = len > erase_unit_size ? erase_unit_size : len;
        file_crc_list[offset / erase_unit_size] = chip_crc16(&file_buf[offset], cnt);
        len -= cnt;
        offset += erase_unit_size;
    }

    u32 need_upgrade = 0;
    if (memcmp(chip_crc_list, file_crc_list, block_count * 2) != 0) {
        need_upgrade = 1;
        //skip head
        jl_erase(0 * erase_unit_size + flash_eoffset_size, erase_unit_size);
        for (int i = 1; i < block_count; i++) {
            if (chip_crc_list[i] != file_crc_list[i]) {
                printf("skip_diff @ 0x%x 0x%02x 0x%02x\n", i * erase_unit_size, chip_crc_list[i], file_crc_list[i]);
                jl_erase(i * erase_unit_size + flash_eoffset_size, erase_unit_size);
                jl_write(&file_buf[i * erase_unit_size], i * erase_unit_size + flash_eoffset_size, erase_unit_size);
            }
        }
    }

   
    if (need_upgrade) {   
        //fix head
         jl_write(&file_buf[0 * erase_unit_size], 0 * erase_unit_size + flash_eoffset_size, erase_unit_size);
         printf("diff @ 0x%x 0x%02x 0x%02x\n", 0, chip_crc_list[0], file_crc_list[0]);
        //verify
        jl_crc_list(0 + flash_eoffset_size, file_size, chip_crc_list);

  
        for (int i = 0; i < block_count; i++) {
            if (chip_crc_list[i] != file_crc_list[i]) {
                printf("data crc error @0x%x 0x%02x != 0x%02x\n", 
                    i* erase_unit_size, chip_crc_list[i], file_crc_list[i]);
				getchar();
                return -1;
            }
        }
    }

    jl_rboot();//升级成后发送rboot指令， 小机退出升级循环

    printf("succ");
	getchar();
    return 0;
}