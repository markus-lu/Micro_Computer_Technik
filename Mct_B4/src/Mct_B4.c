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

int main() {
    Clock.init();
    Events.init();
    Menu.init();
    I2C.init();
    Serial.init();
    FrontIO.init();

    while (1) {
        Clock.loop_once();
        Menu.loop_once();
    }
    I2C.deinit();
    Serial.deinit();
    return 0;
}
