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

// TODO: Fragen, wie LCD.write_symbols funktioniert (undokumentiert)

void delay(uint32_t dtime) {
	volatile uint32_t i;
	for (i = 0; i < dtime; i++) {
		__asm("nop");
	}
}

int main() {
    SystemCoreClockUpdate();
	struct MenuState menu;
    Menu.init(&menu);

    struct ClockState clock;
    Clock.init(&clock);
    while (1) {
    	delay(5000000);
    }
    Serial.deinit();
	return 0;
}
