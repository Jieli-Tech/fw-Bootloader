#include "user.h"
#include "printf.h"
#include "jlfs.h"
#include "usb/usb_stack.h"
#include "atomic.h"
#include "crc.h"
#include "dec.h"


JLFS_FILE upgrade_file;

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
            u16 crc;
        } crc;

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
        } init;

        struct {
            u8 vid[4];
            u8 pid[16];
            u32 sdk_id;
        } device_check;
    };

} JL_SECTOR_COMMAND_ITEM;


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

static u16 communication_key;

JL_SECTOR_COMMAND_ITEM *g_ops;

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

void enc_data(u8 *p, u32 l)
{
    if (communication_key) {
        for (int i = 0; i < l; i += 2) {
            p[i] ^= communication_key;
            p[i + 1] ^= communication_key >> 8;
        }
    }
}

static u32 recv_data_buf[64 / 4];
void *recv_new_data()
{
    void *data_p = NULL;
    OS_ENTER_CRITICAL();
    if (new_data) {
        new_data = 0;
#if (__HID_UART_MODE_SEL==0)
        data_p = (void *)recv_data_buf;
        u32 rx_len = usb_g_intr_read(0, HID_EP_OUT, (u8 *)data_p, 64, 0);
        enc_data((u8 *)data_p, 64);
#else
        data_p = (void *)get_ut_up_buf_p();
        u32 rx_len = get_ut_up_rxlen();
        enc_data((u8 *)data_p, rx_len);
#endif
        /* log_info_hexdump((u8 *)data_p, rx_len); */
    }
    OS_EXIT_CRITICAL();
    return data_p;
}

void upgrade_rsp(void *p, u32 len)
{
    u8 tx[64];
    memcpy(tx, p, len);
    enc_data(tx, 64);
    /* log_info("ACK:\n"); */
    /* log_info_hexdump(tx, 64); */
#if (__HID_UART_MODE_SEL==0)
    hid_tx_data(NULL, tx, sizeof(tx));
#else
    ut_up_tx_data(tx, len);
#endif
}

void jl_su_ex_key()
{
    u16 key = chip_crc16_with_init(g_ops->ex_key.data, g_ops->ex_key.data_length, g_ops->rand) & 0xf0f0;

    log_info("key %x\n", key);
    for (int i = 0; i < g_ops->ex_key.data_length; i++) {
        g_ops->ex_key.data[i] = get_random();
    }

    g_ops->rand = get_random();

    key |= chip_crc16_with_init(g_ops->ex_key.data, g_ops->ex_key.data_length, g_ops->rand) & 0x0f0f;

    log_info("key %x\n", key);
    upgrade_rsp(g_ops, 64);
    communication_key = key;
    log_info("%s() %x\n", __func__, communication_key);
}
void jl_su_device_init()
{
    u8 err = jlfs_fopen_by_name(&upgrade_file, (const char *)g_ops->init.file_name, g_ops->init.mode);
    if (err) {
        log_info("file open error !\n");
        memset(&g_ops->init, 0, sizeof(g_ops->init));
    } else {
        log_info("%s() 0x%x %d\n", __func__, upgrade_file.addr, upgrade_file.size);
        g_ops->init.upgrade_addr = upgrade_file.addr;
        g_ops->init.upgrade_len = upgrade_file.size;
        g_ops->init.upgrade_eoffset = jlfs_get_flash_eoffset_size();
        g_ops->init.flash_alignsize = jlfs_get_flash_align_size();
    }
    upgrade_rsp(g_ops, sizeof(*g_ops));
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
        g_ops->crc_list.crc[i] = chip_crc16(p, l);
        log_info("%x ", g_ops->crc_list.crc[i]);
    }
    log_info("\n");
    upgrade_rsp(g_ops, 16 + block_count * 2);
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
    g_ops->erase.type = r;
    upgrade_rsp(g_ops, sizeof(*g_ops));
}

enum {
    JL_SU_CMD_SUSS,
    JL_SU_CMD_CRC_ERROR,
    JL_SU_CMD_SDK_ID_ERROR,
};

void jl_su_write_flash()
{
    /* log_info("%s() address %x %d\n", __func__, g_ops->write.address, g_ops->write.data_length); */
    u16 crc = chip_crc16(g_ops->write.data, g_ops->write.data_length);
    if (crc != g_ops->write.data_crc) {
        g_ops->rsp_status = JL_SU_CMD_CRC_ERROR;
    } else {
        jlfs_fseek(&upgrade_file, g_ops->write.address);
        u32 r = jlfs_write(&upgrade_file, g_ops->write.data, g_ops->write.data_length);
        if (r != g_ops->write.data_length) {
            g_ops->rsp_status = JL_SU_CMD_CRC_ERROR;
        } else {
            g_ops->rsp_status = JL_SU_CMD_SUSS;
        }
    }
    upgrade_rsp(g_ops, sizeof(*g_ops));
}
void jl_su_devcie_check()
{
    g_ops->rsp_status = JL_SU_CMD_SUSS;
    if (g_ops->device_check.sdk_id == 0x12345678) { //用户自定义，可以用于区分产品类型，避免烧写错误固件
        jlfs_get_pid_vid(g_ops->device_check.pid, g_ops->device_check.vid);
    } else {
        g_ops->rsp_status = JL_SU_CMD_SDK_ID_ERROR;
    }

    upgrade_rsp(g_ops, sizeof(*g_ops));
}

void upgrade_loop()
{
    communication_key = 0;

    while (1) {

        g_ops = (JL_SECTOR_COMMAND_ITEM *)recv_new_data();

        if (g_ops == NULL) {
            wdt_clr();
            continue;
        }

        switch (g_ops->cmd) {
        case JL_SU_CMD_EX_KEY:
            jl_su_ex_key();
            break;
        case JL_SU_CMD_DEVICE_INIT:
            jl_su_device_init();
            break;
        case JL_SU_CMD_FLASH_CRC:
            jl_su_flash_crc();
            break;
        case JL_SU_CMD_WRITE:
            jl_su_write_flash();
            break;
        case JL_SU_CMD_ERASE:
            jl_su_erase();
            break;
        case JL_SU_CMD_DEVICE_CHECK:
            jl_su_devcie_check();
            break;
        case JL_SU_CMD_REBOOT:
            log_info("reboot\n");
            ut_devic_mode_close();
            usb_disable_for_ota();

            /* chip_reset(); */
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

#if (__HID_UART_MODE_SEL==0)
    usb_device_mode(0, HID_CLASS);
#else
    ut_device_mode("PA01", "PA02", 1000000);
#endif

    upgrade_loop();
    return 0;
}
