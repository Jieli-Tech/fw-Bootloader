#include "common.h"
#include "irq.h"
#include "printf.h"
#include "usb/usb_stack.h"
#include "usb/hid.h"
#include "usb/usb_config.h"

#define LOG_TAG_CONST       UPGRADE
#define LOG_TAG             "[hid]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
#include "log.h"

static const u8 sHIDDescriptor[] = {
//HID
    //InterfaceDeszcriptor:
    USB_DT_INTERFACE_SIZE,     // Length
    USB_DT_INTERFACE,          // DescriptorType
    /* 0x04,                      // bInterface number */
    0x00,                       // bInterface number
    0x00,                      // AlternateSetting
    0x02,                        // NumEndpoint
    USB_CLASS_HID,             // Class = Human Interface Device
    0x00,                      // Subclass, 0 No subclass, 1 Boot Interface subclass
    0x00,                      // Procotol, 0 None, 1 Keyboard, 2 Mouse
    0x00,                      // Interface Name

    //HIDDescriptor:
    0x09,                      // bLength
    USB_HID_DT_HID,            // bDescriptorType, HID Descriptor
    0x00, 0x01,                // bcdHID, HID Class Specification release NO.
    0x00,                      // bCuntryCode, Country localization (=none)
    0x01,                       // bNumDescriptors, Number of descriptors to follow
    0x22,                       // bDescriptorType, Report Desc. 0x22, Physical Desc. 0x23
    0,//LOW(ReportLength)
    0, //HIGH(ReportLength)

    USB_DT_ENDPOINT_SIZE,       // bLength
    USB_DT_ENDPOINT,            // bDescriptorType, Type
    USB_DIR_IN | HID_EP_IN,     // bEndpointAddress
    USB_ENDPOINT_XFER_INT,      // Interrupt
    LOBYTE(MAXP_SIZE_HIDIN), HIBYTE(MAXP_SIZE_HIDIN),// Maximum packet size
    1,     // Poll every 10msec seconds

    USB_DT_ENDPOINT_SIZE,       // bLength
    USB_DT_ENDPOINT,            // bDescriptorType, Type
    USB_DIR_OUT | HID_EP_OUT,   // bEndpointAddress
    USB_ENDPOINT_XFER_INT,      // Interrupt
    LOBYTE(MAXP_SIZE_HIDOUT), HIBYTE(MAXP_SIZE_HIDOUT),// Maximum packet size
    0x01,                       // bInterval, for high speed 2^(n-1) * 125us, for full/low speed n * 1ms
};

static const u8 sHIDReportDesc[] = {
    USAGE_PAGE(2, 0x00, 0xff),      // vendor-defined 1
    USAGE(1, 0x01),                 // vendor-value 1
    COLLECTION(1, APPLICATION),     // application
    USAGE(1, 0x02),
    LOGICAL_MIN(2, 0, 0),
    LOGICAL_MAX(2, 0xff, 0),
    REPORT_SIZE(1, 0x08),          // 1 bit
    REPORT_COUNT(1, MAXP_SIZE_HIDIN),         // eight 1bit data
    INPUT(1, 0x02),             // data, variabie, absolute
    USAGE(1, 0x03),
    LOGICAL_MIN(2, 0, 0),
    LOGICAL_MAX(2, 0xff, 0),
    REPORT_SIZE(1, 0x08),
    REPORT_COUNT(1, MAXP_SIZE_HIDOUT),
    OUTPUT(1, 0x02),
    END_COLLECTION,

};

static u32 get_hid_report_desc_len(u32 index)
{
    u32 len = 0;
    len = sizeof(sHIDReportDesc);
    return len;
}
static void *get_hid_report_desc(u32 index)
{
    u8 *ptr  = NULL;
    ptr = (u8 *)sHIDReportDesc;
    return ptr;
}


static u8 *hid_ep_in_dma;
static u8 *hid_ep_out_dma;
static struct usb_device_t *hid_device;

u32 hid_tx_data(struct usb_device_t *usb_device, const u8 *buffer, u32 len)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    return usb_g_intr_write(usb_id, HID_EP_IN, buffer, len);
}

extern void set_new_data_flag(int flag);
static void hid_rx_data(struct usb_device_t *usb_device, u32 ep)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    set_new_data_flag(1);
    /* log_info_hexdump(hid_ep_out_dma,64); */
#if 0
    u32 rx_len = usb_g_intr_read(usb_id, ep, hid_ep_in_dma, 64, 0);
    hid_ep_in_dma[0] = JL_RAND->R64L;
    hid_tx_data(usb_device, NULL, rx_len);
    log_info("%s() %d", __func__, rx_len);
    log_info_hexdump(hid_ep_out_dma, rx_len);
#endif

}

static void hid_endpoint_init(struct usb_device_t *usb_device, u32 itf)
{
    hid_device = usb_device;
    const usb_dev usb_id = usb_device2id(usb_device);
    usb_g_ep_config(usb_id, HID_EP_IN | USB_DIR_IN, USB_ENDPOINT_XFER_INT, 0, hid_ep_in_dma, MAXP_SIZE_HIDIN);
    usb_enable_ep(usb_id, HID_EP_IN);

    usb_g_set_intr_hander(usb_id, HID_EP_OUT, hid_rx_data);
    usb_g_ep_config(usb_id, HID_EP_OUT, USB_ENDPOINT_XFER_INT, 1, hid_ep_out_dma, MAXP_SIZE_HIDOUT);
}
u32 hid_register(const usb_dev usb_id)
{
    hid_ep_in_dma = usb_alloc_ep_dmabuffer(usb_id, HID_EP_IN | USB_DIR_IN, MAXP_SIZE_HIDIN);

    hid_ep_out_dma = usb_alloc_ep_dmabuffer(usb_id, HID_EP_OUT, MAXP_SIZE_HIDOUT);
    return 0;
}

void hid_release(const usb_dev usb_id)
{
    return ;
}

static void hid_reset(struct usb_device_t *usb_device, u32 itf)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    log_error("%s", __func__);
#if USB_ROOT2
    usb_disable_ep(usb_id, HID_EP_IN);
#else
    hid_endpoint_init(usb_device, itf);
#endif
}
static u32 hid_recv_output_report(struct usb_device_t *usb_device, struct usb_ctrlrequest *setup)
{
    const usb_dev usb_id = usb_device2id(usb_device);
    u32 ret = 0;
    u8 read_ep[8];
    u8 mute;
    u16 volume = 0;
    usb_read_ep0(usb_id, read_ep, MIN(sizeof(read_ep), setup->wLength));
    ret = USB_EP0_STAGE_SETUP;
    put_buf(read_ep, 8);


    return ret;
}

static u32 hid_itf_hander(struct usb_device_t *usb_device, struct usb_ctrlrequest *req)
{
    if ((int)req == -1) {
        return 0;
    }
    const usb_dev usb_id = usb_device2id(usb_device);
    u32 tx_len;
    u8 *tx_payload = usb_get_setup_buffer(usb_device);
    u32 bRequestType = req->bRequestType & USB_TYPE_MASK;
    switch (bRequestType) {
    case USB_TYPE_STANDARD:
        switch (req->bRequest) {
        case USB_REQ_GET_DESCRIPTOR:
            switch (HIBYTE(req->wValue)) {
            case USB_HID_DT_HID:
                tx_payload = (u8 *)sHIDDescriptor + USB_DT_INTERFACE_SIZE;
                tx_len = 9;
                tx_payload = usb_set_data_payload(usb_device, req, tx_payload, tx_len);
                tx_payload[7] = LOBYTE(get_hid_report_desc_len(req->wIndex));
                tx_payload[8] = HIBYTE(get_hid_report_desc_len(req->wIndex));
                break;
            case USB_HID_DT_REPORT:
                hid_endpoint_init(usb_device, req->wIndex);
                tx_len = get_hid_report_desc_len(req->wIndex);
                tx_payload = get_hid_report_desc(req->wIndex);
                usb_set_data_payload(usb_device, req, tx_payload, tx_len);
                break;
            }// USB_REQ_GET_DESCRIPTOR
            break;
        case USB_REQ_SET_DESCRIPTOR:
            usb_set_setup_phase(usb_device, USB_EP0_STAGE_SETUP);
            break;
        case USB_REQ_SET_INTERFACE:
            if (usb_device->bDeviceStates == USB_DEFAULT) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_ADDRESS) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_CONFIGURED) {
                //只有一个interface 没有Alternate
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            }
            break;
        case USB_REQ_GET_INTERFACE:
            if (req->wLength) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_DEFAULT) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_ADDRESS) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_CONFIGURED) {
                tx_len = 1;
                tx_payload[0] = 0x00;
                usb_set_data_payload(usb_device, req, tx_payload, tx_len);
            }
            break;
        case USB_REQ_GET_STATUS:
            if (usb_device->bDeviceStates == USB_DEFAULT) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else if (usb_device->bDeviceStates == USB_ADDRESS) {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            } else {
                usb_set_setup_phase(usb_device, USB_EP0_SET_STALL);
            }
            break;
        }//bRequest @ USB_TYPE_STANDARD
        break;

    case USB_TYPE_CLASS: {
        switch (req->bRequest) {
        case USB_REQ_SET_IDLE:
            usb_set_setup_phase(usb_device, USB_EP0_STAGE_SETUP);
            break;
        case USB_REQ_GET_IDLE:
            tx_len = 1;
            tx_payload[0] = 0;
            usb_set_data_payload(usb_device, req, tx_payload, tx_len);
            break;
        case USB_REQ_SET_REPORT:
            usb_set_setup_recv(usb_device, hid_recv_output_report);
            break;
        }//bRequest @ USB_TYPE_CLASS
    }
    break;
    }
    return 0;
}

u32 hid_desc_config(const usb_dev usb_id, u8 *ptr, u32 *cur_itf_num)
{
    log_error("hid interface num:%d\n", *cur_itf_num);
    u8 *_ptr = ptr;
    memcpy(ptr, sHIDDescriptor, sizeof(sHIDDescriptor));
    ptr[2] = *cur_itf_num;
    if (usb_set_interface_hander(usb_id, *cur_itf_num, hid_itf_hander) != *cur_itf_num) {
        ASSERT(0, "hid set interface_hander fail");
    }
    if (usb_set_reset_hander(usb_id, *cur_itf_num, hid_reset) != *cur_itf_num) {
        ASSERT(0, "hid set interface_reset_hander fail");
    }

    ptr[USB_DT_INTERFACE_SIZE + 7] = LOBYTE(get_hid_report_desc_len(0));
    ptr[USB_DT_INTERFACE_SIZE + 8] = HIBYTE(get_hid_report_desc_len(0));
    *cur_itf_num = *cur_itf_num + 1;
    return sizeof(sHIDDescriptor) ;
}
