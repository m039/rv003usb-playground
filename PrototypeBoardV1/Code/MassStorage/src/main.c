#include "ch32fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

#define USE_SOFTWARE_RESET 0
#define USE_KEYBOARD_TEST 0

#define DEBOUNCE_TIME Ticks_from_Ms(50)

#define LED PA1
#define BUTTON PA2

uint32_t lastDebounceTime = 0;
volatile uint8_t buttonPressed = 0;
uint8_t debounceState = FUN_HIGH;
uint8_t lastDebounceState = FUN_HIGH;

static uint8_t buffer[32];

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

	buffer[0] = 0x55;
	buffer[1] = 0x53;
	buffer[2] = 0x42;
	buffer[3] = 0x53;

	usb_send_data(buffer, 13, 0, 0b11000011);

	while(1) {
#if RV003USB_EVENT_DEBUGGING
		uint32_t * ue = GetUEvent();
		if( ue )
		{
			printf( "%lu %lx %lx %lx\n", ue[0], ue[1], ue[2], ue[3] );
		}
#endif

		uint32_t currentTime = SysTick->CNT;
		uint8_t measuredState = funDigitalRead(BUTTON);

		if (lastDebounceState != debounceState) {
			lastDebounceTime = currentTime;
			lastDebounceState = measuredState;
		}

		if ((currentTime - lastDebounceTime > DEBOUNCE_TIME || lastDebounceTime > currentTime) && debounceState != measuredState) {
			debounceState = measuredState;
			if (measuredState == FUN_LOW) {
				buttonPressed = 1;
			}
		}

#if USE_SOFTWARE_RESET
		if (buttonPressed) {
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

hid_keyboard_report_t buttonData = {0};

void usb_handle_user_data( struct usb_endpoint * e, int current_endpoint, uint8_t * data, int len, struct rv003usb_internal * ist )
{
	LogUEvent(37, current_endpoint, len, 0 );
}

void usb_handle_user_in_request( struct usb_endpoint * e, uint8_t * scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal * ist )
{
		
	LogUEvent(38, endp, 0, 0 );
		
	usb_send_data(buffer, 8, 1, sendtok);
}

void usb_handle_other_control_message(struct usb_endpoint * e, struct usb_urb * s, struct rv003usb_internal * ist) {
	buffer[0] = 0x55;
	buffer[1] = 0x53;
	buffer[2] = 0x42;
	buffer[3] = 0x53;

	e->opaque = buffer;
	e->max_len = 13;
	
	LogUEvent(420, s->wRequestTypeLSBRequestMSB, s->lValueLSBIndexMSB, s->wLength );
}