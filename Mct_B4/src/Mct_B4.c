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
#include "Clock.h"
#include "Events.h"
#include "FrontIO.h"
#include "RGBLED.h"
#include "I2CLEDs.h"

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

#include <stdio.h>

void delay(uint32_t dtime) {
	volatile uint32_t i;
	for (i = 0; i < dtime; i++) {
		__asm("nop");
	}
}

int main() {
	struct MenuState menu;
//    Clock.init();
//    Events.init();
//    Menu.init();
	I2C.init();
//    Serial.init();
//    FrontIO.init();

//    while (1) {
//        Clock.loop_once();
//        Menu.loop_once(&menu);
//    }
//    I2C.deinit();
//    Serial.deinit();

//    RGBLED.init();
//    while(1){
//    	delay(5000000);
//        RGBLED.set_green();
//    	delay(5000000);
//        RGBLED.set_red();
//    	delay(5000000);
//        RGBLED.set_blue();
//    	delay(5000000);
//        RGBLED.off();
//    }
//    RGBLED.off();
	I2CLEDs.init();
//	while (true) {
//		for (int i = 0; i <= 16; ++i) {
//			I2CLEDs.set_leds(i);
//			delay(5000000);
//		}
//	}

	return 0;
}
