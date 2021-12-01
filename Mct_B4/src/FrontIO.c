#include "FrontIO.h"
#include "I2C.h"

/**
 *  \file     FrontIO.c
*/

/*********************************************************************/
/**
Diese Funktion initialisiert I²C für die IO auf der Frontplatte

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void frontio_init() {
	// Initialisierung I2C
    i2c_init(I2C_SPEED);
}

/*********************************************************************/
/**
Diese Funktion setzt die Ausgänge des Portexpanders auf der Frontplatte.

\param  leds
        Gewünschte LED Zustände
        (0x00 - 0x0F [ein Bit pro LED])

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void frontio_set_leds(uint8_t leds) {
	// Speicher Reservieren
    uint8_t data[1];
    // Untere Nibble Invertiert
    // LEDs sind Low-Aktiv
    // Pull-Up Wiedersatnd
    data[0] = ~(leds & 0x0F);
    // LEDs über I2C beschreiben
    i2c_write(PCF8574A_ADDRESS, data, 1);
}

/*********************************************************************/
/**
Diese Funktion liest die Eingänge des Portexpanders auf der Frontplatte.

\param  -

\return Tasterzustände<br>
        1 = gedrückt, 0 = nicht gedrückt<br>
        (0x00 - 0x0F [ein Bit pro Taster])

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint8_t frontio_get_buttons() {
	// Speicher Reservieren
    uint8_t data[1];
    // Taster über I2C auslesen
    i2c_read(PCF8574A_ADDRESS, data, 1);
    // Taster sind Low-Aktiv
	int inverted = ~data[0];
	// Taster sind im Oberen Nibble
	// werden ins Untere Geschoben
	int shifted = inverted >> 4;
    // Nur den unteren Nibble zurückgeben
	return shifted & 0x0F;
}
