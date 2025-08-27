#ifndef _USB_CONFIG_BOARDS_H
#define _USB_CONFIG_BOARDS_H

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

#endif 
