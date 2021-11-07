/*
 ===============================================================================
 Name        : LEDKey.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen der Uhrzeit und Datum/auslesen von taster ← Protokoll
			   selber schreiben

 ===============================================================================
 */

#ifndef _LEDKEY_H_
#define _LEDKEY_H_

#include "GPIO.h"

struct ledkey {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct ledkey LEDKey;

#endif
