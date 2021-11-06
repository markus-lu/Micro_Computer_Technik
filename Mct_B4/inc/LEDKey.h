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

struct ledkey {
    void (*init)(void);
    void (*loop_once)(void);
};

extern const struct ledkey LEDKey;

