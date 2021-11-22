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
    LEDKey.init();

    LEDKey.set_brightness(4, true);
    int counter = 0;
    struct DateTime test;

    test.seconds = 11;
    test.minutes = 22;
    test.hours = 11;
    test.weekday = 3;
    test.month = 11;
    test.year = 99;

    RTC.write_time(&test);
    delay(50000000);

    while (true) {
        RTC.read_time(&test);
        printf("Seconds: %d\n", test.seconds);
        printf("Minuten: %d\n", test.minutes);
        printf("Stunden: %d\n", test.hours);
        printf("Wochentag: %d\n", test.weekday);
        printf("Tag: %d\n", test.day);
        printf("Monat: %d\n", test.month);
        printf("Jahr: %d\n", test.year);

        uint8_t digits[16];
        digits[0] = SEG_0;
        digits[1] = SEG_1;
        digits[2] = SEG_2;
        digits[3] = SEG_3;
        digits[4] = SEG_4;
        digits[5] = SEG_5;
        digits[6] = SEG_6;
        digits[7] = SEG_7;
        digits[8] = SEG_8;
        digits[9] = SEG_9;
        digits[10] = SEG_A;
        digits[11] = SEG_B;
        digits[12] = SEG_C;
        digits[13] = SEG_D;
        digits[14] = SEG_E;
        digits[15] = SEG_F;

        LEDKey.set_display_data(8, &digits[counter++]);
        counter %= 8;

        delay(500000000);
    }

    LEDKey.deinit();
    return 0;
}
