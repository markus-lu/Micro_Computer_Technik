/*
 ===============================================================================
 Name        : Mct_A3.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "Mct_A3.h"

int main(void) {
	uint32_t keys = 0; // Tastencodes
	uint32_t Faktor1 = 0; // Multiplikator
	uint32_t Faktor2 = 0; // Multiplikand
	uint32_t Produkt = 0; // Produkt der Multiplikation
	uint32_t valuesAvailable = A_B_NOT_AVAILABLE; // welche Werte wurden eingelesen ?

	io_init(); // I/O-Ports initialisieren
	show_result(Produkt); // anfangs alle LEDs aus

	while (1) {
		keys = getkey(); // Taster auslesen

		keyhandler(keys, &Faktor1, &Faktor2, &Produkt, &valuesAvailable); // einlesen + berechnen

		if ((valuesAvailable == A_B_AVAILABLE)
				|| (valuesAvailable == A_B_NOT_AVAILABLE)) {
			show_result(Produkt); // Ergebnis anzeigen
		}
	}
	return 0;
}

void io_init(void) {

	// Eingang Pins

	// Nutze als GPIO
	LPC_PINCON->PINSEL0 = 0;
	LPC_PINCON->PINSEL1 = 0;

	// Nutze als Eingang
	LPC_PINCON->PINMODE0 = 0;
	LPC_PINCON->PINMODE1 = 0;

	// Nutze nur einige Pins
	LPC_GPIO0->FIOMASK = ~ALL_INPUTS;

	// Pins als Eingang
	LPC_GPIO0->FIODIR = ~ALL_INPUTS;

	// Ausgangs Pins

	// Nutze als GPIO
	LPC_PINCON->PINSEL4 = 0;

	// Nutze nur Leds
	LPC_GPIO2->FIOMASK = ~LEDS;

	// Pins als Ausgang
	LPC_GPIO2->FIODIR = LEDS;

}

void show_result(uint32_t res) {

	LPC_GPIO2->FIOPIN = ~(res << 1);

}

uint32_t getkey(void) {

	uint32_t input = ~LPC_GPIO0->FIOPIN;

	//printf("Input: %X \n", input);

	// Bits die graucht werden
	// TA1 -> P0.6
	// Ta2 -> P0.7
	// Ta3 -> P0.8
	// E0 -> P0.2
	// E1 -> P0.3
	// E2 -> P0.21
	// E3 -> P0.16

	uint32_t buttons = (input & (TA1 | TA2 | TA3)) >> 6;

	uint32_t switches = 0;
	switches |= (input & E0) << 1;
	switches |= (input & E1) << 1;
	switches |= (input & E2) >> 16;
	switches |= (input & E3) >> 10;
	uint32_t output = switches | buttons;

	//printf("Output: %X \n", output);
	return output;

}
void delay(uint32_t dtime) {

}

void keyhandler(uint32_t keys, uint32_t *A, uint32_t *B, uint32_t *C,
		uint32_t *valuesAvailable) {

	uint32_t input = keys >> 3;
	if (keys & 1) {
		*A = input;
		*valuesAvailable |= A_AVAILABLE;

	} else if (keys & 2) {
		*B = input;
		*valuesAvailable |= B_AVAILABLE;

	} else if (keys & 4) {
		*A = 0;
		*B = 0;
		*C = 0;
		*valuesAvailable = A_B_NOT_AVAILABLE;
	}

	if (*valuesAvailable == A_B_AVAILABLE) {
		*C = *A * *B;
	}

}

