#ifndef  __HID_H__
#define  __HID_H__

//do not add brace to the macro outside
#define SHORT_ITEMS(prefix, _len, ...) \
    ((prefix) | (((_len) > 0) ? 1 << ((_len) - 1) : 0)), ##__VA_ARGS__

/*Main Items*/
#define INPUT(len, ...)              SHORT_ITEMS(0x80, len, ##__VA_ARGS__)
#define OUTPUT(len, ...)             SHORT_ITEMS(0x90, len, ##__VA_ARGS__)
#define COLLECTION(len, ...)         SHORT_ITEMS(0xA0, len, ##__VA_ARGS__)
#define FEATURE(len, ...)            SHORT_ITEMS(0xB0, len, ##__VA_ARGS__)
#define END_COLLECTION               0xC0

/*Golbal Items*/
#define USAGE_PAGE(len, ...)         SHORT_ITEMS(0x04, len, ##__VA_ARGS__)
#define LOGICAL_MIN(len, ...)        SHORT_ITEMS(0x14, len, ##__VA_ARGS__)
#define LOGICAL_MAX(len, ...)        SHORT_ITEMS(0x24, len, ##__VA_ARGS__)
#define PHYSICAL_MIN(len, ...)       SHORT_ITEMS(0x34, len, ##__VA_ARGS__)
#define PHYSICAL_MAX(len, ...)       SHORT_ITEMS(0x44, len, ##__VA_ARGS__)
#define UNIT_EXPONENT(len, ...)      SHORT_ITEMS(0x54, len, ##__VA_ARGS__)
#define UNIT(len, ...)               SHORT_ITEMS(0x64, len, ##__VA_ARGS__)
#define REPORT_SIZE(len, ...)        SHORT_ITEMS(0x74, len, ##__VA_ARGS__)
#define REPORT_ID(len, ...)          SHORT_ITEMS(0x84, len, ##__VA_ARGS__)
#define REPORT_COUNT(len, ...)       SHORT_ITEMS(0x94, len, ##__VA_ARGS__)
#define PUSH                         SHORT_ITEMS(0xA4, 0)
#define POP                          SHORT_ITEMS(0xB4, 0)

/*Local Items*/
#define USAGE(len, ...)              SHORT_ITEMS(0x08, len, ##__VA_ARGS__)
#define USAGE_MIN(len, ...)          SHORT_ITEMS(0x18, len, ##__VA_ARGS__)
#define USAGE_MAX(len, ...)          SHORT_ITEMS(0x28, len, ##__VA_ARGS__)
#define DESIGNATOR_INDEX(len, ...)   SHORT_ITEMS(0x38, len, ##__VA_ARGS__)
#define DESIGNATOR_MIN(len, ...)     SHORT_ITEMS(0x48, len, ##__VA_ARGS__)
#define DESIGNATOR_MAX(len, ...)     SHORT_ITEMS(0x58, len, ##__VA_ARGS__)
#define STRING_INDEX(len, ...)       SHORT_ITEMS(0x78, len, ##__VA_ARGS__)
#define STRING_MIN(len, ...)         SHORT_ITEMS(0x88, len, ##__VA_ARGS__)
#define STRING_MAX(len, ...)         SHORT_ITEMS(0x98, len, ##__VA_ARGS__)
#define DELIMITER(len, ...)          SHORT_ITEMS(0xA8, len, ##__VA_ARGS__)


/*Consumer Page*/
#define CONSUMER_PAGE           0x0C
#define CONSUMER_CONTROL        0x01
#define GENERIC_DESKTOP_CTRLS   0x01

/*Usage*/
#define POINTER                 0x01
#define MOUSE                   0x02
#define BUTTON                  0x09
#define X_AXIS                  0x30
#define Y_AXIS                  0x31

//Collection
#define PHYSICAL                0x00
#define APPLICATION             0x01
#define LOGICAL                 0x02
#define REPORT                  0x03

#define USB_HID_DT_HID   (USB_TYPE_CLASS | 0x01)
#define USB_HID_DT_REPORT    (USB_TYPE_CLASS | 0x02)
#define USB_HID_DT_PHYSICAL  (USB_TYPE_CLASS | 0x03)
/*
 *           * HID requests
 *            */
#define USB_REQ_GET_REPORT   0x01
#define USB_REQ_GET_IDLE     0x02
#define USB_REQ_GET_PROTOCOL     0x03
#define USB_REQ_SET_REPORT   0x09
#define USB_REQ_SET_IDLE     0x0A
#define USB_REQ_SET_PROTOCOL     0x0B
u32 hid_desc_config(const usb_dev usb_id, u8 *ptr, u32 *cur_itf_num);
void hid_key_handler(struct usb_device_t *usb_device, u32 hid_key);
u32 hid_send_data(const void *p, u32 len);

u32 hid_register(const usb_dev usb_id);
void hid_release(const usb_dev usb_id);



#endif  /*HID_H*/
