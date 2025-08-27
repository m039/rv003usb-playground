#ifndef _USB_CONFIG_H
#define _USB_CONFIG_H

#include "funconfig.h"

#define LOCAL_CONCAT(A, B) A##B
#define LOCAL_EXP(A, B) LOCAL_CONCAT(A,B)

//Defines the number of endpoints for this device. (Always add one for EP0). For two EPs, this should be 3.
#define ENDPOINTS 3

#include <usb_config_boards.h>

#define RV003USB_OPTIMIZE_FLASH 0
#define RV003USB_HANDLE_IN_REQUEST 1
#define RV003USB_OTHER_CONTROL 1
#define RV003USB_HANDLE_USER_DATA 1
#define RV003USB_HID_FEATURES 0

#define RV003USB_EVENT_DEBUGGING 1
// #define RV003USB_USE_REBOOT_FEATURE_REPORT 0

#ifndef __ASSEMBLER__

#include <tinyusb_hid.h>
#include <tusb_types.h>

#define INSTANCE_DESCRIPTORS
#ifdef INSTANCE_DESCRIPTORS

// https://www.beyondlogic.org/usbnutshell/usb5.shtml#DeviceDescriptors
static const uint8_t device_descriptor[] = {
	18, //Length
	TUSB_DESC_DEVICE,  //Type (Device)
	0x00, 0x02, //Spec
	TUSB_CLASS_UNSPECIFIED, //Device Class
	0x0, 	//Device Subclass
	0x0, 	//Device Protocol  (000 = use config descriptor)
	0x08, 	//Max packet size for EP0 (This has to be 8 because of the USB Low-Speed Standard)
	0x39, 0x00, //ID Vendor
	0x03, 0x00, //ID Product
	0x01, 0x00, //ID Rev
	1, //Manufacturer string
	2, //Product string
	3, //Serial string
	1, //Max number of configurations
};

#define MSC_SUB_CLASS_SCSI 	6 		// SCSI transparent command set

#define MSC_PROTOCOL_BBB 	0x50 	// USB Mass Storage Class Bulk-Only (BBB) Transport 

static const uint8_t config_descriptor[] = {
	// Configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9,                        // bLength;
	TUSB_DESC_CONFIGURATION,  // bDescriptorType;
	32, 0x00,    	          // wTotalLength
	0x01,                     // bNumInterfaces (Normally 1)
	0x01,                     // bConfigurationValue
	0x00,                     // iConfiguration
	0x80,                     // bmAttributes (was 0xa0)
	0x64,                     // bMaxPower (200mA)

	// Mass Storage
	9,                        // bLength
	TUSB_DESC_INTERFACE,      // bDescriptorType
	0,                        // bInterfaceNumber  = 1 instead of 0 -- well make it second.
	0,                        // bAlternateSetting
	2,                        // bNumEndpoints
	TUSB_CLASS_MSC,           // bInterfaceClass
	MSC_SUB_CLASS_SCSI,       // bInterfaceSubClass
	MSC_PROTOCOL_BBB,    	  // bInterfaceProtocol
	0,                        // iInterface

	// Endpoint
	7,                        // bLength
	TUSB_DESC_ENDPOINT,       // Endpoint Descriptor (Must be 5)
	USB_DIR_IN | 1,           // Endpoint Address
	TUSB_XFER_BULK,      	  // Attributes
	0x08, 0x00,               // Size
	0,                        // Interval

	// Endpoint
	7,                        // bLength
	TUSB_DESC_ENDPOINT,       // Endpoint Descriptor (Must be 5)
	USB_DIR_OUT | 2,          // Endpoint Address
	TUSB_XFER_BULK,      	  // Attributes
	0x08, 0x00,               // Size
	0,                        // Interval
};

#define STR_MANUFACTURER u"m039"
#define STR_PRODUCT      u"CH32V003 Mass Storage Demo"
#define STR_SERIAL       u"000"

struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wString[];
};
const static struct usb_string_descriptor_struct string0 __attribute__((section(".rodata"))) = {
	4,
	3,
	{0x0409}
};
const static struct usb_string_descriptor_struct string1 __attribute__((section(".rodata")))  = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
const static struct usb_string_descriptor_struct string2 __attribute__((section(".rodata")))  = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};
const static struct usb_string_descriptor_struct string3 __attribute__((section(".rodata")))  = {
	sizeof(STR_SERIAL),
	3,
	STR_SERIAL
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex).
const static struct descriptor_list_struct {
	uint32_t	lIndexValue;
	const uint8_t	*addr;
	uint8_t		length;
} descriptor_list[] = {
	{0x00000100, device_descriptor, sizeof(device_descriptor)},
	{0x00000200, config_descriptor, sizeof(config_descriptor)},
	{0x00000300, (const uint8_t *)&string0, 4},
	{0x04090301, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x04090302, (const uint8_t *)&string2, sizeof(STR_PRODUCT)},	
	{0x04090303, (const uint8_t *)&string3, sizeof(STR_SERIAL)}
};
#define DESCRIPTOR_LIST_ENTRIES ((sizeof(descriptor_list))/(sizeof(struct descriptor_list_struct)) )

#endif // INSTANCE_DESCRIPTORS

#endif

#endif 
