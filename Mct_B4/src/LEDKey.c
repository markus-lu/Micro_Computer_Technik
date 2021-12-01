#include "LEDKey.h"
#include "Serial.h"

/**
 *  \file     LEDKey.c
*/

/*********************************************************************/
/**
\brief Diese Funktion initialisiert das Custom Serial Protocol für das LED&KEY.

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void ledkey_init() {
	// Initialisierung Serielles Protokoll
    serial_init();
}

/*********************************************************************/
/**
\brief Diese Funktion liest aus, welche Taster auf dem LED&KEY gedrückt sind.

\param  -

\return Gedrückte Taster
        Bits entsprechen den in der LEDKey.h definierten BUTTON_0 bis BUTTON_7
        (0x00 - 0xFF [ein Bit pro Taster])

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
uint8_t ledkey_get_buttons() {
	// Speicherreservierung
    uint8_t output = 0;
    uint8_t data[4];
    // Taster lesen
    serial_read(READ_DATA_COMMAND, data, 4);
    // Zurückgegeben werden 4 Bytes
    // Das niedrigste Bit pro Nibble ist ein Taster
    for (int i = 0; i < 4; ++i) {
    	// Output einen nach Links schieben
    	// LSB ist wieder frei
        output <<= 1;
        // Unden mit Höheren Nibble
        // Vergleichen mit 0 sorgt dafür das 1 oder an das LSB geschieben wird
        output |= (data[i] & 0x10) != 0;
        // Output einen nach Links schieben
        // LSB ist wieder frei
        output <<= 1;
        // Unden mit niedrigeren Nibble
        // Vergleichen mit 0 sorgt dafür das 1 oder an das LSB geschieben wird
        output |= (data[i] & 0x01) != 0;
    }
    // Rückgabe der gedrückten Taster
    return output;
}

/*********************************************************************/
/**
\brief Diese Funktion ersetzt die anzuzeigenden LEDS und 7-Segment-Anzeigen.

\param  leds
        Bitmuster für die LEDS über den 7-Segmenten
        (0x00 - 0xFF, ein Bit pro LED, MSB = Links, LSB = Rechts)
\param  digits
        Byte-Array, wobei jedes Byte die das Bitmuster für jedes 7-Segment enthält
        (0x00 [kein Segment leuchtet] - 0xFF [alle Segmente leuchten])

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void ledkey_set_display_data(uint8_t leds, const uint8_t *digits) {
	// Ankündigung zum schreiben
    serial_write_command(WRITE_DATA_COMMAND);
    // Reservierung von Speicher
    uint8_t data[16];

    // 7Seg nur in jedem geraden Register
    // LEDs in jedem Ungeraden
    data[0] = digits[0];
    data[1] = (leds >> 7) & 0b1;
    data[2] = digits[1];
    data[3] = (leds >> 6) & 0b1;
    data[4] = digits[2];
    data[5] = (leds >> 5) & 0b1;
    data[6] = digits[3];
    data[7] = (leds >> 4) & 0b1;
    data[8] = digits[4];
    data[9] = (leds >> 3) & 0b1;
    data[10] = digits[5];
    data[11] = (leds >> 2) & 0b1;
    data[12] = digits[6];
    data[13] = (leds >> 1) & 0b1;
    data[14] = digits[7];
    data[15] = (leds >> 0) & 0b1;

    // 7Seg beschreiben
    serial_write(SET_ADDRESS_COMMAND, data, 16);
}

/*********************************************************************/
/**
\brief Diese Funktion setzt die Helligkeit der 7-Segmente und kann das Display auch ganz ausschalten.

\param  brightness
        Display Helligkeit
        (0 - MAX_BRIGHTNESS [7])
\param  display_on
        Display an
        (false / true)

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void ledkey_set_brightness(uint8_t brightness, bool display_on) {
	// Befehlsbyte zusammenbauen für Helligkeitssteuerung
    uint8_t command = DISPLAY_CONTROL_COMMAND | (brightness & MAX_BRIGHTNESS);
    if (display_on) {
        command |= DISPLAY_ON;
    }
    // Befehl senden
    serial_write_command(command);
}

/*********************************************************************/
/**
\brief Diese Funktion deinitialisiert das Custom Serial Protocol für das LED&KEY.

\param  -

\return -

\version 20.11.2021

\todo -
\bug  keine Fehler bekannt
**********************************************************************/
void ledkey_deinit() {
	// Serielle schnittstelle auschalten
    serial_deinit();
}
