#include "user.h"
#include "printf.h"
#include "jlfs.h"
#include "usb/usb_stack.h"
#include "atomic.h"
#include "crc.h"
#include "dec.h"

#define LOG_TAG_CONST       UPGRADE
#define LOG_TAG             "[user]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#include "log.h"

/**********************************************************************************************************
多字节数据：低字节在前，高字节在后，即小端格式(little endian)。
CRC16 标准： CRC-CCITT (XModem)。
回复使用相同指令。

| Byte[0]         | Byte[1]         | Byte[3~2] | Byte[4] | Byte[5]    | Byte[6~n-2]       | Byte[n~n-1] |
| --------------- | --------------- | --------- | ------- | ---------- | ----------------- | ----------- |
| syncdata0(0xAA) | syncdata1(0x55) | cmd_len   | cmd     | rsp_status | param(部分指令无) | crc16       |

参数说明：
syncdata0:  固定为 0xAA
syncdata1:  固定为0x55
cmd_len:    cmd + rsp_status + union 数据的 长度
cmd:        操作指令
rsp_status: 回复状态
param:      对应操作指令的参数（部分指令无参数）
crc16:      整个命令包（除去crc自身）的crc16结果

demo加密部分为 cmd+rsp_status+param
**********************************************************************************************************/

JLFS_FILE upgrade_file;

#define SYNC_MARK0 0xAA
#define SYNC_MARK1 0x55

#define JL_SU_CMD_DEVICE_INIT   0xC0
#define JL_SU_CMD_DEVICE_CHECK  0xC1
#define JL_SU_CMD_ERASE         0xC2
#define JL_SU_CMD_WRITE         0xC3
#define JL_SU_CMD_FLASH_CRC     0xC4
/* #define JL_SU_CMD_EX_KEY        0xC5 */

#define JL_SU_CMD_REBOOT        0xCA

#define JL_ERASE_TYPE_PAGE   1
#define JL_ERASE_TYPE_SECTOR 2
#define JL_ERASE_TYPE_BLOCK  3

enum {
    JL_SU_CMD_SUSS,
    JL_SU_CMD_SDK_ID_ERROR,
    JL_SU_CMD_WRITE_ERROR,
    JL_SU_CMD_ERASE_ERROR,
    JL_SU_CMD_OTHER_ERROR,
};

//上位机结构体
typedef struct {
    u8 syncdata0;
    u8 syncdata1;
    u16 cmd_len;
    u8 cmd;
    u8 rsp_status;
    union {
        struct {
            u32 sdk_id;
        } device_check;

        struct {
            u32 address;		// 烧写FLASH地址
            u32 data_length;    //烧写长度
            u8  data[0];		// 烧写数据
        } write;

        struct {
            u32 address;    //需要对齐
            u32 type;
        } erase;

        struct {
            u32 address;
            u32 len;
            u32 block_size;
        } crc_list;

        struct {
            u8 file_name[16];
            u8 mode;
        }  init;
    };
} __attribute__((packed)) JL_SECTOR_COMMAND_ITEM;

//设备结构体
typedef struct {
    u8 syncdata0;
    u8 syncdata1;
    u16 cmd_len;
    u8 cmd;
    u8 rsp_status;
    union {
        struct {
            u16 crc[0];
        } crc_list;

        struct {
            u32 upgrade_addr;
            u32 upgrade_len;
            u32 upgrade_eoffset;
            u32 flash_alignsize;
        }  init;

        struct {
            u8 vid[4];
            u8 pid[16];
            u32 sdk_id;
        } device_check;
    };
} __attribute__((packed)) JL_SECTOR_COMMAND_DEV_ITEM;

extern void chip_reset(void);
extern u32 get_uboot_offset(void);
extern void set_uboot_offset(u32 offset);

extern void *get_global_buffer(u32 size);
extern u32 usb_g_intr_read(const usb_dev usb_id, u32 ep, u8 *ptr, u32 len, u32  block);
extern u32 hid_tx_data(struct usb_device_t *usb_device, const u8 *buffer, u32 len);

extern u8 *get_ut_up_buf_p(void);
extern u32 get_ut_up_rxlen(void);
extern void ut_up_tx_data(u8 *buf, u32 len);
extern void ut_device_mode(const char *ut_tx, const char *ut_rx, u32 baud);
extern void ut_devic_mode_close(void);
extern u16 jlfs_get_app_dir_head_datacrc(void);
extern u32 jlfs_get_flash_eoffset_size(void);

/* static u16 communication_key; */

JL_SECTOR_COMMAND_ITEM *g_ops;
JL_SECTOR_COMMAND_DEV_ITEM *dev_g_ops;
u8 dev_g_buf[64] __attribute__((aligned(4))); //这个buf限制64是为了兼容usb

volatile int new_data;
static u32 flash_alignsize;

__attribute__((weak))
void usb_disable_for_ota()
{

}

void set_new_data_flag(int flag)
{
    new_data = flag;
}

//10进制，注意要与上位机一致，不然无法升级
u32 communication_key = 12345678;
//加解密
void enc_dec_data(u8 *buf, u32 len)
{
    u32 keyIdx = 0;
    u8 *encKey = (u8 *)&communication_key;

    for (u32 i = 0; i < len; ++i, ++keyIdx) {
        buf[i] ^= encKey[keyIdx & 0x3];
    }
}

u8 *enc_cmd_data(u8 *buf, u32 len)
{
    u32 i;
    u16 data_crc = 0;
    u16 check_crc = 0;
    u16 rx_cnt = 0;
    u16 cmd_len = 0;

    if (len < 2 + 2 + 2 + 2) {
        log_info("data len not enough\n");
        return NULL;
    }

    log_info("+++++decode_len:%d\n", len);
    /* put_buf(buf, len); */

    for (i = 0; i < len - 7; i++) {
        if (buf[i] == SYNC_MARK0 && buf[i + 1] == SYNC_MARK1) { //检测头
            cmd_len = *((u16 *)(buf + i + 2)); //获取cmd的长度
            if (i + 2 + 2 + cmd_len + 2 > len) { //数据包长度
                log_info("cmd len too long\n");
                return NULL;
            }

            data_crc |= *(buf + i + 2 + 2 + cmd_len);
            data_crc |= (*(buf + i + 2 + 2 + cmd_len + 1)) << 8; //获取传下来的crc
            check_crc = chip_crc16(&buf[i], 2 + 2 + cmd_len); //本地校验crc
            if (data_crc == check_crc) {
                /* printf("enc cmd ok\n"); */
                //对数据体解码
                enc_dec_data(buf + i + 4, cmd_len);

                return &(buf[i]);  //正常确则返回指令包地址
            } else {
                log_info("cmd crc err,crc:0x%x,check:0x%x\n", data_crc, check_crc);
                return NULL;
            }
        }
    }
    log_info("cnt not find mark\n");
    return NULL;
}

static u32 recv_data_buf[64 / 4];
void *recv_new_data()
{
    void *data_p = NULL;
    local_irq_disable();
    if (new_data) {
        new_data = 0;
#if (USB_MODE==1)
        data_p = (void *)recv_data_buf;
        u32 rx_len = usb_g_intr_read(0, HID_EP_OUT, (u8 *)data_p, 64, 0);
        data_p =  enc_cmd_data((u8 *)data_p, rx_len);
#else
        u32 rx_len = get_ut_up_rxlen();
        data_p =  enc_cmd_data((u8 *)get_ut_up_buf_p(), rx_len);
#endif
        /* log_info_hexdump((u8 *)data_p, rx_len); */
    }
    local_irq_enable();
    return data_p;
}

void upgrade_rsp(void *p, u32 len)
{
    u8 tx[64];
    memcpy(tx, p, len);
    log_info("+++++ACK_len:0x%x\n", len);
    log_info_hexdump((u8 *)tx, len);
#if (USB_MODE==1)
    hid_tx_data(NULL, tx, sizeof(tx));
#else
    ut_up_tx_data(tx, len);
#endif
}

//设置余下协议部分及回复
void set_remain_and_upgrade_rsp(u8 cmd)
{
    u16 crc16_len;
    u16 crc16_da;
    dev_g_ops->syncdata0 = 0xAA;
    dev_g_ops->syncdata1 = 0x55;          //设置标志
    crc16_len = 1 + 1 + 2 + dev_g_ops->cmd_len; //数据长度
    dev_g_ops->cmd = cmd;

    //对数据体加密
    enc_dec_data(((u8 *)dev_g_ops) + 4, dev_g_ops->cmd_len);

    crc16_da = chip_crc16(dev_g_ops, crc16_len);
    *((u8 *)dev_g_ops + crc16_len) = crc16_da & 0xff;
    *((u8 *)dev_g_ops + crc16_len + 1) = crc16_da >> 8;

    upgrade_rsp(dev_g_ops, crc16_len + 2); //总长度为数据长度+crc16长度
}


void jl_su_device_init()
{
    u16 crc16_len = 0;
    u16 crc16 = 0;
    u8 err = jlfs_fopen_by_name(&upgrade_file, (const char *)g_ops->init.file_name, g_ops->init.mode);
    if (err) {
        log_info("file open error !\n");
        memset(&dev_g_ops->init, 0, sizeof(dev_g_ops->init));
        dev_g_ops->rsp_status = JL_SU_CMD_OTHER_ERROR;
    } else {
        log_info("%s() 0x%x %d\n", __func__, upgrade_file.addr, upgrade_file.size);
        dev_g_ops->init.upgrade_addr = upgrade_file.addr;
        dev_g_ops->init.upgrade_len = upgrade_file.size;
        dev_g_ops->init.upgrade_eoffset = jlfs_get_flash_eoffset_size();
        dev_g_ops->init.flash_alignsize = jlfs_get_flash_align_size();

        dev_g_ops->rsp_status = JL_SU_CMD_SUSS;
    }
    dev_g_ops->cmd_len = 1 + 1 + 4 + 4 + 4 + 4; //根据协议计算长度,不能用sizeof
    /* printf("cmd LEN:%d\n",dev_g_ops->cmd_len); */

    set_remain_and_upgrade_rsp(JL_SU_CMD_DEVICE_INIT);
}

void jl_su_flash_crc()
{
    u32 block_count = g_ops->crc_list.len / g_ops->crc_list.block_size;
    u32 l = flash_alignsize;
    u8 *p = get_global_buffer(l); //4K
    log_info("%s() address:%x len:%d block_size:%d block_count:%d\n",
             __func__, g_ops->crc_list.address, g_ops->crc_list.len,
             g_ops->crc_list.block_size, block_count);

    jlfs_fseek(&upgrade_file, g_ops->crc_list.address);
    for (int i = 0; i < block_count; i++) {
        jlfs_fread(&upgrade_file, p, l);
        dev_g_ops->crc_list.crc[i] = chip_crc16(p, l);
        log_info("%x ", dev_g_ops->crc_list.crc[i]);
    }
    log_info("\n");
    dev_g_ops->rsp_status = JL_SU_CMD_SUSS;
    dev_g_ops->cmd_len = 1 + 1 + block_count * 2;

    set_remain_and_upgrade_rsp(JL_SU_CMD_FLASH_CRC);
}
void jl_su_erase()
{
    /* log_info("%s() address %x %d\n", __func__, g_ops->erase.address, g_ops->erase.type); */

    jlfs_fseek(&upgrade_file, g_ops->erase.address);

    u32 len = 0;

    switch (g_ops->erase.type) {
    case JL_ERASE_TYPE_PAGE:
        len = 256;
        break;
    case JL_ERASE_TYPE_SECTOR:
        len = 4096;
        break;
    case JL_ERASE_TYPE_BLOCK:
        len = 65536;
        break;
    default:
        break;
    }

    u32 r = jlfs_erase(&upgrade_file, len);

    if (r == 0) {
        dev_g_ops->rsp_status = JL_SU_CMD_ERASE_ERROR;
    } else {
        dev_g_ops->rsp_status = JL_SU_CMD_SUSS;
    }
    dev_g_ops->cmd_len = 1 + 1;

    set_remain_and_upgrade_rsp(JL_SU_CMD_ERASE);
}


void jl_su_write_flash()
{
    jlfs_fseek(&upgrade_file, g_ops->write.address);
    u32 r = jlfs_write(&upgrade_file, g_ops->write.data, g_ops->write.data_length);
    if (r != g_ops->write.data_length) {
        dev_g_ops->rsp_status = JL_SU_CMD_WRITE_ERROR;
    } else {
        dev_g_ops->rsp_status = JL_SU_CMD_SUSS;
    }
    dev_g_ops->cmd_len = 1 + 1;
    set_remain_and_upgrade_rsp(JL_SU_CMD_WRITE);
}
void jl_su_devcie_check()
{
    if (g_ops->device_check.sdk_id == 0x12345678) { //用户自定义，可以用于区分产品类型，避免烧写错误固件
        jlfs_get_pid_vid(dev_g_ops->device_check.pid, dev_g_ops->device_check.vid);
        dev_g_ops->rsp_status = JL_SU_CMD_SUSS;
    } else {
        dev_g_ops->rsp_status = JL_SU_CMD_SDK_ID_ERROR;
    }

    dev_g_ops->device_check.sdk_id = 0x12345678; //设备id
    dev_g_ops->cmd_len = 1 + 1 + 4 + 16 + 4;
    set_remain_and_upgrade_rsp(JL_SU_CMD_DEVICE_CHECK);
}

void upgrade_loop()
{
    dev_g_ops = (JL_SECTOR_COMMAND_DEV_ITEM *)dev_g_buf;//回复buf 64大小是后面需要兼容usb

    while (1) {

        g_ops = (JL_SECTOR_COMMAND_ITEM *)recv_new_data();

        if (g_ops == NULL) {
            wdt_clr();
            continue;
        }

        switch (g_ops->cmd) {
        case JL_SU_CMD_DEVICE_INIT:
            log_info("JL_SU_CMD_DEVICE_INIT\n");
            jl_su_device_init();
            break;
        case JL_SU_CMD_FLASH_CRC:
            log_info("JL_SU_CMD_FLASH_CRC\n");
            jl_su_flash_crc();
            break;
        case JL_SU_CMD_WRITE:
            log_info("JL_SU_CMD_WRITE:\n");
            jl_su_write_flash();
            break;
        case JL_SU_CMD_ERASE:
            log_info("JL_SU_CMD_ERASE\n");
            jl_su_erase();
            break;
        case JL_SU_CMD_DEVICE_CHECK:
            log_info("JL_SU_CMD_DEVICE_CHECK\n");
            jl_su_devcie_check();
            break;
        case JL_SU_CMD_REBOOT:
            log_info("JL_SU_CMD_REBOOT\n");
            ut_devic_mode_close();
            usb_disable_for_ota();
#if(USE_UPGRADE_MAGIC)
            uboot_set_uart_upgrade_succ();
#endif
            /* chip_reset(); */ //可以复位，也可以直接跑去app
            return;
        }
        g_ops = NULL;
    }
}

u8 user_check_upgrade(u32 jlfs_err)
{
    flash_alignsize = jlfs_get_flash_align_size();
    u32 flash_app_base_addr = jlfs_get_flash_app_base_addr();
    u32 flash_offset_size = jlfs_get_flash_eoffset_size();

    log_info("flash_alignsize = %d\n", flash_alignsize);
    log_info("flash_app_start_addr = 0x%x\n", flash_app_base_addr);
    log_info("flash_offset_size = 0x%x\n", flash_offset_size);

    u32 flash_uboot_offset = get_uboot_offset();
    log_info("flash_uboot_offset = 0x%x\n", flash_uboot_offset);
    if (flash_uboot_offset == 1024 * 64) {
        log_info("cpoy uboot\n");
        JLFS_FILE app_head = {
            .addr = flash_app_base_addr,
            .size = flash_alignsize,
            .pos  = 0,
        };
        JLFS_FILE file_paste = {
            .addr = flash_offset_size,
            .size = flash_app_base_addr,
        };
        JLFS_FILE file_copy = {
            .addr = flash_uboot_offset,
            .size = flash_app_base_addr,
        };
        jlfs_erase(&app_head, app_head.size);

        set_uboot_offset(flash_offset_size);

        jlfs_fseek(&file_paste, 0);
        jlfs_erase(&file_paste, file_paste.size);
        u32 len;
        u32 l = flash_alignsize;
        u8 *p = get_global_buffer(l);
        jlfs_fseek(&file_copy, 0);
        jlfs_fseek(&file_paste, 0);
        do {
            len = jlfs_fread(&file_copy, p, l);
            if (!len) {
                break;
            }
            jlfs_write(&file_paste, p, len);
        } while (1);
    }

    u8 err = jlfs_fopen_by_name(&upgrade_file, "app_dir_head", 0);
    if (err) {
        log_info("file open error !\n");
        return 1;
    }
    log_info("%s() 0x%x 0x%x\n", __func__, upgrade_file.addr, upgrade_file.size);

#if (USB_MODE==1)
    usb_device_mode(0, HID_CLASS);
#else
    ut_device_mode("PA01", "PA02", 1000000);
#endif

    upgrade_loop();
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//通过检测标志位进入升级，可以由app层跳转过来
//详细见文档说明
extern u32 nvram_list[];
#define NV_RAM_LIST_ADDR  (nvram_list)
//数据要与升级前匹配 start ok
static const u8 uboot_uart_upgrade_mode_magic[8] = {
    'u', 'b', 'o', 'o', 't', 0x5a, 's', 't',
};

static const u8 uboot_uart_upgrade_succ_magic[8] = {
    'u', 'b', 'o', 'o', 't', 0xa5, 'o', 'k',
};


u8 uboot_check_upgrade_magic()
{
    if (memcmp((char *)NV_RAM_LIST_ADDR, uboot_uart_upgrade_mode_magic, sizeof(uboot_uart_upgrade_mode_magic)) == 0) {
        memset((char *)NV_RAM_LIST_ADDR, 0, sizeof(uboot_uart_upgrade_mode_magic));
        log_info("\n****************** check_upgrade flag ok  *****************\n\n");
        return 1;
    }
    return 0;
}

void uboot_set_uart_upgrade_succ()
{
    memcpy((char *)NV_RAM_LIST_ADDR, uboot_uart_upgrade_succ_magic, sizeof(uboot_uart_upgrade_succ_magic));
}
