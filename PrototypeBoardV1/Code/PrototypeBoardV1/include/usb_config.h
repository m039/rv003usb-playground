#ifndef _USB_CONFIG_H
#define _USB_CONFIG_H

#include "funconfig.h"

#define LOCAL_CONCAT(A, B) A##B
#define LOCAL_EXP(A, B) LOCAL_CONCAT(A,B)

//Defines the number of endpoints for this device. (Always add one for EP0). For two EPs, this should be 3.
#define ENDPOINTS 3

#if defined(PROTOTYPE_BOARD_V1A)

#define USB_PORT C     // [A,C,D] GPIO Port to use with D+, D- and DPU
#define USB_PIN_DP 2   // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM 1   // [0-4] GPIO Number for USB D- Pin

#elif defined(PROTOTYPE_BOARD_V1B)

#define USB_PORT C     // [A,C,D] GPIO Port to use with D+, D- and DPU
#define USB_PIN_DP 2   // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM 4   // [0-4] GPIO Number for USB D- Pin
#define USB_PIN_DPU 1  // [0-7] GPIO for feeding the 1.5k Pull-Up on USB D- Pin; Comment out if not used / tied to 3V3!

#elif defined(PROTOTYPE_BOARD_V1C)

#define USB_PORT C     // [A,C,D] GPIO Port to use with D+, D- and DPU
#define USB_PIN_DP 2   // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM 4   // [0-4] GPIO Number for USB D- Pin

#elif defined(PROTOTYPE_BOARD_V2A)

#define USB_PORT D     // [A,C,D] GPIO Port to use with D+, D- and DPU
#define USB_PIN_DP 3   // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM 4   // [0-4] GPIO Number for USB D- Pin

#endif

#define RV003USB_OPTIMIZE_FLASH 0
#define RV003USB_HANDLE_IN_REQUEST 1
#define RV003USB_OTHER_CONTROL 1
#define RV003USB_HANDLE_USER_DATA 1
#define RV003USB_HID_FEATURES 0

// #define RV003USB_EVENT_DEBUGGING 1
// #define RV003USB_USE_REBOOT_FEATURE_REPORT 0

#ifndef __ASSEMBLER__

#include <tinyusb_hid.h>
#include <tusb_types.h>

//#define INSTANCE_DESCRIPTORS
#ifdef INSTANCE_DESCRIPTORS

//Taken from http://www.usbmadesimple.co.uk/ums_ms_desc_dev.htm
static const uint8_t device_descriptor[] = {
	18, //Length
	TUSB_DESC_DEVICE,  //Type (Device)
	0x00, 0x02, //Spec
	TUSB_CLASS_UNSPECIFIED, //Device Class
	0x0, //Device Subclass
	0x0, //Device Protocol  (000 = use config descriptor)
	0x08, //Max packet size for EP0 (This has to be 8 because of the USB Low-Speed Standard)
	0x39, 0x00, //ID Vendor
	0x01, 0x00, //ID Product
	0x02, 0x00, //ID Rev
	1, //Manufacturer string
	2, //Product string
	3, //Serial string
	1, //Max number of configurations
};

static const uint8_t keyboard_hid_desc[] = {   /* USB report descriptor */
	HID_USAGE_PAGE( HID_USAGE_PAGE_DESKTOP ),
	HID_USAGE( HID_USAGE_DESKTOP_KEYBOARD ),
	HID_COLLECTION ( HID_COLLECTION_APPLICATION ),
		// Modification key
		HID_USAGE_PAGE( HID_USAGE_PAGE_KEYBOARD ),
    	HID_USAGE_MIN( HID_KEY_CONTROL_LEFT ),
    	HID_USAGE_MAX( HID_KEY_GUI_RIGHT ),
		HID_LOGICAL_MIN( 0 ),
		HID_LOGICAL_MAX( 1 ),
		HID_REPORT_SIZE( 1 ),
		HID_REPORT_COUNT( 8 ),
		HID_INPUT( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
		
		// 1 byte reserved
		HID_REPORT_SIZE( 8 ),
		HID_REPORT_COUNT( 1 ),
		HID_INPUT( HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE),

		// Leds, 5 bits
		HID_USAGE_PAGE( HID_USAGE_PAGE_LED ),
		HID_REPORT_SIZE( 1 ),
		HID_REPORT_COUNT( 5 ),
    	HID_USAGE_MIN( 0x01 ), // Num Lock
	    HID_USAGE_MAX( 0x05 ), // Kana
		HID_LOGICAL_MIN( 0 ),
		HID_LOGICAL_MAX( 1 ),
		HID_OUTPUT( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),
		
		// Padding, 3 bits
		HID_REPORT_SIZE( 3 ),
		HID_REPORT_COUNT( 1 ),
		HID_OUTPUT( HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE ),

		// Padding to fill the rest of the buffer to 8 bytes.
		HID_REPORT_SIZE( 8 ),
		HID_REPORT_COUNT( 7 ),
		HID_OUTPUT( HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE ),

		// 6 Keys
		HID_USAGE_PAGE( HID_USAGE_PAGE_KEYBOARD ),
		HID_REPORT_SIZE( 8 ),
		HID_REPORT_COUNT( 6 ),
		HID_LOGICAL_MIN( 0 ),
		HID_LOGICAL_MAX( 0xff ),
    	HID_USAGE_MIN( 0 ),
	    HID_USAGE_MAX( 0xff ),
		HID_INPUT( HID_DATA | HID_ARRAY | HID_ABSOLUTE ),
    HID_COLLECTION_END,
};

static const uint8_t keyboard_media_hid_desc[] = { /* USB report descriptor */
	HID_USAGE_PAGE( HID_USAGE_PAGE_CONSUMER ),
	HID_USAGE( HID_USAGE_CONSUMER_CONTROL ),
	HID_COLLECTION ( HID_COLLECTION_APPLICATION ),
	    // Mute key code
		HID_USAGE ( HID_USAGE_CONSUMER_MUTE ), 
		HID_LOGICAL_MIN( 0 ),
		HID_LOGICAL_MAX( 1 ),
		HID_REPORT_SIZE( 1 ),
		HID_REPORT_COUNT( 1 ),
		HID_INPUT( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),

		// Padding, 7 bits reserved
		HID_REPORT_SIZE( 7 ),
		HID_REPORT_COUNT( 1 ),
		HID_INPUT( HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE ),
	HID_COLLECTION_END,
};

static const uint8_t config_descriptor[] = {
	// Configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
	9,                        // bLength;
	TUSB_DESC_CONFIGURATION,  // bDescriptorType;
	0x3b, 0x00,               // wTotalLength
	0x02,                     // bNumInterfaces (Normally 1)
	0x01,                     // bConfigurationValue
	0x00,                     // iConfiguration
	0x80,                     // bmAttributes (was 0xa0)
	0x64,                     // bMaxPower (200mA)

	// Keyboard 1
	9,                        // bLength
	TUSB_DESC_INTERFACE,      // bDescriptorType
	0,                        // bInterfaceNumber  = 1 instead of 0 -- well make it second.
	0,                        // bAlternateSetting
	1,                        // bNumEndpoints
	TUSB_CLASS_HID,           // bInterfaceClass (0x03 = HID)
	HID_SUBCLASS_BOOT,        // bInterfaceSubClass
	HID_PROTOCOL_KEYBOARD,    // bInterfaceProtocol (1 = Keyboard, 2 = Mouse)
	0,                        // iInterface

	9,                        // bLength
	HID_DESC_TYPE_HID,        // bDescriptorType (HID)
	0x11, 0x01,               // bcd 1.11
	0x00,                     //country code
	0x01,                     // Num descriptors
	HID_DESC_TYPE_REPORT,     // DescriptorType[0] (HID)
	sizeof(keyboard_hid_desc), 0x00,

	7,                        // bLength
	TUSB_DESC_ENDPOINT,       // Endpoint Descriptor (Must be 5)
	0x81,                     // Endpoint Address
	0x03,                     // Attributes
	0x08, 0x00,               // Size
	10,                       // Interval

	// Keyboard 2 
	9,                        // bLength
	TUSB_DESC_INTERFACE,      // bDescriptorType
	1,                        // bInterfaceNumber  = 1 instead of 0 -- well make it second.
	0,                        // bAlternateSetting
	1,                        // bNumEndpoints
	TUSB_CLASS_HID,           // bInterfaceClass (0x03 = HID)
	HID_SUBCLASS_BOOT,        // bInterfaceSubClass
	HID_PROTOCOL_KEYBOARD,    // bInterfaceProtocol (1 = Keyboard, 2 = Mouse)
	0,                        // iInterface

	9,                        // bLength
	HID_DESC_TYPE_HID,        // bDescriptorType (HID)
	0x11, 0x01,               // bcd 1.11
	0x00,                     //country code
	0x01,                     // Num descriptors
	HID_DESC_TYPE_REPORT,     // DescriptorType[0] (HID)
	sizeof(keyboard_media_hid_desc), 0x00,

	7,                        // bLength
	TUSB_DESC_ENDPOINT,       // Endpoint Descriptor (Must be 5)
	0x82,                     // Endpoint Address
	0x03,                     // Attributes
	0x08, 0x00,               // Size
	10,                       // Interval
};

#define STR_MANUFACTURER u"m039"
#define STR_PRODUCT      u"CH32V003 PrototypeBoard"
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
	{0x00002200, keyboard_hid_desc, sizeof(keyboard_hid_desc)},
	{0x00012200, keyboard_media_hid_desc, sizeof(keyboard_media_hid_desc)},
	{0x00000300, (const uint8_t *)&string0, 4},
	{0x04090301, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x04090302, (const uint8_t *)&string2, sizeof(STR_PRODUCT)},	
	{0x04090303, (const uint8_t *)&string3, sizeof(STR_SERIAL)}
};
#define DESCRIPTOR_LIST_ENTRIES ((sizeof(descriptor_list))/(sizeof(struct descriptor_list_struct)) )

#endif // INSTANCE_DESCRIPTORS

#endif

#endif 
