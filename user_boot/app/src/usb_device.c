#include "usb/usb_stack.h"
#include "usb/usb_config.h"
#include "irq.h"
#include "gpio.h"
#include "printf.h"
#include "delay.h"
#include "usb/hid.h"
/* #include "usb/cdc.h" */

#define     TCFG_USB_SLAVE_HID_ENABLE       1

static void usb_device_init(const usb_dev usb_id)
{

    usb_config(usb_id);
    usb_g_sie_init(usb_id);
    usb_slave_init(usb_id);
    u8 *ep0_dma_buffer = usb_alloc_ep_dmabuffer(usb_id, 0, 64);

    usb_set_dma_raddr(usb_id, 0, ep0_dma_buffer);
    usb_set_dma_raddr(usb_id, 1, ep0_dma_buffer);
    usb_set_dma_raddr(usb_id, 2, ep0_dma_buffer);
    usb_set_dma_raddr(usb_id, 3, ep0_dma_buffer);
    usb_set_dma_raddr(usb_id, 4, ep0_dma_buffer);

    usb_write_intr_usbe(usb_id, INTRUSB_RESET_BABBLE | INTRUSB_SUSPEND);
    usb_clr_intr_txe(usb_id, -1);
    usb_clr_intr_rxe(usb_id, -1);
    usb_set_intr_txe(usb_id, 0);
    usb_set_intr_rxe(usb_id, 0);
    usb_g_isr_reg(usb_id, 3, 0);
    /* usb_sof_isr_reg(usb_id,3,0); */
    /* usb_sofie_enable(usb_id); */
    /* USB_DEBUG_PRINTF("ep0 addr %x %x\n", usb_get_dma_taddr(0), ep0_dma_buffer); */
}
static void usb_device_hold(const usb_dev usb_id)
{

    usb_g_hold(usb_id);
    usb_release(usb_id);
}


int usb_device_mode(const usb_dev usb_id, const u32 class)
{
    u8 class_index = 0;
    if (class == 0) {
        gpio_set_direction(IO_PORT_DM + 2 * usb_id, 1);
        gpio_set_pull_up(IO_PORT_DM + 2 * usb_id, 0);
        gpio_set_pull_down(IO_PORT_DM + 2 * usb_id, 0);
        gpio_set_die(IO_PORT_DM + 2 * usb_id, 0);

        gpio_set_direction(IO_PORT_DP + 2 * usb_id, 1);
        gpio_set_pull_up(IO_PORT_DP + 2 * usb_id, 0);
        gpio_set_pull_down(IO_PORT_DP + 2 * usb_id, 0);
        gpio_set_die(IO_PORT_DP + 2 * usb_id, 0);

        mdelay(15);

        gpio_set_die(IO_PORT_DM + 2 * usb_id, 1);
        gpio_set_die(IO_PORT_DP + 2 * usb_id, 1);

#if TCFG_USB_SLAVE_MSD_ENABLE
        msd_release(usb_id);
#endif

#if TCFG_USB_SLAVE_AUDIO_ENABLE
        uac_release(usb_id);
#endif

#if TCFG_USB_SLAVE_CDC_ENABLE
        cdc_release(usb_id);
#endif
#if TCFG_USB_SLAVE_HID_ENABLE
        hid_release(usb_id);
#endif
        usb_device_hold(usb_id);
        return 0;
    }

    usb_memory_init();

    usb_add_desc_config(usb_id, MAX_INTERFACE_NUM, NULL);


#if TCFG_USB_SLAVE_HID_ENABLE
    if ((class & HID_CLASS) == HID_CLASS) {
        log_info("add desc std hid");
        hid_register(usb_id);
        usb_add_desc_config(usb_id, class_index++, hid_desc_config);
    }
#endif

#if TCFG_USB_SLAVE_CDC_ENABLE
    if ((class & CDC_CLASS) == CDC_CLASS) {
        log_info("add desc cdc");
        usb_add_desc_config(usb_id, class_index++, cdc_desc_config);
        cdc_register(usb_id);
    }
#endif

    usb_device_init(usb_id);
    return 0;
}

