#include "ch32fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

#include <prototype_boards.h>

#define USE_SOFTWARE_RESET 0

int main()
{
	SystemInit();

	printf("Start\n");

	funGpioInitA();

	funPinMode(LED, GPIO_CFGLR_OUT_10Mhz_PP);
	funPinMode(BUTTON, GPIO_CFGLR_IN_PUPD);

	GpioOf(BUTTON)->OUTDR |= 1 << ((BUTTON)&0xf);

	funDigitalWrite(LED, FUN_HIGH);
	
#ifndef USB_PIN_DPU

	LOCAL_EXP(GPIO,USB_PORT)->CFGLR = (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*USB_PIN_DM);
	LOCAL_EXP(GPIO,USB_PORT)->BSHR = 1<<(USB_PIN_DM + 16);
	
#endif

	Delay_Ms(1); // Ensures USB re-enumeration after bootloader or reset; Spec demand >2.5µs ( TDDIS )
	usb_setup();

	while(1) {
#if RV003USB_EVENT_DEBUGGING
		uint32_t * ue = GetUEvent();
		if( ue )
		{
			printf( "%lu %lx %lx %lx\n", ue[0], ue[1], ue[2], ue[3] );
		}
#endif

#if USE_SOFTWARE_RESET
		if (funDigitalRead(BUTTON) == FUN_LOW) {
			FLASH->BOOT_MODEKEYR = 0x45670123; // KEY1
			FLASH->BOOT_MODEKEYR = 0xCDEF89AB; // KEY2
			FLASH->STATR = 0x4000;
			RCC->RSTSCKR |= 0x1000000;
			PFIC->CFGR = 0xBEEF0080;
		}
#endif		

		Delay_Ms(10);
	}
}

void usb_handle_user_data( struct usb_endpoint * e, int current_endpoint, uint8_t * data, int len, struct rv003usb_internal * ist )
{
	LogUEvent(320, current_endpoint, len, 0 );
}

void usb_handle_user_in_request( struct usb_endpoint * e, uint8_t * scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal * ist )
{
	LogUEvent(310, endp, 0, 0 );
}

void usb_handle_other_control_message(struct usb_endpoint * e, struct usb_urb * s, struct rv003usb_internal * ist) {
	LogUEvent(300, s->wRequestTypeLSBRequestMSB, s->lValueLSBIndexMSB, s->wLength );
}