/*
 ===============================================================================
 Name        : Mct_B4.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include "Mct_B4.h"
#include "I2C.h"
#include "Serial.h"
#include "Menu.h"
#include "LCD.h"
#include "Clock.h"
#include "Events.h"
#include "FrontIO.h"
#include "RGBLED.h"
#include "I2CLEDs.h"
#include "LEDKey.h"

#ifdef __USE_CMSIS

#include "LPC17xx.h"
#include <types.h>

#endif

#include <stdio.h>
#include <cr_section_macros.h>

void delay(uint32_t dtime) {
	volatile uint32_t i;
	for (i = 0; i < dtime; i++) {
		__asm("nop");
	}
}

int main() {
	SystemCoreClockUpdate();
	printf("Hello World!!!\n");

	Serial.init();
	{
		uint8_t data[1];
		data[0] = 0x40;
		Serial.write(data, 1);
	}
	{
		uint8_t data[1];
		data[0] = 0x8F;
		Serial.write(data, 1);
	}



	uint8_t data[17];
	data[0] = 0xC0;

	data[1] = SEG_8;
	data[3] = SEG_9;
	data[5] = SEG_0;
	data[7] = SEG_1;
	data[9] = SEG_4;
	data[11] = SEG_5;
	data[13] = SEG_6;
	data[15] = SEG_7;
	Serial.write(data, 17);


	Serial.deinit();
	return 0;
}
