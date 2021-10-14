/*
 * Mct_A3.h
 *
 *  Created on: 07.10.2021
 *      Author: marku
 */

#ifndef MCT_A3_H_
#define MCT_A3_H_

#define TA1 (1 << 6)
#define TA2 (1 << 7)
#define TA3 (1 << 8)
#define E0 (1 << 2)
#define E1 (1 << 3)
#define E2 (1 << 21)
#define E3 (1 << 16)
#define LEDS (0b11111111 << 1)

#define ALL_INPUTS (TA1 | TA2 | TA3 | E0 | E1 | E2 | E3)


#define A_B_NOT_AVAILABLE 0b00 		// weder A noch B wurden bisher eingelesen
#define A_AVAILABLE 0b01 			// A wurde eingelesen
#define B_AVAILABLE 0b10 			// B wurde eingelesen
#define A_B_AVAILABLE 0b11 			// A und B wurden bereits eingelesen

void io_init(void); 				// Initialisierung der benutzten Ein-/Ausgabeschnittstellen
void show_result (uint32_t res);	// Ergebnis der Multiplikation auf acht LEDs ausgeben
uint32_t getkey(void); 				// Abfrage und Einlesen der Taster
void delay (uint32_t dtime);		// Erzeugen einer Zeitverzögerung

void keyhandler ( 	uint32_t keys, 	// Auswertung der Tasten,
					uint32_t *A, 	// Berechnung und Setzen von Flags
					uint32_t *B,
					uint32_t *C,
					uint32_t *valuesAvailable);


#endif /* MCT_A3_H_ */
