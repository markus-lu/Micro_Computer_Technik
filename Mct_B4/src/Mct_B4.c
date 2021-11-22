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

	I2C.init();
	RTC.init();
	struct DateTime test;

	test.seconds = 11;
	test.minutes = 22;
	test.hours = 11;
	test.weekday = 3;
	test.month = 11;
	test.year = 99;

	RTC.write_time(&test);
	delay(100000000);

	while(true){
	RTC.read_time(&test);
	printf("Seconds: %d\n",test.seconds);
	printf("Minuten: %d\n",test.minutes);
	printf("Stunden: %d\n",test.hours);
	printf("Wochentag: %d\n",test.weekday);
	printf("Tag: %d\n",test.day);
	printf("Monat: %d\n",test.month);
	printf("Jahr: %d\n",test.year);
	delay(100000000);
	}



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

	data[1] = SEG_1;
	data[3] = SEG_2;
	data[5] = SEG_3;
	data[7] = SEG_4;
	data[9] = SEG_5;
	data[11] = SEG_6;
	data[13] = SEG_7;
	data[15] = SEG_8;
	Serial.write(data, 17);
	printf("End!!\n");

	Serial.deinit();
	return 0;
}
