/*
 ===============================================================================
 Name        : LEDKey.h
 Author      : Markus Luckau, Simon Struck
 Version     : 1.0
 Copyright   : Unlicense
 Description : Anzeigen der Uhrzeit und Datum + Auslesen der Taster

 ===============================================================================
 */

#ifndef _LEDKEY_H_
#define _LEDKEY_H_

#include "types.h"
#include "GPIO.h"

/**
 *  \file     LEDKey.h
*/

/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 0
 */
#define SEG_0 0b00111111
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 1
 */
#define SEG_1 0b00000110
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 2
 */
#define SEG_2 0b01011011
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 3
 */
#define SEG_3 0b01001111
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 4
 */
#define SEG_4 0b01100110
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 5
 */
#define SEG_5 0b01101101
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 6
 */
#define SEG_6 0b01111101
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 7
 */
#define SEG_7 0b00000111
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 8
 */
#define SEG_8 0b01111111
/**
 * \brief Bitmuster für 7-Segment-Anzeige der Ziffer 9
 */
#define SEG_9 0b01101111

/**
 * \brief Bitmuster für 7-Segment-Anzeige des Punktes
 */
#define SEG_DOT 0b10000000

/**
 * \brief Befehls-Bits um Daten auf das 7-Segment-Display zu schreiben
 */
#define WRITE_DATA_COMMAND      0b01000000
/**
 * \brief Befehls-Bits um Daten vom den Tastern zu lesen
 */
#define READ_DATA_COMMAND       0b01000010
/**
 * \brief Befehls-Bits um die Adresse auf die geschrieben wird zu setzen
 */
#define SET_ADDRESS_COMMAND     0b11000000
/**
 * \brief Befehls-Bits um das Display zu kontrollieren (z.B. Helligkeit einstellen)
 */
#define DISPLAY_CONTROL_COMMAND 0b10000000
/**
 * \brief Befehls-Bit um das Display einzuschalten
 */
#define DISPLAY_ON      		0b00001000
/**
 * \brief Bitmaske für die Helligkeit des 7-Segment-Displays
 */
#define MAX_BRIGHTNESS 			0b00000111

/**
 * \brief Bit des 1. Buttons von links
 */
#define BUTTON_0 (1 << 6)
/**
 * \brief Bit des 2. Buttons von links
 */
#define BUTTON_1 (1 << 4)
/**
 * \brief Bit des 3. Buttons von links
 */
#define BUTTON_2 (1 << 2)
/**
 * \brief Bit des 4. Buttons von links
 */
#define BUTTON_3 (1 << 0)
/**
 * \brief Bit des 5. Buttons von links
 */
#define BUTTON_4 (1 << 7)
/**
 * \brief Bit des 6. Buttons von links
 */
#define BUTTON_5 (1 << 5)
/**
 * \brief Bit des 7. Buttons von links
 */
#define BUTTON_6 (1 << 3)
/**
 * \brief Bit des 8. Buttons von links
 */
#define BUTTON_7 (1 << 1)

uint8_t ledkey_get_buttons(void);

void ledkey_set_display_data(uint8_t leds, const uint8_t *digits);

void ledkey_set_brightness(uint8_t brightness, bool display_on);

void ledkey_deinit(void);

#endif
