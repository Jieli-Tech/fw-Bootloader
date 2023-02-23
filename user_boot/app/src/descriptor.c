#include "usb/usb_stack.h"
#include "string.h"
#include "printf.h"
#include "irq.h"

static const u8 sDeviceDescriptor[] = { //<Device Descriptor
    USB_DT_DEVICE_SIZE,      // bLength: Size of descriptor
    USB_DT_DEVICE,       // bDescriptorType: Device
#if defined(FUSB_MODE) && FUSB_MODE
    0x10, 0x01,     // bcdUSB: USB 1.1
#elif defined(FUSB_MODE) && (FUSB_MODE ==0 )
    0x00, 0x02,     // bcdUSB: USB 2.0
#else
#error "USB_SPEED_MODE not defined"
#endif
    0x00,       // bDeviceClass: none
    0x00,       // bDeviceSubClass: none
    0x00,       // bDeviceProtocol: none
    EP0_SETUP_LEN,//EP0_LEN,      // bMaxPacketSize0: 8/64 bytes
    'J', 'L',     // idVendor: 0x4a4c - JL
    'U', 'A',     // idProduct: chip id
    0x00, 0x01,     // bcdDevice: version 1.0
    0x01,       // iManufacturer: Index to string descriptor that contains the string <Your Name> in Unicode
    0x02,       // iProduct: Index to string descriptor that contains the string <Your Product Name> in Unicode
    0x00,       // iSerialNumber: none
    0x01        // bNumConfigurations: 1
};

static const u8 LANGUAGE_STR[] = {
    0x04, 0x03, 0x09, 0x04
};
static const u8 product_string[] = {
    42,
    0x03,
    'U', 0x00,
    'S', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'C', 0x00,
    'o', 0x00,
    'm', 0x00,
    'p', 0x00,
    'o', 0x00,
    's', 0x00,
    'i', 0x00,
    't', 0x00,
    'e', 0x00,
    ' ', 0x00,
    'D', 0x00,
    'e', 0x00,
    'v', 0x00,
    'i', 0x00,
    'c', 0x00,
    'e', 0x00,
};
static const u8 MANUFACTURE_STR[] = {
    34,         //该描述符的长度为34字节
    0x03,       //字符串描述符的类型编码为0x03
    0x4a, 0x00, //J
    0x69, 0x00, //i
    0x65, 0x00, //e
    0x6c, 0x00, //l
    0x69, 0x00, //i
    0x20, 0x00, //
    0x54, 0x00, //T
    0x65, 0x00, //e
    0x63, 0x00, //c
    0x68, 0x00, //h
    0x6e, 0x00, //n
    0x6f, 0x00, //o
    0x6c, 0x00, //l
    0x6f, 0x00, //o
    0x67, 0x00, //g
    0x79, 0x00, //y
};

static const u8 sConfigDescriptor[] = {	//<Config Descriptor
//ConfiguraTIon
    USB_DT_CONFIG_SIZE,    //bLength
    USB_DT_CONFIG,    //DescriptorType : ConfigDescriptor
    0, 0, //TotalLength
    0,//bNumInterfaces: 在set_descriptor函数里面计算
    0x01,    //bConfigurationValue - ID of this configuration
    0x00,    //Unused
    0x80,    //Attributes:Bus Power
    200,     //MaxPower = 200*2ma
};
void get_device_descriptor(u8 *ptr)
{
    memcpy(ptr, sDeviceDescriptor, USB_DT_DEVICE_SIZE);
}
void get_language_str(u8 *ptr)
{
    memcpy(ptr, LANGUAGE_STR, LANGUAGE_STR[0]);
}
void get_manufacture_str(u8 *ptr)
{
    memcpy(ptr, MANUFACTURE_STR, MANUFACTURE_STR[0]);
}
void get_product_str(u8 *ptr)
{
    memcpy(ptr, product_string, product_string[0]);
}
void get_iserialnumber_str(u8 *ptr)
{
}

void get_string_ee(u8 *ptr)
{

}
const u8 *usb_get_config_desc()
{
    return sConfigDescriptor;

}
